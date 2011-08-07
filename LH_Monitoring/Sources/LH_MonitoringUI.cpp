#include "LH_MonitoringUI.h"

LH_MonitoringUI::LH_MonitoringUI(LH_QtObject *parent, monitoringDataMode dataMode, bool includeGroups) : QObject(parent)
{
    dataMode_ = dataMode;
    includeGroups_ = includeGroups;

    data_ = NULL;
    mode_ = ui_mode_items;
    initializationState_ = 0;

    int LH_FLAG_SAVEOBJECT_VISIBILITY = LH_FLAG_HIDDEN; //for debugging, set this to LH_FLAG_READONLY

    setup_monitoring_app_ = new LH_Qt_QStringList(parent, "Application", QStringList() << "(Please Select)" << "Aida64" << "ATI Tray Tools" << "Core Temp" << "Fraps" << "GPU-Z" << "HWiNFO" << "HWMonitor + HWMonTray" << "Logitech Monitoring Gadget" << "MSI Afterburner" << "RivaTuner" << "SpeedFan", LH_FLAG_READONLY | LH_FLAG_NOSAVE);
    setup_monitoring_app_->setHelp( "<p>The 3rd party application you are using used to monitor your system.</p>");
    setup_monitoring_app_->setOrder(-4);

    setup_monitoring_app_name_ = new LH_Qt_QString(parent,"(Application Name)", "", LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_monitoring_app_name_->setOrder(-4);

    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Type", QStringList(), LH_FLAG_READONLY | LH_FLAG_NOSAVE);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Type)",0, -1000, 1000, LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_type_index_->setOrder(-4);

    setup_value_type_name_ = new LH_Qt_QString(parent,"(Value Type Name)", "", LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_type_name_->setOrder(-4);

    setup_value_group_ = new LH_Qt_QStringList(parent, "Value Group", QStringList(), LH_FLAG_READONLY | LH_FLAG_NOSAVE);
    setup_value_group_->setHelp( "<p>The value to display.</p>");
    setup_value_group_->setOrder(-4);

    setup_value_group_index_ = new LH_Qt_int(parent,"(Value Group)",0, -1000, 1000, LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_group_index_->setOrder(-4);

    setup_value_group_name_ = new LH_Qt_QString(parent,"(Value Group Name)", "", LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_group_name_->setOrder(-4);

    setup_value_item_ = new LH_Qt_QStringList(parent,"Value Item", QStringList(), LH_FLAG_READONLY | LH_FLAG_NOSAVE);
    setup_value_item_->setHelp( "<p>The value to display.</p>");
    setup_value_item_->setOrder(-4);

    setup_value_item_index_ = new LH_Qt_int(parent,"(Value Item)",0, -1000, 1000, LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_item_index_->setOrder(-4);

    setup_value_item_name_ = new LH_Qt_QString(parent,"(Value Item Name)", "", LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_item_name_->setOrder(-4);

    setup_value_index_ = new LH_Qt_int(parent,"(Value Index)", 0, -1000, 1000, LH_FLAG_SAVEOBJECT_VISIBILITY);
    setup_value_index_->setOrder(-4);

    setup_value_offset_ = new LH_Qt_bool(parent,"Apply Offsets", true, LH_FLAG_HIDDEN);
    setup_value_offset_->setHelp( "<p>Add any user defined offsets to the value (defined within RivaTuner).</p>");
    setup_value_offset_->setOrder(-4);

    setup_value_format_ = new LH_Qt_bool(parent,"Use Formatted Data", false, LH_FLAG_HIDDEN);
    setup_value_format_->setHelp( "<p>Relates to RivaTuner's \"raw data transforming mode\" or Afterburner's \"Formatted Data\".</p><p>(If you don't know what this is, leave it disabled and ignore it.)</p>");
    setup_value_format_->setOrder(-4);

    QTimer::singleShot(1000, this, SLOT(connectChangeEvents()));
}

void LH_MonitoringUI::connectChangeEvents()
{
    setAppSelection();
    if(mode_==ui_mode_index)
        setIndexSelection();
    else
    {
        setTypeSelection();
        setGroupSelection();
        setItemSelection();
    }

    setup_monitoring_app_->setFlag(LH_FLAG_READONLY, false);

    connect(setup_monitoring_app_, SIGNAL(changed()), this, SLOT(changeAppSelection()) );
    connect(setup_value_type_, SIGNAL(changed()), this, SLOT(changeTypeSelection()) );
    connect(setup_value_group_, SIGNAL(changed()), this, SLOT(changeGroupSelection()) );
    connect(setup_value_item_, SIGNAL(changed()), this, SLOT(changeItemSelection()) );

    emit initialized();
}

void LH_MonitoringUI::reset(ui_mode_type mode)
{
    mode_ = mode;
    setVisible(mon_type | mon_group | mon_item, false);
    setReadonly(mon_type | mon_group | mon_item, false);
    clear(mon_type | mon_group | mon_item);
    showOffset(false);
    showFormat(false);
}

void LH_MonitoringUI::showOffset(bool b)
{
    setup_value_offset_->setFlag(LH_FLAG_HIDDEN, !b);
}

void LH_MonitoringUI::showFormat(bool b)
{
    setup_value_format_->setFlag(LH_FLAG_HIDDEN, !b);
}

void LH_MonitoringUI::setVisible(int et, bool b)
{
    if((et & mon_type)==mon_type)
        setup_value_type_->setFlag(LH_FLAG_HIDDEN, !b);
    if((et & mon_group)==mon_group)
        setup_value_group_->setFlag(LH_FLAG_HIDDEN, !b);
    if((et & mon_item)==mon_item)
        setup_value_item_->setFlag(LH_FLAG_HIDDEN, !b);
}

void LH_MonitoringUI::clear(int et)
{
    if((et & mon_type)==mon_type)
        setup_value_type_->list().clear();
    if((et & mon_group)==mon_group)
        setup_value_group_->list().clear();
    if((et & mon_item)==mon_item)
        setup_value_item_->list().clear();
}
void LH_MonitoringUI::append(ui_mon_entry_type et, QString text)
{
    switch(et)
    {
    case mon_type:
        setup_value_type_->list().append(text);
        break;
    case mon_group:
        setup_value_group_->list().append(text);
        break;
    case mon_item:
        setup_value_item_->list().append(text);
        break;
    }
}

void LH_MonitoringUI::insert(ui_mon_entry_type et, int index, QString text)
{
    switch(et)
    {
    case mon_type:
        setup_value_type_->list().insert(index, text);
        break;
    case mon_group:
        setup_value_group_->list().insert(index, text);
        break;
    case mon_item:
        setup_value_item_->list().insert(index, text);
        break;
    }
}

void LH_MonitoringUI::refresh(int et)
{
    if((et & mon_type)==mon_type)
        setup_value_type_->refreshList();
    if((et & mon_group)==mon_group)
        setup_value_group_->refreshList();
    if((et & mon_item)==mon_item)
        setup_value_item_->refreshList();
}

void LH_MonitoringUI::setReadonly(int et, bool b)
{
    if((et & mon_type)==mon_type)
        setup_value_type_->setFlag(LH_FLAG_READONLY, b);
    if((et & mon_group)==mon_group)
        setup_value_group_->setFlag(LH_FLAG_READONLY, b);
    if((et & mon_item)==mon_item)
        setup_value_item_->setFlag(LH_FLAG_READONLY, b);
}

QString LH_MonitoringUI::at(ui_mon_entry_type et, int i)
{
    switch(et)
    {
    case mon_type:
        return setup_value_type_->list().at(i);
        break;
    case mon_group:
        return setup_value_group_->list().at(i);
        break;
    case mon_item:
        return setup_value_item_->list().at(i);
        break;
    }
    return "";
}

int LH_MonitoringUI::value(ui_mon_entry_type et)
{
    switch(et)
    {
    case mon_type:
        return setup_value_type_->value();
        break;
    case mon_group:
        return setup_value_group_->value();
        break;
    case mon_item:
        return setup_value_item_->value();
        break;
    }
    return -1;
}

QString LH_MonitoringUI::valueText(ui_mon_entry_type et)
{
    switch(et)
    {
    case mon_type:
        return setup_value_type_->valueText();
        break;
    case mon_group:
        return setup_value_group_->valueText();
        break;
    case mon_item:
        return setup_value_item_->valueText();
        break;
    }
    return "";
}

int LH_MonitoringUI::count(ui_mon_entry_type et)
{
    switch(et)
    {
    case mon_type:
        return setup_value_type_->list().count();
        break;
    case mon_group:
        return setup_value_group_->list().count();
        break;
    case mon_item:
        return setup_value_item_->list().count();
        break;
    }
    return -1;
}

void LH_MonitoringUI::setValue(ui_mon_entry_type et, int i, bool fix)
{
    switch(et)
    {
    case mon_type:
        if(fix && i>=setup_value_type_->list().length())
            i = setup_value_type_->list().length()-1;
        return setup_value_type_->setValue(i);
        break;
    case mon_group:
        if(fix && i>=setup_value_group_->list().length())
            i = setup_value_group_->list().length()-1;
        return setup_value_group_->setValue(i);
        break;
    case mon_item:
        if(fix && i>=setup_value_item_->list().length())
            i = setup_value_item_->list().length()-1;
        return setup_value_item_->setValue(i);
        break;
    }
}

bool LH_MonitoringUI::applyOffset()
{
    return setup_value_offset_->value();
}

bool LH_MonitoringUI::applyFormat()
{
    return setup_value_format_->value();
}

void LH_MonitoringUI::setTypeSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        if (setup_value_type_->list().length()==0)
            break;
        setup_value_type_->setValue(setup_value_type_index_->value());
        break;
    case ui_mode_index:
        setIndex();
        break;
    case ui_mode_aida64:
        if (((LH_Aida64Data*)data_)->loadXML(true))
        {
            for(int i = 0; i<setup_value_type_->list().length(); i++)
                if(QString(setup_value_type_->list().at(i)).remove(" [Unavailable]") == setup_value_type_name_->value())
                    setup_value_type_->setValue(i);
        } else {
            setup_value_type_->list().clear();
            setup_value_type_->list().append(QString(setup_value_type_name_->value() + " [Unavailable]").trimmed());
            setup_value_type_->refreshList();
            setup_value_type_->setValue(setup_value_type_->list().count()-1);
        }
        break;
    }
}

void LH_MonitoringUI::changeTypeSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        if (setup_value_type_->list().length()!=0)
            setup_value_type_index_->setValue(setup_value_type_->value());
        setup_value_item_->list().clear();
        break;
    case ui_mode_index:
        loadItemsList( setup_value_type_->value() );
        setup_value_index_->setValue( getIndex() );
        break;
    case ui_mode_aida64:
        if (setup_value_type_->list().length()!=0)
        {
            if(setup_value_type_->value()!=-1)
                if(setup_value_type_->value() < setup_value_type_->list().count())
                    setup_value_type_name_->setValue(QString(setup_value_type_->list().at(setup_value_type_->value())).remove(" [Unavailable]"));
            setup_value_group_name_->setValue("");
            setup_value_item_name_->setValue("");
            ((LH_Aida64Data*)data_)->updateLists();
            changeGroupSelection();
        }
        break;
    }
    emit typeChanged();
}

void LH_MonitoringUI::setGroupSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        if (setup_value_group_->list().length()!=0)
            setup_value_group_->setValue(setup_value_group_index_->value());
        break;
    case ui_mode_index:
        //N/A
        break;
    case ui_mode_aida64:
        if (((LH_Aida64Data*)data_)->loadXML(true))
        {
            for(int i = 0; i<setup_value_group_->list().length(); i++)
                if(QString(setup_value_group_->list().at(i)).remove(" [Unavailable]") == setup_value_item_name_->value())
                    setup_value_group_->setValue(i);
        } else
        {
            setup_value_group_->list().clear();
            setup_value_group_->list().append(QString(setup_value_group_name_->value() + " [Unavailable]").trimmed());
            setup_value_group_->refreshList();
            setup_value_group_->setValue(setup_value_group_->list().count()-1);
        }
        break;
    }
}

