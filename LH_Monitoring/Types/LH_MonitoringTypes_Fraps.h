#ifndef LH_FRAPSTYPES_H
#define LH_FRAPSTYPES_H

#ifdef Q_OS_WIN32
# include <windows.h>
#endif

//#ifndef DWORD
//# define DWORD quint32
//#endif

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

#endif // LH_FRAPSTYPES_H
