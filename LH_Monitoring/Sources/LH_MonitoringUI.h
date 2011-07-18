#ifndef LH_MONITORINGUI_H
#define LH_MONITORINGUI_H

#include <QObject>

#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_bool.h"

#include "LH_MonitoringData.h"

#include "LH_AfterburnerData.h"
#include "LH_ATITrayToolsData.h"
#include "LH_CoreTempData.h"
#include "LH_SpeedFanData.h"
#include "LH_RivaTunerData.h"
#include "LH_GPUZData.h"
#include "LH_FrapsData.h"
#include "LH_LogitechData.h"
#include "LH_Aida64Data.h"

enum ui_mon_entry_type
{
    mon_type = 1,
    mon_group = 2,
    mon_item = 4
};

enum ui_mode_type
{
    ui_mode_items,
    ui_mode_index,
    ui_mode_aida64
};

struct sensorGroup
{
    QStringList names;
    QList<int> indexes;
};

class LH_MonitoringUI : public QObject
{
    Q_OBJECT

    ui_mode_type mode_;
    monitoringDataMode dataMode_;
    bool includeGroups_;

    void acquireAppData();

public:
    LH_MonitoringData *data_;

    LH_Qt_QStringList *setup_monitoring_app_;
    LH_Qt_QString     *setup_monitoring_app_name_;

    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_QStringList *setup_value_group_;
    LH_Qt_QStringList *setup_value_item_;

    LH_Qt_int         *setup_value_type_index_;
    LH_Qt_int         *setup_value_item_index_;
    LH_Qt_int         *setup_value_group_index_;

    LH_Qt_int         *setup_value_index_;

    LH_Qt_QString     *setup_value_type_name_;
    LH_Qt_QString     *setup_value_group_name_;
    LH_Qt_QString     *setup_value_item_name_;

    LH_Qt_bool *setup_value_offset_;
    LH_Qt_bool *setup_value_format_;

    QList<sensorGroup> sensors_;

    LH_MonitoringUI(LH_QtObject *parent, monitoringDataMode dataMode, bool includeGroups = false);

    void reset(ui_mode_type mode = ui_mode_items);
    void setVisible(int, bool = true);
    void showOffset(bool = true);
    void showFormat(bool = true);

    void clear(int);
    void append(ui_mon_entry_type, QString);
    void refresh(int);
    void setReadonly(int, bool);
    QString at(ui_mon_entry_type, int);
    int value(ui_mon_entry_type);
    QString valueText(ui_mon_entry_type);
    int count(ui_mon_entry_type);
    void setValue(ui_mon_entry_type, int);

    bool applyOffset();
    bool applyFormat();

    int getIndex();
    void setIndex(int index = -1);

    void loadItemsList(int i);

signals:
    void appSet();
    void appChanged();
    void typeSet();
    void typeChanged();
    void groupSet();
    void groupChanged();
    void itemSet();
    void itemChanged();
    void indexSet();

public slots:
    void setAppSelection();
    void changeAppSelection();
    void setTypeSelection();
    void changeTypeSelection();
    void setGroupSelection();
    void changeGroupSelection();
    void setItemSelection();
    void changeItemSelection();
    void setIndexSelection();
};

#endif // LH_MONITORINGUI_H