void LH_MonitoringUI::changeGroupSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        if (setup_value_group_->list().length()!=0)
            setup_value_group_index_->setValue(setup_value_group_->value());
        setup_value_item_->list().clear();
        break;
    case ui_mode_index:
        //N/A
        break;
    case ui_mode_aida64:
        if (setup_value_group_->list().length()!=0)
        {
            if(setup_value_group_->value()!=-1)
                if(setup_value_group_->value() < setup_value_group_->list().count())
                    setup_value_group_name_->setValue(setup_value_group_->list().at(setup_value_group_->value()));
            setup_value_item_name_->setValue("");
            ((LH_Aida64Data*)data_)->updateLists();
            changeItemSelection();
            setup_value_item_->setFlag(LH_FLAG_HIDDEN, (setup_value_item_->list().count()<=1) );
        }
        break;
    }
    emit groupChanged();
}

void LH_MonitoringUI::setItemSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        if (setup_value_item_->list().length()!=0)
            setup_value_item_->setValue(setup_value_item_index_->value());
        break;
    case ui_mode_index:
        break;
    case ui_mode_aida64:
        if (((LH_Aida64Data*)data_)->loadXML(true))
        {
            for(int i = 0; i<setup_value_item_->list().length(); i++)
                if(QString(setup_value_item_->list().at(i)).remove(" [Unavailable]") == setup_value_item_name_->value())
                    setup_value_item_->setValue(i);
        } else
        {
            setup_value_item_->list().clear();
            setup_value_item_->list().append(QString(setup_value_item_name_->value() + " [Unavailable]").trimmed());
            setup_value_item_->refreshList();
            setup_value_item_->setValue(setup_value_item_->list().count()-1);
        }
        break;
    }
}

