#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <QList>
#include <QStringList>
#include <QDebug>

typedef struct {qreal value; int duration;} DataPoint;

class DataLine : public QList<DataPoint>
{
    friend class DataLineCollection;

    int limit_;
    int totalDuration_;
    int popDuration_;

    void setPopDuration(int popDuration) { popDuration_ = popDuration;}

    void copyFrom(DataLine src)
    {
        limit_ = src.limit_;
        totalDuration_ = src.totalDuration_;
        popDuration_ = src.popDuration_;
        name = src.name;

        qDebug() << src.count();
        for(int i = 0; i<src.count(); i++)
            append( src[i] );
    }

    DataLine() : QList<DataPoint>()
    {
        name = "Unnamed";
        limit_ = 0;
        totalDuration_ = 0;
        popDuration_ = 0;
    }

public:
    QString name;

    DataLine(QString name, int limit) : QList<DataPoint>()
    {
        name = name;
        limit_ = limit;
        totalDuration_ = 0;
        popDuration_ = 0;
    }

    int limit() { return limit_; }

    qreal averageDuration()
    {
        if(length()==0)
            return 0;
        else
            return ((qreal)totalDuration_)/(length());
    }

    qreal predictedTotalDuration()
    {
        return averageDuration() * limit_;
    }

    void setLimit(int limit)
    {
        limit_ = limit;
        while(limit_<length())
            removeLast();
    }

    void clear()
    {
        QList<DataPoint>::clear();
        totalDuration_ = 0;
    }

    void addValue(qreal value, int duration)
    {
        int popped_duration = 0;
        bool popped_once = false;
        if (length()>=limit_)
        {
            if(popDuration_ != 0) qDebug() << "popDuration_: " << popDuration_;
            while( (!popped_once) || (popped_duration + last().duration) <= popDuration_ )
            {
                popped_once = true;
                popped_duration += last().duration;
                totalDuration_ -= last().duration;
                pop_back();
                if(popDuration_ != 0) qDebug() << "pop";
            }
            if(popDuration_ != 0) qDebug() << "popped: " << popped_duration;
        }
        push_front( (DataPoint){value, duration} );
        totalDuration_ += duration;
    }
};

class DataLineCollection : public QList<DataLine>
{
    int limit_;

private:
    void setNames(QStringList names)
    {
        this->clear();
        for(int i = 0; i < names.count(); i++)
            add(names[i]);
    }

public:
    DataLineCollection() : QList<DataLine>() {
        limit_ = 0;
    }
    DataLineCollection(int limit) : QList<DataLine>() {
        limit_ = limit;
    }

    int limit() { return limit_; }

    int pointLength()
    {
        return (length()==0? 0 : (*this)[0].length());
    }

    void add(QString name)
    {
        this->append( DataLine(name, limit_) );
    }

    void setCount(int c, QString nameTemplate = "line %1")
    {
        this->clear();
        for(int i = 0; i < c; i++)
            add(nameTemplate.arg(i));
    }

    QStringList names()
    {
        QStringList names;
        for(int i = 0; i < count(); i++)
            names.append(this->at(i).name);
        return names;
    }

    void setLimit(int limit)
    {
        limit_ = limit;
        for(int i =0; i<length(); i++ )
            (*this)[i].setLimit(limit);
    }

    DataLineCollection averageOver(int desiredDuration)
    {
        DataLineCollection dlc;
        dlc.setLimit(limit_);
        dlc.setNames(this->names());
        //qDebug() << "averageOver " << desiredDuration << " ms";
        for(int lineIndex = 0; lineIndex < dlc.count(); lineIndex++)
        {
            QList<DataLine>::operator [](lineIndex).setPopDuration(desiredDuration);

            DataLine dl = this->at(lineIndex);

            qreal value = 0;
            int duration = 0;
            int count = 0;
            for(int i = dl.length() - 1; i >=0; i--)
            {
                value += dl.at(i).value;
                duration += dl.at(i).duration;
                count ++;

                if(duration == desiredDuration || (i > 0 && duration + dl.at(i-1).duration > desiredDuration))
                {
                    dlc[lineIndex].addValue(value / count, desiredDuration);
                    value = 0;
                    duration = 0;
                    count = 0;
                }
            }
        }
        return dlc;
    }

    void copyFrom(DataLineCollection src)
    {
        limit_ = src.limit();
        //setNames(src.names());
        for(int lineIndex = 0; lineIndex < src.count(); lineIndex++)
        {
            DataLine dl;
            dl.copyFrom(src[lineIndex]);
            append( dl );
        }
    }
}
;

#endif // DATACOLLECTION_H
