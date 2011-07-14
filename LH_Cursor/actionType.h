#ifndef ACTIONTYPE_H
#define ACTIONTYPE_H

#include "../LH_Qt_QString.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_InputState.h"
#include "../LH_Qt_QTextEdit.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_bool.h"

#include "LH_CursorData.h"

#include <QDomElement>

class LH_CursorAction;

enum actionParameterType
{
    aptString,
    aptFile,
    aptInteger
};

enum actionLimitType
{
    altWait,
    altCursorX,
    altCursorY
};

class actionParameter
{
public:
    QString description;
    actionParameterType type;
    QString xmlTag;
    QString xmlAttribute;
    actionLimitType limitType;

    actionParameter( QString d,
                    actionParameterType t = aptString,
                    QString xt = QString(),
                    QString xa = QString(),
                    actionLimitType lt = altWait ) :
        description(d), type(t), xmlTag(xt), xmlAttribute(xa), limitType(lt) {}
};

class actionType
{
    LH_CursorAction *cursorAction_;

    void setIntMinMax(LH_Qt_int *int_, int minVal, int maxVal)
    {
        int val = int_->value();
        int_->setMinMax(minVal,maxVal);
        if(val<minVal)val = minVal;
        if(val>maxVal)val = maxVal;
        int_->setValue(val);
    }

public:
    QString typeCode;
    QString description;
    QList<actionParameter> parameters;

    actionType(
        LH_CursorAction *cursorAction,
        QString typeCode__ = QString(),
        QString description__ = QString(),
        QList<actionParameter> parameters__ = QList<actionParameter>()
        ) :
        cursorAction_(cursorAction),
        typeCode(typeCode__),
        description(description__),
        parameters(parameters__)
    {}

    QString getParameter(QDomElement e, int index);
    QString generateXML(bool enabled, QString desc, QStringList paramValues);
    void displayParameter(int id, LH_Qt_QString *desc_, LH_Qt_QString *str_, LH_Qt_int *int_, LH_Qt_QFileInfo *file_, cursorData cd, QDomElement e = QDomElement());
    QString getParameterValue(int id, LH_Qt_QString *str_, LH_Qt_int *int_, LH_Qt_QFileInfo *file_);
};

#endif // ACTIONTYPE_H