void LH_MonitoringUI::changeItemSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        if (setup_value_item_->list().length()!=0)
            setup_value_item_index_->setValue(setup_value_item_->value());
        break;
    case ui_mode_index:
        setup_value_index_->setValue(getIndex());
        break;
    case ui_mode_aida64:
        if (setup_value_item_->list().length()!=0)
            if(setup_value_item_->value()!=-1)
                if(setup_value_item_->value()<setup_value_item_->list().count())
                    setup_value_item_name_->setValue(setup_value_item_->list().at(setup_value_item_->value()));
        data_->setIsGroup(setup_value_item_name_->value() == "All");
        break;
    }
    emit itemChanged();
}

void LH_MonitoringUI::setIndexSelection()
{
    switch(mode_)
    {
    case ui_mode_items:
        break;
    case ui_mode_index:
        setIndex();
        break;
    case ui_mode_aida64:
        break;
    }
}

int LH_MonitoringUI::getIndex()
{
    if(mode_!=ui_mode_index) return 0;
    int typeID = setup_value_type_->value();
    int itemID = setup_value_item_->value();
    if(typeID < sensors_.length() && typeID != -1)
        if(itemID < sensors_[typeID].indexes.length() && itemID!=-1)
            return sensors_[typeID].indexes[itemID];
    return 0;
}

