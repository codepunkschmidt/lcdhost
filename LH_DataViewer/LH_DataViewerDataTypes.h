#ifndef LH_DATAVIEWERDATANODE_H
#define LH_DATAVIEWERDATANODE_H

#include <QString>
#include <QHash>
#include <QMutex>

#define polling_rate 200

#ifdef Q_WS_WIN
#include <windows.h>
#include <winbase.h>
#include <Psapi.h>
#include <tlhelp32.h>
#else
    typedef unsigned long DWORD;
#endif

enum MemoryDataType
{
    MEMTYPE_NONE = 0,
    MEMTYPE_4BYTE,
    MEMTYPE_8BYTE,
    MEMTYPE_FLOAT,
    MEMTYPE_DOUBLE
};

struct itemDefinition
{
    QString name;
    QString address;
    int x;
    int y;
    QString formatting;
    bool hasData;
    uint memory;
    uint *offsets;
    MemoryDataType memoryDataType;
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

#ifdef Q_WS_WIN
    HANDLE processHandle_;
    DWORD processID_;

    bool getProcessValue(uint address, uint offsets[], void *dest, SIZE_T len)
    {
        SIZE_T r;
        HANDLE hProcess = processHandle(); //get the handle from the root node
        if(!hProcess)
            return false;
        else
        {
            if(offsets)
            {
                int offsetCount = sizeof(offsets)/sizeof(uint);
                for(int i = 0; i<offsetCount; i++)
                {
                    if(!getProcessValue(address, NULL, &address, 4))
                       return false;
                    address += offsets[i];
                }
            }
            if(len==0) len = sizeof(typeof(dest));
            BOOL ok=ReadProcessMemory(hProcess, (BYTE *) address, dest, len, &r);
            return (ok && (r == len));
        }
    }

    QString getProcessValue() {
        int val_int;
        float val_flt;
        double val_dbl;
        qlonglong val_lng;

        bool ok;
        switch(definition_.memoryDataType)
        {
        case MEMTYPE_4BYTE:
            ok = getProcessValue(definition_.memory, definition_.offsets, &val_int, 4);
            if(ok)
                return QString::number(val_int);
            break;
        case MEMTYPE_8BYTE:
            ok = getProcessValue(definition_.memory, definition_.offsets, &val_lng, 8);
            if(ok)
                return QString::number(val_lng);
            break;
        case MEMTYPE_FLOAT:
            ok = getProcessValue(definition_.memory, definition_.offsets, &val_flt, 4);
            if(ok)
                return QString::number(val_flt);
            break;
        case MEMTYPE_DOUBLE:
            ok = getProcessValue(definition_.memory, definition_.offsets, &val_dbl, 8);
            if(ok)
                return QString::number(val_dbl);
            break;
        case MEMTYPE_NONE:
            return "??";
        }
        return "??";
    }

    bool validatePID(DWORD pid, QString exeFile)
    {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if ( processesSnapshot == INVALID_HANDLE_VALUE )
            return false;

        bool searching = Process32First(processesSnapshot, &processInfo);
        while(searching)
        {
            if ( pid == processInfo.th32ProcessID )
            {
                CloseHandle(processesSnapshot);
                return ( exeFile == QString::fromWCharArray(processInfo.szExeFile) );
            }
            searching = Process32Next(processesSnapshot, &processInfo);
        }

        CloseHandle(processesSnapshot);
        return false;
    }

    DWORD getProcessId(QString exeFile)
    {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if ( processesSnapshot == INVALID_HANDLE_VALUE )
            return 0;

        bool searching = Process32First(processesSnapshot, &processInfo);
        while(searching)
        {
            if ( exeFile == QString::fromWCharArray(processInfo.szExeFile) )
            {
                CloseHandle(processesSnapshot);
                return processInfo.th32ProcessID;
            }
            searching = Process32Next(processesSnapshot, &processInfo);
        }

        CloseHandle(processesSnapshot);
        return 0;
    }
#endif

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
    void clearValues()
    {
        setValue("");
        foreach(QString key, childNodes_.keys())
            for(int i = 0; i < childNodes_[key].count(); i++)
                childNodes_[key][i]->clearValues();
    }

    void resetCursors()
    {
        cursorPositions_.clear();
    }

    dataNode(dataNode* parentNode = 0, itemDefinition def = (itemDefinition){"","",-1,-1,"",false,0,NULL,MEMTYPE_NONE }, QString nodeValue = "" )
    {
        mutex = new QMutex(QMutex::Recursive);
        value_ = nodeValue;
        definition_ = def;
        parentNode_ = parentNode;
        childNodes_ = QHash<QString, QList<dataNode*> >();
        processHandle_ = NULL;
    }

    ~dataNode()
    {
        mutex->lock();
        if(definition_.offsets)
        {
            delete definition_.offsets; // TODO: should this be delete[]?
            definition_.offsets = NULL;
        }
#ifdef Q_WS_WIN
        if(processHandle_)
            CloseHandle(processHandle_);
#endif
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
        return addChild((itemDefinition){name,"",-1,-1,"",false,0,NULL,MEMTYPE_NONE}, val);
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
            childNodes_[name].append(new dataNode(this,(itemDefinition){name,"",-1,-1,"",false,0,NULL,MEMTYPE_NONE}, val));
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

    void debugTree()
    {
        qDebug() << this->address() << " = " << this->value();
        for(int i = 0; i < childNodes_.count(); i++)
        {
            QList<dataNode*> childList = child(keys().at(i));
            for(int j = 0; j < childList.count(); j++)
                childList[j]->debugTree();
        }
    }

    bool refreshProcessValues()
    {
        bool changed = false;
#ifdef Q_WS_WIN
        QString newVal = getProcessValue();
        if(newVal!=value_)
        {
            setValue( newVal );
            changed |= true;
        }

        foreach(QString key, childNodes_.keys())
            for(int i = 0; i < childNodes_[key].count(); i++)
                changed |= childNodes_[key][i]->refreshProcessValues();
#endif
        return changed;
    }

    bool openProcess(QString exeFile, QString &feedbackMessage)
    {
#ifdef Q_WS_WIN
        feedbackMessage = "";
        if(processHandle_)
        {
            if(validatePID(processID_, exeFile))
                return true;
            else
            {
                feedbackMessage = "LH_DataViewer: Process ID no longer valid.";
                processID_ = 0;
                CloseHandle(processHandle_);
                processHandle_ = NULL;
            }
        }

        if (exeFile=="")
        {
            feedbackMessage = "No process specified";
            return false;
        }

        processID_ = getProcessId(exeFile);

        if ( !processID_ )
        {
            feedbackMessage = QString("Could not find process \"%1\"").arg(exeFile);
            return false;
        }

        processHandle_ = OpenProcess(PROCESS_VM_READ, false, processID_);
        if ( !processHandle_ )
        {
            feedbackMessage = QString("Could not open process \"%1\"").arg(exeFile);
            return false;
        }

        refreshProcessValues();
        return true;
#else
        Q_UNUSED(exeFile);
        feedbackMessage = "Direct memory access is unsupported on this operating system"
        return false;
#endif
    }

#ifdef Q_WS_WIN
    HANDLE processHandle()
    {
        if(parentNode_)
            return parentNode()->processHandle();
        else
            return processHandle_;
    }

    uint memoryAddress()
    {
        return definition_.memory;
    }

    uint *memoryOffsets()
    {
        return definition_.offsets;
    }
#endif
};




#endif // LH_DATAVIEWERDATANODE_H
