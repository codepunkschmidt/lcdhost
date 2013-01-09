#ifndef LH_CORETEMPTYPES_H
#define LH_CORETEMPTYPES_H

#ifdef Q_OS_WIN32
# include <windows.h>
#endif

struct CTMemory
{
        unsigned int	uiLoad[256];
        unsigned int	uiTjMax[128];
        unsigned int	uiCoreCnt;
        unsigned int	uiCPUCnt;
        float		fTemp[256];
        float		fVID;
        float		fCPUSpeed;
        float		fFSBSpeed;
        float		fMultiplier;
        char		sCPUName[100];
        unsigned char	ucFahrenheit;
        unsigned char	ucDeltaToTjMax;
};

#endif // LH_CORETEMPTYPES_H