void LH_MonitoringUI::setIndex(int index)
{
    if(mode_!=ui_mode_index) return;
    if(index==-1) index = setup_value_index_->value();
    for(int i=0; i<sensors_.count(); i++)
    {
        if(sensors_[i].indexes.contains(index))
        {
            if(setup_value_type_->value()!=i || setup_value_item_->list().count()==0)
            {
                setup_value_type_->setValue(i);
                loadItemsList(i);
            }
            setup_value_item_->setValue(sensors_[i].indexes.indexOf(index));
            return;
        }
    }
    setup_value_type_->setValue(-1);
    setup_value_item_->setValue(-1);
}

void LH_MonitoringUI::loadItemsList(int i)
{
    setup_value_item_->list().clear();
    if(i < sensors_.length() && i >= 0)
        for(int j=0; j<sensors_[i].names.count(); j++)
            setup_value_item_->list().append(sensors_[i].names[j]);
    setup_value_item_->refreshList();

    setup_value_item_->setFlag(LH_FLAG_HIDDEN , (setup_value_item_->list().count()<=1));
}

void LH_MonitoringUI::changeAppSelection()
{
    if (setup_monitoring_app_->list().length()!=0)
    {
        QString val = setup_monitoring_app_->valueText();
        if(val!="") setup_monitoring_app_name_->setValue(val);
    }

    acquireAppData();

    emit appChanged();
}

void LH_MonitoringUI::setAppSelection()
{
    QString val = setup_monitoring_app_name_->value();
    if(val!="") setup_monitoring_app_->setValue(val);
    acquireAppData();
}

void LH_MonitoringUI::acquireAppData()
{
    if(data_)
    {
        delete data_;
        data_ = NULL;
    }
#ifdef LH_MONITORING_LIBRARY
    if(setup_monitoring_app_->valueText() == "MSI Afterburner")
        data_ = new LH_AfterburnerData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "ATI Tray Tools")
        data_ = new LH_ATITrayToolsData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "Core Temp")
        data_ = new LH_CoreTempData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "SpeedFan")
        data_ = new LH_SpeedFanData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "RivaTuner")
        data_ = new LH_RivaTunerData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "GPU-Z")
        data_ = new LH_GPUZData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "Fraps")
        data_ = new LH_FrapsData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "Logitech Monitoring Gadget")
        data_ = new LH_LogitechData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "Aida64")
        data_ = new LH_Aida64Data((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    if(setup_monitoring_app_->valueText() == "HWMonitor + HWMonTray")
        data_ = new LH_HWMonData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
    // if(setup_monitoring_app_->valueText() == "HWiNFO")
        // data_ = new LH_HWiNFOData((LH_QtObject*)parent(), this, dataMode_, includeGroups_);
#elif LH_TORRENTMON_LIBRARY
#endif

    if(!data_) reset();
}
