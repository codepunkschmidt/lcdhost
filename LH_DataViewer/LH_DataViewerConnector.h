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

#ifndef LH_DataViewerConnector_H
#define LH_DataViewerConnector_H

#include <QDateTime>
#include <QFileSystemWatcher>
#include <QHash>

#include "../LH_Text/LH_Text.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_QTextEdit.h"

// #include <stdio.h>
// #include <windows.h>

#include "LH_DataViewerData.h"

struct thresholdItem
{
    float levelBase;
    QStringList levelNames;
};

struct thresholdList
{
    QList<thresholdItem> levels;
};

class LH_DataViewerConnector : public LH_Text
{
    Q_OBJECT
    QFileSystemWatcher *sourceWatcher_;
    QString watchPath_;
    QList<itemDefinition> itemDefinitions_;
    QHash<QString, thresholdList*> lists_;

    bool repolled_;

    int sourceType_;
    bool isDelimited_;
    int updateLength_;
    char delimiter_;
    int columnWidth_;
    bool isSingleWrite_;
    int completeCount_;
    int dataExpiry_;

    QList<QStringList> parsingList;

    void populateValues(dataNode* node, QStringList sourceLines);
    void updateNodes(QStringList sourceLines);
    QString getTextValue(QStringList lines, itemDefinition def);
    QString formatData(QString data, QString formatting);
    QStringList splitByWidth(QString str, int w);
    QStringList listLanguages();

    void parseAddress(dataNode* currentNode, QStringList addresses, QStringList parseData, QHash<QString,int> indexes);
    dataNode* findNode(QString address, QHash<QString,int> indexes);

protected:
    LH_Qt_QFileInfo *setup_map_file_;
    LH_Qt_QFileInfo *setup_data_file_;
    LH_Qt_QTextEdit *setup_usage_;
    LH_Qt_QStringList *setup_language_;

public:
    LH_DataViewerConnector();
    ~LH_DataViewerConnector();

    int polling();
    QImage *render_qimage( int w, int h );

    static lh_class *classInfo();

public slots:
    void sourceFileChanged();
    void mapFileChanged();
    void sourceFileUpdated(const QString &path);
    void languageFileChanged();
};

#endif // LH_DataViewerConnector_H
