#ifndef LH_DATAVIEWERDATANODE_H
#define LH_DATAVIEWERDATANODE_H

#include <QString>
#include <QHash>
#include <QMutex>

#define polling_rate 200

struct itemDefinition
{
    QString name;
    QString address;
    int x;
    int y;
    QString formatting;
    bool hasData;
};

struct sharedItem
{
    QString name;
    QString value;
};

class sharedCollection
{
    QList<sharedItem> items_;
public:
    bool valid;
    QString lastUpdated;
    QString expiresAt;

    void clear(int createCount = 0)
    {
        valid = false;
        lastUpdated = "";
        expiresAt = "N/A";
        items_.clear();

        for(int i=0; i<createCount; i++)
            items_.append((sharedItem){"",""});
    }

    int count()
    {
        return items_.count();
    }

    sharedItem item(int index)
    {
        return items_[index];
    }

    sharedItem operator[](int index)
    {
        return items_[index];
    }

    void setItem(int index, QString name, QString value = "" )
    {
        items_[index].name = name;
        items_[index].value = value;
    }
};

class dataNode
{
    QString defaultItem_;

    itemDefinition definition_;
    QString value_;

    QHash<QString, QList<dataNode*> > childNodes_;
    dataNode* parentNode_;

    QHash<QString, int> cursorPositions_;
protected:
    QMutex* mutex;

    void divorce()
    {
        parentNode_ = 0;
    }
    void clear()
    {
        foreach(QString key, childNodes_.keys())
        {
            while (childNodes_[key].count()>0)
            {
                dataNode* temp = childNodes_[key].last();
                childNodes_[key].removeLast();
                delete temp;
            }
        }
    }
public:
    void resetCursors()
    {
        cursorPositions_.clear();
    }

    dataNode(dataNode* parentNode = 0, itemDefinition def = (itemDefinition){"","",-1,-1,"",false}, QString nodeValue = "" )
    {
        mutex = new QMutex(QMutex::Recursive);
        value_ = nodeValue;
        definition_ = def;
        parentNode_ = parentNode;
        childNodes_ = QHash<QString, QList<dataNode*> >();
    }

    ~dataNode()
    {
        mutex->lock();
        clear();
        divorce();
        mutex->unlock();
        delete mutex;
    }

    dataNode *parentNode()
    {
        return parentNode_;
    }

    QString value()
    {
        return value_;
    }
    QString name()
    {
        return definition_.name;
    }
    itemDefinition definition()
    {
        return definition_;
    }
    void setValue(QString val)
    {
        mutex->lock();
        value_ = val;
        mutex->unlock();
    }
    dataNode* addChild(QString name, QString val = "")
    {
        return addChild((itemDefinition){name,"",-1,-1,"",false}, val);
    }
    dataNode* addChild(itemDefinition def, QString val = "")
    {
        mutex->lock();
        //remove the * if present and set as default item
        if(def.name.startsWith("*"))
        {
            Q_ASSERT(defaultItem_ == "");
            def.name = def.name.remove(0,1);
            defaultItem_ = def.name.toLower();
        }

        //if new, add a new entry in the children collection
        if(!childNodes_.contains(def.name.toLower()))
            childNodes_.insert(def.name.toLower(), QList<dataNode*>());

        //add a new node to the collection for this named child
        childNodes_[def.name.toLower()].append(new dataNode(this,def,val));
        return childNodes_[def.name.toLower()].last();
        mutex->unlock();
    }

    dataNode* openChild(QString name, QString val = "")
    {
        name = name.toLower();

        if(name.startsWith("*"))
        {
            name = name.remove(0,1);
            Q_ASSERT(defaultItem_ == "" || defaultItem_ == name);
            defaultItem_ = name.toLower();
        }

        //if new, add a new entry in the children collection
        if(!childNodes_.contains(name))
            childNodes_.insert(name, QList<dataNode*>());

        if(!cursorPositions_.contains(name))
            cursorPositions_.insert(name,0);

        if(cursorPositions_[name] >= childNodes_[name].count())
            childNodes_[name].append(new dataNode(this,(itemDefinition){name,"",-1,-1,"",false}, val));
        else
            childNodes_[name][cursorPositions_[name]]->setValue(val);

        cursorPositions_[name]++;
        childNodes_[name][cursorPositions_[name]-1]->resetCursors();
        return childNodes_[name][cursorPositions_[name]-1];
    }


    QList<dataNode*> operator[](QString name)
    {
        return childNodes_[name.toLower()];
    }
    QList<dataNode*> child(QString name)
    {
        return childNodes_[name.toLower()];
    }
    QString defaultItem()
    {
        return defaultItem_;
    }

    bool contains(QString name)
    {
        return childNodes_.contains(name.toLower());
    }

    QList<QString> keys()
    {
        return childNodes_.keys();
    }

    bool hasData()
    {
        return definition_.hasData;
    }

    bool hasChildren()
    {
        return (childNodes_.count()!=0);
    }

    QString address(QString childNode = "")
    {
        QString nodeAddress = "";
        dataNode* currentNode = this;
        while (currentNode != 0 && currentNode->parentNode()!=0)
        {
            nodeAddress = QString("%1%2%3").arg(currentNode->name()).arg(nodeAddress==""?"":".").arg(nodeAddress);
            currentNode = currentNode->parentNode();
        }
        if(childNode!="")
            nodeAddress = QString("%1%2%3").arg(nodeAddress).arg(nodeAddress==""?"":".").arg(childNode);
        return nodeAddress.toLower();
    }
};




#endif // LH_DATAVIEWERDATANODE_H
