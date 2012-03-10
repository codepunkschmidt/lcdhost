#ifndef LH_MONITORINGUI_H
#define LH_MONITORINGUI_H

#include <QObject>

#include "LH_Qt_QStringList.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_int.h"
#include "LH_Qt_bool.h"

#include "LH_MonitoringData.h"

#ifdef LH_MONITORING_LIBRARY
#include "LH_AfterburnerData.h"
#include "LH_ATITrayToolsData.h"
#include "LH_CoreTempData.h"
#include "LH_SpeedFanData.h"
#include "LH_RivaTunerData.h"
#include "LH_GPUZData.h"
#include "LH_FrapsData.h"
#include "LH_LogitechData.h"
#include "LH_Aida64Data.h"
#include "LH_HWMonData.h"
#include "LH_HWiNFOData.h"
#elif LH_DRIVESTATS_LIBRARY
#include "LH_DriveStatsData.h"
#endif

enum ui_mon_entry_type
{
    mon_type = 1,
    mon_group = 2,
    mon_item = 4
};

enum ui_mode_type
{
    ui_mode_items,
#ifdef LH_MONITORING_LIBRARY
    ui_mode_aida64,
#endif
    ui_mode_index
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
    bool adaptiveUnits_;

    int initializationState_;

    void acquireAppData();

    void updateUnitOptions();

public:
    LH_MonitoringData *data_;

    LH_Qt_QStringList *setup_monitoring_app_;
    LH_Qt_QString     *setup_monitoring_app_name_;

    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_QStringList *setup_value_group_;
    LH_Qt_QStringList *setup_value_item_;
    LH_Qt_QStringList *setup_unit_selection_;

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

    LH_MonitoringUI(LH_QtObject *parent, monitoringDataMode dataMode, bool includeGroups = false, bool adaptiveUnits = false);

    void reset(ui_mode_type mode = ui_mode_items);
    void setVisible(int, bool = true);
    void showOffset(bool = true);
    void showFormat(bool = true);

    void clear(int);
    void append(ui_mon_entry_type, QString);
    void insert(ui_mon_entry_type et, int index, QString text);
    void refresh(int);
    void setReadonly(int, bool);
    QString at(ui_mon_entry_type, int);
    int value(ui_mon_entry_type);
    QString valueText(ui_mon_entry_type);
    int count(ui_mon_entry_type);
    void setValue(ui_mon_entry_type, int, bool fix=false);

    bool applyOffset();
    bool applyFormat();

    int getIndex();
    void setIndex(int index = -1);

    void loadItemsList(int i);

    void setAppSelection();
    void setTypeSelection();
    void setGroupSelection();
    void setItemSelection();
    void setIndexSelection();

signals:
    void appChanged();
    void typeChanged();
    void groupChanged();
    void itemChanged();
    void initialized();

protected slots:
    void connectChangeEvents();

public slots:
    void changeAppSelection();
    void changeTypeSelection();
    void changeGroupSelection();
    void changeItemSelection();
};

#endif // LH_MONITORINGUI_H
