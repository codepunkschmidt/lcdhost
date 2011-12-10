#ifndef LH_LOGICBOX_H
#define LH_LOGICBOX_H

#include <LH_QtCFInstance.h>
#include <QTimer>

class LH_LogicBox : public LH_QtCFInstance
{
    Q_OBJECT

protected:
    LH_Qt_QString *setup_input1_;
    LH_Qt_QString *setup_input2_;

    LH_Qt_QString *setup_output_;

public:
    LH_LogicBox();

    const char *userInit();

    static lh_class *classInfo();

public slots:
    void refreshOutput();
};

#endif // LH_LOGICBOX_H
