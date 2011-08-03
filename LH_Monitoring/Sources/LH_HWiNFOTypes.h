#ifndef _HWISENSSM_H_INCLUDED_
#define _HWISENSSM_H_INCLUDED_

#define HWiNFO_SENSORS_MAP_FILE_NAME     "HWiNFO_SENS_SM"

#define HWiNFO_MAX_SENSORS         16    // max # of sensor instances
#define HWiNFO_MAX_TEMPERATURES    128   // max # of temp sensor outputs per sensor
#define HWiNFO_MAX_VOLTAGES        32    // max # of volt sensor outputs per sensor
#define HWiNFO_MAX_FANS            16    // max # of fan sensor outputs per sensor
#define HWiNFO_MAX_CURRENTS        16    // max # of current sensor outputs per sensor
#define HWiNFO_MAX_POWERS          16    // max # of power sensor outputs per sensor
#define HWiNFO_MAX_OTHER           32    // max # of other sensor outputs per sensor

#define HWiNFO_SENSORS_STRING_LEN  128

#pragma pack(1)

typedef struct _HWiNFO_SENSORS_READING_LIST {

  BOOL fValid;                              // If this reading has a valid value
  char szLabel[HWiNFO_SENSORS_STRING_LEN];  // e.g. "Chassis2 Fan"
  char szUnit[16];                          // e.g. "RPM"
  double Value;

} HWiNFO_SENSORS_READING_LIST, *PHWiNFO_SENSORS_READING_LIST;

typedef struct _HWiNFO_SENSORS_SENSOR_ENTRY {

  BOOL fValid;          // If this sensor entry is present
  char szSensorName[HWiNFO_SENSORS_STRING_LEN];

  HWiNFO_SENSORS_READING_LIST Temperatures[HWiNFO_MAX_TEMPERATURES];
  HWiNFO_SENSORS_READING_LIST Voltages[HWiNFO_MAX_VOLTAGES];
  HWiNFO_SENSORS_READING_LIST Fans[HWiNFO_MAX_FANS];
  HWiNFO_SENSORS_READING_LIST Currents[HWiNFO_MAX_CURRENTS];
  HWiNFO_SENSORS_READING_LIST Powers[HWiNFO_MAX_POWERS];
  HWiNFO_SENSORS_READING_LIST Others[HWiNFO_MAX_OTHER];

} HWiNFO_SENSORS_SENSOR_ENTRY, *PHWiNFO_SENSORS_SENSOR_ENTRY;

typedef struct _HWiNFO_SENSORS_SHARED_MEM_HEADER {

  DWORD   dwSignature;      // "HWiS"
  DWORD   dwVersion;        // = 1 for current
  DWORD   dwRevision;       // = 1 for current

} HWiNFO_SENSORS_SHARED_MEM_HEADER, *PHWiNFO_SENSORS_SHARED_MEM_HEADER;

typedef struct _HWiNFO_SENSORS_SHARED_MEM {

  HWiNFO_SENSORS_SHARED_MEM_HEADER header;
  __time64_t  poll_time;                                        // last polling time
  HWiNFO_SENSORS_SENSOR_ENTRY Sensors[HWiNFO_MAX_SENSORS];

} HWiNFO_SENSORS_SHARED_MEM, *PHWiNFO_SENSORS_SHARED_MEM;

#pragma pack()

#endif
