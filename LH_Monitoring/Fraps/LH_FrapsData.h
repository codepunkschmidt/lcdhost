#ifndef LH_FRAPSDATA_H
#define LH_FRAPSDATA_H

#include "../_Templates/LH_MonitoringData.h"

#ifdef Q_WS_WIN
# include <windows.h>
#endif

#ifndef DWORD
# define DWORD quint32
#endif

#ifndef WINAPI
# define WINAPI
#endif

struct FRAPS_SHARED_DATA {
   DWORD sizeOfStruct;
   DWORD currentFPS;
   DWORD totalFrames;
   DWORD timeOfLastFrame;
   char gameName[32];
};

class LH_FrapsData: public LH_MonitoringData
{
    void pingFraps();
protected:
    LH_Qt_QString *setup_FPS_;
    bool getData(float& value, QString& text, QString& units);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_int *setup_value_type_index_;

    LH_FrapsData( LH_QtObject *parent, monitoringDataMode dataMode = mdmNumbers);
    ~LH_FrapsData();

    QString appName() { return "Fraps"; }
    monitoringDataType getType();

    void setTypeSelection();
    void changeTypeSelection();
};

#endif // LH_FRAPSDATA_H
