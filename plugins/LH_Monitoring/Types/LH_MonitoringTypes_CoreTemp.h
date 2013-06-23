#ifndef LH_CORETEMPTYPES_H
#define LH_CORETEMPTYPES_H

#ifdef Q_OS_WIN
# include <windows.h>
#endif

struct CTMemory
{
        unsigned int	uiLoad[256];
        unsigned int	uiTjMax[128];
        unsigned int	uiCoreCnt;
        unsigned int	uiCPUCnt;
        qreal		fTemp[256];
        qreal		fVID;
        qreal		fCPUSpeed;
        qreal		fFSBSpeed;
        qreal		fMultiplier;
        char		sCPUName[100];
        unsigned char	ucFahrenheit;
        unsigned char	ucDeltaToTjMax;
};

#endif // LH_CORETEMPTYPES_H
