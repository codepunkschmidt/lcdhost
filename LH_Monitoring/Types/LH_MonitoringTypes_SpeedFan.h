#ifndef LH_SPEEDFANTYPES_H
#define LH_SPEEDFANTYPES_H

#pragma pack(1)
struct SFMemory {
        quint16 version;
        quint16 flags;
        int MemSize;
        int handle;
        quint16 NumTemps;
        quint16 NumFans;
        quint16 NumVolts;
        signed int temps[32];
        signed int fans[32];
        signed int volts[32];
};
#pragma pack()

#endif // LH_SPEEDFANTYPES_H
