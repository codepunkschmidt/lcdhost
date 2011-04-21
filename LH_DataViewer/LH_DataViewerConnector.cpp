/**
  \file     LH_DataViewerConnector.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#include "LH_DataViewerConnector.h"
#include <QDebug>
#include <QPainter>
#include <QStringList>
#include <cmath>

LH_PLUGIN_CLASS(LH_DataViewerConnector)

lh_class *LH_DataViewerConnector::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Dynamic/DataViewer",
        "DataViewerConnector",
        "Data Connector",
        -1, -1,
        lh_instance_calltable_NULL
    };

    if( classInfo.width == -1 )
    {
        QFont arial10("Arial",10);
        QFontMetrics fm( arial10 );
        classInfo.height = fm.height();
        classInfo.width = classInfo.height * 4;
    }

    return &classInfo;
}

#define source_type_XML 1
#define source_type_TXT 2
#define source_type_INI 3


LH_DataViewerConnector::LH_DataViewerConnector(const char* name) : LH_Text(name)
{
    // Hide inherited attributes we don't use
    setup_text_->setFlags( LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE);
    setText(" ");
    setup_horizontal_->setFlag( LH_FLAG_HIDDEN, true );
    setup_vertical_->setFlag( LH_FLAG_HIDDEN, true );
    setup_scrollrate_->setFlag( LH_FLAG_HIDDEN, true );
    setup_scrollstep_->setFlag( LH_FLAG_HIDDEN, true );
    setup_bgcolor_->setFlag( LH_FLAG_HIDDEN, true );

    setup_font_->setFlag( LH_FLAG_HIDDEN, true );
    setup_fontresize_->setFlag( LH_FLAG_HIDDEN, true );
    setup_pencolor_->setFlag( LH_FLAG_HIDDEN, true );

    QStringList langs = listLanguages();
    qDebug() << "languages: " << langs.count() << ": " << langs.join(",");
    setup_language_ = new LH_Qt_QStringList(this,"Language",langs, LH_FLAG_AUTORENDER);
    setup_language_->setFlag(LH_FLAG_HIDDEN, langs.length()<=1);
    connect( setup_language_, SIGNAL(changed()), this, SLOT(languageFileChanged()) );

    setup_map_file_ = new LH_Qt_QFileInfo(this,"Data Map",QFileInfo(), LH_FLAG_AUTORENDER );
    connect( setup_map_file_, SIGNAL(changed()), this, SLOT(mapFileChanged()) );

    setup_data_file_ = new LH_Qt_QFileInfo(this,"Data Source",QFileInfo(), LH_FLAG_AUTORENDER );
    connect( setup_data_file_, SIGNAL(changed()), this, SLOT(sourceFileChanged()) );


    watchPath_ = "";
    sourceWatcher_ = new QFileSystemWatcher(this);
    connect( sourceWatcher_, SIGNAL(fileChanged(const QString)), this, SLOT(sourceFileUpdated(const QString)) );

    rootNode = new dataNode();
    sharedData = new sharedCollection();

    repolled_ = false;
}

QStringList LH_DataViewerConnector::listLanguages()
{
    QStringList languages = QStringList();
    languages.append("Default");
qDebug() << state()->dir_layout;
    QDir layoutDir = QDir(state()->dir_layout);
    QStringList filters;
    filters << "lists.*.txt";
    layoutDir.setNameFilters(filters);
    QFileInfoList langFiles = layoutDir.entryInfoList(QDir::Files);

    QRegExp rxList("lists\\.(.*)\\.txt");
    foreach(QFileInfo f, langFiles)
    {
        QString fileName = f.fileName();
        fileName = fileName.replace(rxList,"\\1");
        languages.append(fileName);
    }
    return languages;
}

LH_DataViewerConnector::~LH_DataViewerConnector()
{
    //delete sourceWatcher_;
    delete rootNode;
    rootNode = 0;
    delete sharedData;
    sharedData = 0;
    return ;
}

int LH_DataViewerConnector::polling()
{
    //repoll the data automatically once to facilitate loading the data
    //when a layout is first loaded.
    if(!repolled_)
    {
        repolled_ = true;
        return 100;
    } else {
        //second call happens after the initial delay. fake the update to refresh.
        sourceFileUpdated("");
        return 0;
    }
}

QImage *LH_DataViewerConnector::render_qimage( int w, int h )
{
    if( !prepareForRender(w,h) ) return NULL;

    QPainter painter;
    if( painter.begin(image_) )
    {
        QRectF target;

        target.setSize( textimage().size() );
        target.moveLeft( image_->width()/2 - textimage().width()/2 );
        target.moveTop( image_->height()/2 - textimage().height()/2 );

        if( textimage().width() > image_->width() )
            target.moveLeft( 0 );

        painter.drawImage( target, textimage(), textimage().rect() );
        painter.end();
    }

    return image_;
}

void LH_DataViewerConnector::populateValues(dataNode* node, QStringList sourceLines)
{
    foreach(QString key, node->keys())
    {
        foreach(dataNode* childNode, node->child(key))
        {
            if(childNode->hasData())
                childNode->setValue( getTextValue(sourceLines, childNode->definition()) );
            if(childNode->hasChildren())
                populateValues(childNode, sourceLines);
        }
    }
}

QString LH_DataViewerConnector::getTextValue(QStringList lines, itemDefinition def)
{
    if (lines.count()<=def.y)
        return "";
    else {
        QString dataLine = lines[def.y];
        QStringList dataItems;
        if (isDelimited_)
            dataItems = dataLine.split(delimiter_);
        else
            dataItems = splitByWidth(dataLine, columnWidth_);

        if (dataItems.count()<=def.x)
            return "";
        else
            return formatData(dataItems[def.x].trimmed(), def.formatting);
    }
}

QString LH_DataViewerConnector::formatData(QString data, QString formatting)
{
    QString result = data;
    QRegExp rxList("list:(.*),(.*)");
    if (formatting!="")
    {
        if (formatting.startsWith("rx:"))
        {
            QRegExp rx = QRegExp(formatting.mid(3));
            if(rx.numCaptures()!=0)
                result = result.replace(rx,"\\1").trimmed();
            else
                result = result.remove(rx).trimmed();
        } else
        if (rxList.indexIn(formatting)!=-1)
        {
            QString segmentName = rxList.cap(1).toLower();
            int listIndex    = rxList.cap(2).toInt();
            float levelVal   = result.toFloat();

            if(!lists_.contains(segmentName))
                qWarning()<<"List " << segmentName << " not found!";
            else
            {
                thresholdList* list = lists_.value(segmentName);
                //qDebug()<<"List " << segmentName << " : " << list->levels.count();
                for(int i=0; i<list->levels.count(); i++)
                {
                    //qDebug() << levelVal << " vs " <<  list->levels.at(i).levelBase << " (" << list->levels.at(i).levelNames.at(listIndex).trimmed() << ")";
                    if(levelVal < list->levels.at(i).levelBase) break;
                    if(listIndex<list->levels.at(i).levelNames.count())
                        result = list->levels.at(i).levelNames.at(listIndex).trimmed();
                    else
                        result = "ERROR: Invalid list column!";
                }
            }
        } else
        if (formatting == "seconds>time")
        {
            int s = result.toInt() ;
            int d = floor(s / (24 *60 *60));
            QTime tm = QTime::fromString("00:00:00.000", "HH:mm:ss.zzz");
            result = QString("%1 day%2 %3").arg(d).arg(d!=1? "s" : "").arg(tm.addSecs(s%(24 *60 *60)).toString("HH:mm:ss"));
        }
    }
    return result;
}

QStringList LH_DataViewerConnector::splitByWidth(QString str, int w)
{
    QStringList list;
    while (str!="")
    {
        if(str.length()>w)
        {
            list.append(str.left(w));
            str = str.right(str.length()-w);
        } else {
            list.append(str);
            str = "";
            break;
        }
    }
    return list;
}

void LH_DataViewerConnector::sourceFileChanged()
{
    if(watchPath_!="") sourceWatcher_->removePath(watchPath_);
    if( !setup_data_file_->value().isFile() )
    {
        watchPath_ = "";
        setup_text_->setValue(tr("No such source file."));
        setup_text_->setFlag(LH_FLAG_HIDDEN,false);
    }
    else
    {
        setup_text_->setValue(tr("Connected to Data"));
        setup_text_->setFlag(LH_FLAG_HIDDEN,true);
        watchPath_ = setup_data_file_->value().absoluteFilePath();
        //qDebug() << "watching source file: " << watchPath_;
        sourceWatcher_->addPath(watchPath_);
        sourceFileUpdated(watchPath_);
    }
}

void LH_DataViewerConnector::mapFileChanged()
{
    setup_map_file_->value().refresh();
    if( !setup_map_file_->value().isFile() )
    {
        setup_text_->setValue(tr("No such map file."));
        setup_text_->setFlag(LH_FLAG_HIDDEN,false);
        return;
    }
    else
    {
        setup_text_->setValue(tr("Connected to Data"));
        setup_text_->setFlag(LH_FLAG_HIDDEN,true);
        QFile file( setup_map_file_->value().filePath() );

        if( file.open( QIODevice::ReadOnly) )
        {
            dataNode *currentNode = rootNode;
            QTextStream stream(&file);
            QRegExp rx = QRegExp(";.*$");
            QString fileContent = stream.readAll();

            QStringList items = fileContent.split('\r',QString::SkipEmptyParts);

            itemDefinitions_.clear();
            lists_.clear();
            QString segmentName="";
            QString segment = "";
            parsingList.clear();
            foreach (QString item, items)
            {
                QString removedPart = item.right(item.length() - item.indexOf(rx)).trimmed();
                item = item.remove(rx).trimmed();
                if (item!="")
                {
                    if(item.toLower()=="[[end]]")
                        break;
                    else
                    if(item.startsWith("["))
                    {
                        segment = item.toLower();
                        if(segment.startsWith("[list:"))
                        {
                            segmentName = QString(segment).replace(QRegExp("\\[list:(.*)\\]") , "\\1" );
                            if (!lists_.contains(segmentName)) lists_.insert(segmentName, new thresholdList() );
                        }
                        if(segment.startsWith("[definition:"))
                        {
                            segmentName = QString(segment).replace(QRegExp("\\[definition:(.*)\\]") , "\\1" );
                            currentNode = currentNode->addChild(segmentName);
                        }
                        if(segment.startsWith("[/definition:"))
                        {
                            segmentName = QString(segment).replace(QRegExp("\\[definition:(.*)\\]") , "\\1" );
                            currentNode = currentNode->parentNode();
                            segment = QString("[definition:%1]").arg(currentNode->name());
                        }
                    } else
                    if(segment=="[format]")
                    {
                        QString property = item.section('=',0,0).trimmed().toLower();
                        QString value = item.section('=',1,-1).trimmed();
                        if(property=="type")
                            sourceType_ = (value.toLower()=="xml"? source_type_XML : (value.toLower()=="ini"? source_type_INI : source_type_TXT)); else
                        if(property=="delimited")                        
                            isDelimited_ = (value.toLower()=="true" || value.toLower()=="yes" || value.toLower()=="1"); else
                        if(property=="delimiter")
                        {
                            value += removedPart;
                            if (value.at(0) == '\\') {
                                if (value.at(1) == 't') delimiter_= '\t'; else
                                if (value.at(1) == ';') delimiter_= ';'; else
                                if (value.at(1) == '\\') delimiter_= '\\'; else
                                    delimiter_ = value.at(1).toLatin1();
                            } else
                                delimiter_ = value.at(0).toLatin1();
                        } else
                        if(property=="width")
                            columnWidth_ = value.toInt(); else
                        if(property=="singlewrite")
                            isSingleWrite_ = (value.toLower()=="true" || value.toLower()=="yes" || value.toLower()=="1"); else
                        if(property=="completeat")
                            completeCount_ = value.toInt();
                        if(property=="dataexpiry")
                            dataExpiry_ = value.toInt();
                        if(property=="language")
                            {setup_language_->list()[0] = QString("%1 (Default)").arg(value); setup_language_->refreshList();}
                    } else
                    if(segment.startsWith("[list:"))
                    {
                        thresholdItem newLevel;
                        //qDebug() << "Add list entry : " << item;
                        newLevel.levelNames = item.split('\t',QString::SkipEmptyParts);
                        newLevel.levelBase = newLevel.levelNames.at(0).trimmed().toFloat();
                        lists_.value(segmentName)->levels.append(newLevel);
                    } else
                    if(segment.startsWith("[definition:"))
                    {
                        QStringList parts = item.split('\t',QString::SkipEmptyParts);
                        if(parts.count()>=2 && sourceType_ == source_type_TXT)
                        {
                            QRegExp rx("^\"(.*)\"$");
                            if (rx.indexIn(parts.at(1).trimmed()) != -1)
                                currentNode->addChild( parts.at(0).trimmed(), rx.cap(1) );
                            else
                                currentNode->addChild( (itemDefinition){
                                    parts.at(0).trimmed(),
                                    parts.at(1).trimmed(),
                                    parts.at(1).trimmed().section(',',0,0).toInt(),
                                    parts.at(1).trimmed().section(',',1,1).toInt(),
                                    (parts.count()<3? "" : parts.at(2)),
                                    true
                                });
                        }
                    } else
                    if(segment.startsWith("[/definition:"))
                    {
                        Q_ASSERT(false);
                    } else
                    if(segment=="[definitions]")
                    {
                        QStringList parts = item.split('\t',QString::SkipEmptyParts);
                        if(parts.count()>=2)
                        {
                            itemDefinition def;
                            def.name = parts.at(0).trimmed();
                            def.address = parts.at(1).trimmed();
                            if(sourceType_ == source_type_TXT)
                            {
                                def.x = def.address.section(',',0,0).toInt();
                                def.y = def.address.section(',',1,1).toInt();
                            }
                            if(parts.count()>=3)
                                def.formatting = parts.at(2);
                            else
                                def.formatting = "";

                            itemDefinitions_.append(def);
                        }
                    } else
                    if(segment=="[parsing]") {
                        //rules for reinterpreting parsed data loaded here
                        QStringList parts = item.split('\t',QString::SkipEmptyParts);
                        if(parts.count()>=2)
                            parsingList.append(parts);
                    } else
                    {
                        qWarning() << QString("Line in unknown segment (%1) unparsed: ").arg(segment) << item;
                    }
                }
            }
            languageFileChanged();
        } else {
            setup_text_->setValue(tr("Unable to open file."));
            setup_text_->setFlag(LH_FLAG_HIDDEN,false);
            return;
        }
    }
}

void LH_DataViewerConnector::sourceFileUpdated(const QString &path)
{
    Q_UNUSED(path);


    if( watchPath_ != "" )
    {
        setup_text_->setValue(tr("Connected to Data"));
        setup_text_->setFlag(LH_FLAG_HIDDEN,true);

        QFile file( watchPath_ );
        QString fileContent = "";
        if( file.open( QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream.setAutoDetectUnicode(true);
            fileContent = stream.readAll();
            file.close();
        }
        switch(sourceType_){
        case source_type_TXT:
        case source_type_INI:
            {
                QStringList sourceLines = fileContent.split('\r',QString::SkipEmptyParts);

                if (isSingleWrite_ || sourceLines.count()>=completeCount_)
                {
                    //clear memory
                    sharedData->clear(itemDefinitions_.count());

                    // set expiry
                    if(dataExpiry_!=0)
                        sharedData->expiresAt =  QFileInfo(file).lastModified().addSecs(dataExpiry_).toString("yyyyMMddHHmmss.zzz");
                    sharedData->lastUpdated = QDateTime::currentDateTime().toString("yyyyMMddHHmmss.zzz");

                    // load defined data
                    switch(sourceType_){
                    case source_type_TXT:
                        //fill any index-based fields
                        for (int i=0; i<itemDefinitions_.count(); i++)
                            sharedData->setItem(i, itemDefinitions_.at(i).name, getTextValue(sourceLines, itemDefinitions_.at(i)));
                        //fill any node-based fields
                        populateValues(rootNode, sourceLines);
                        break;
                    case source_type_INI:
                        updateNodes(sourceLines);
                        break;
                    }

                    // declare valid
                    sharedData->valid = true;
                }
            }
            break;
        default:
            {
                setup_text_->setValue("Data file type not yet supported");
                setup_text_->setFlag(LH_FLAG_HIDDEN,false);
            }
        }
    }
}

void LH_DataViewerConnector::updateNodes(QStringList sourceLines)
{
    dataNode* currentNode = rootNode;
    currentNode->resetCursors();
    QRegExp rx = QRegExp(";.*$");
    for(int i=0; i<sourceLines.count(); i++)
    {
        //QString removedPart = line.right(line.length() - line.indexOf(rx)).trimmed();
        QString line = sourceLines[i].trimmed();
        line = line.remove(rx).trimmed();
        if(line=="")
            continue;
        QStringList parts = line.split(delimiter_,QString::SkipEmptyParts);

        QString nodeName = parts.at(0).trimmed();
        parts.removeFirst();
        QString nodeValue = parts.join(QString(delimiter_)).trimmed();

        if(line.startsWith("[/"))
           currentNode = currentNode->parentNode();
        else
        if(line.startsWith("["))
           currentNode = currentNode->openChild(line.replace(QRegExp("\\[(.*)\\]") , "\\1" ));
        else
           currentNode->openChild( nodeName,  nodeValue );
    }

    for(int i=0; i<parsingList.count(); i++)
    {
        parseAddress(rootNode, parsingList[i][0].trimmed().split('.',QString::SkipEmptyParts), parsingList[i], QHash<QString,int>() );
    }
    //Q_Assert(false); //need to apply parsing rules
}

void LH_DataViewerConnector::parseAddress(dataNode* currentNode, QStringList addresses, QStringList parseData, QHash<QString,int> indexes)
{
    if(addresses.count()>1)
    {
        QStringList myAddresses = QStringList(addresses);
        //continue drilling down the node heirarchy
        QString nodeName = myAddresses[0];
        myAddresses.removeFirst();
        int nodeIndex = -1;
        QRegExp rx("^(.*)\\[([0-9]+)\\]$");
        if(rx.indexIn(nodeName) != -1)
        {
            nodeName = rx.cap(1);
            nodeIndex = rx.cap(2).toInt();
        }

        //You can only create one new layer of nodes here
        Q_ASSERT(currentNode->contains(nodeName));

        QList<dataNode*> nodesList = currentNode->child(nodeName);
        if(nodeIndex==-1 && nodesList.count()==1) nodeIndex = 0;
        if(nodeIndex==-1)
        {
            for (nodeIndex=0; nodeIndex<nodesList.count(); nodeIndex++)
            {
                QHash<QString,int> myIndexes = QHash<QString,int>(indexes);
                myIndexes.insert(nodesList[nodeIndex]->address(), nodeIndex);
                parseAddress(nodesList[nodeIndex], myAddresses, parseData, myIndexes);
            }
        } else
        if(nodeIndex < nodesList.count() && nodeIndex >= 0)
        {
            QHash<QString,int> myIndexes = QHash<QString,int>(indexes);
            myIndexes.insert(nodesList[nodeIndex]->address(), nodeIndex);
            parseAddress(nodesList[nodeIndex], myAddresses, parseData, myIndexes);
        }
    } else {
        //apply parsing
        dataNode* valueNode = findNode(parseData[1].trimmed(), indexes);
        QString formatCode = (parseData.count()<3? "" : parseData[2].trimmed());

        //now parse the format code as a template using stored indexes
        QRegExp rx = QRegExp("\\{([a-zA-Z0-9.[\\]]*)\\}");
        QStringList matches;
        QString tempFormatCode = formatCode;
        while (rx.indexIn(tempFormatCode) != -1)
        {
            if(!matches.contains(rx.cap(1)))
                matches.append(rx.cap(1));
            tempFormatCode.replace(rx.cap(0), "");
        }
        foreach(QString match, matches)
        {
            dataNode* matchNode = findNode(match, indexes);
            if(matchNode == 0)
                continue;
            else
                formatCode.replace(QString("{%1}").arg(match), matchNode->value());
        }

        QString nodeValue = formatData(valueNode->value(), formatCode);
        //create the new child node
        if(valueNode->address() == currentNode->address(addresses[0]))
            valueNode->setValue(nodeValue);
        else
            currentNode->openChild(addresses[0], nodeValue);
    }
}

dataNode* LH_DataViewerConnector::findNode(QString address, QHash<QString,int> indexes)
{
    QStringList path = address.split(".");
    dataNode* curNode = rootNode;
    QRegExp rx("^(.*)\\[([0-9]+)\\]$");
    while (path.length()!=0)
    {
        QString nodeName = path.first();
        path.removeFirst();

        int nodeIndex = -1;
        if(rx.indexIn(nodeName) != -1)
        {
            nodeName = rx.cap(1);
            nodeIndex = rx.cap(2).toInt();
        }

        Q_ASSERT(curNode->contains(nodeName));

        QList<dataNode*> nodesList = curNode->child(nodeName);
        if(nodeIndex==-1 && nodesList.count()==1) nodeIndex = 0;
        if(nodeIndex==-1)
            if(indexes.contains(curNode->address(nodeName)))
                nodeIndex = indexes[curNode->address(nodeName)];

        Q_ASSERT(nodeIndex>=0);
        Q_ASSERT(nodeIndex<curNode->child(nodeName).count());

        curNode = curNode->child(nodeName)[nodeIndex];
    }
    return curNode;
}

void LH_DataViewerConnector::languageFileChanged()
{
    QString fileName;
    if(setup_language_->value()>0 && setup_language_->value() < setup_language_->list().count())
        fileName = QString("%1lists.%2.txt").arg(state()->dir_layout).arg(setup_language_->list().at(setup_language_->value()));
    else
        fileName = setup_map_file_->value().absoluteFilePath();

    if(QFileInfo(fileName).exists())
    {
        QFile file( fileName );
        if( file.open( QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            QRegExp rx = QRegExp(";.*$");
            QString fileContent = stream.readAll();

            QStringList items = fileContent.split('\r',QString::SkipEmptyParts);

            QString segmentName="";
            QString segment = "";
            foreach (QString item, items)
            {
                item = item.remove(rx).trimmed();
                if (item!="")
                {
                    if(item.startsWith("["))
                    {
                        segment = item.toLower();
                        if(segment.startsWith("[list:"))
                        {
                            segmentName = QString(segment).replace(QRegExp("\\[list:(.*)\\]") , "\\1" );
                            if (lists_.contains(segmentName))
                            {
                                lists_.remove(segmentName);
                                lists_.insert(segmentName, new thresholdList() );
                            } else
                                segment = ""; //list does not exist
                        }
                    } else
                    if(segment.startsWith("[list:"))
                    {
                        thresholdItem newLevel;
                        newLevel.levelNames = item.split('\t',QString::SkipEmptyParts);
                        newLevel.levelBase = newLevel.levelNames.at(0).trimmed().toFloat();
                        lists_.value(segmentName)->levels.append(newLevel);
                    }
                }
            }

            sourceFileUpdated(setup_data_file_->value().absoluteFilePath());
        }
    }
}

