#ifndef LH_TS3_CHANNELLIST_H
#define LH_TS3_CHANNELLIST_H

#include <QHash>

struct channeldetail
{
    int cid;
    int pid;
    int order;
    QString name;
    int clientCount;
    bool neededSubscribePower;
};

class channellist: public QHash<int,channeldetail>
{
public:
    bool load(QString responseMsg)
    {
        if(!responseMsg.contains("channel_name=") || !responseMsg.contains("cid=") )
            return false;
        this->clear();
        QStringList list = responseMsg.split('|');
        QRegExp rx("(cid|pid|channel_order|channel_name|total_clients|channel_needed_subscribe_power)\\s*=\\s*(\\S+)");
        int pos = 0;
        foreach(QString item, list)
        {
            channeldetail chan;
            pos = rx.indexIn(item,0);
            while(pos != -1)
            {
                if(rx.cap(1)=="cid")
                    chan.cid = rx.cap(2).toInt();
                if(rx.cap(1)=="pid")
                    chan.pid = rx.cap(2).toInt();
                if(rx.cap(1)=="channel_order")
                    chan.order = rx.cap(2).toInt();
                if(rx.cap(1)=="channel_name")
                    chan.name = rx.cap(2).replace("\\s"," ");;
                if(rx.cap(1)=="total_clients")
                    chan.clientCount = rx.cap(2).toInt();
                if(rx.cap(1)=="channel_needed_subscribe_power")
                    chan.neededSubscribePower = (rx.cap(2).toInt()!=0);
                pos += rx.matchedLength();
                pos = rx.indexIn(item,pos);
            }
            insert(chan.cid, chan);
        }
        return true;
    }
};

#endif // LH_TS3_CHANNELLIST_H
