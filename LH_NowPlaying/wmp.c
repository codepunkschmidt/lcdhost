#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif

#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

// A lot of these guids can be found at:
//        http://mheg2xmltv.googlecode.com/svn/trunk/dcdvbsource/Source/Filter/WMPPlugin/WMPPluginAPI.pas

MIDL_DEFINE_GUID(IID, IID_IWMPErrorItem,           0x3614C646,0x3B3B,0x4DE7,0xA8,0x1E,0x93,0x0E,0x3F,0x21,0x27,0xB3);
MIDL_DEFINE_GUID(IID, IID_IWMPError,               0xA12DCF7D,0x14AB,0x4C1B,0xA8,0xCD,0x63,0x90,0x9F,0x06,0x02,0x5B);
MIDL_DEFINE_GUID(IID, IID_IWMPMedia,               0x94D55E95,0x3FAC,0x11D3,0xB1,0x55,0x00,0xC0,0x4F,0x79,0xFA,0xA6);
MIDL_DEFINE_GUID(IID, IID_IWMPControls,            0x74C09E02,0xF828,0x11D2,0xA7,0x4B,0x00,0xA0,0xC9,0x05,0xF3,0x6E);
MIDL_DEFINE_GUID(IID, IID_IWMPSettings,            0x9104D1AB,0x80C9,0x4FED,0xAB,0xF0,0x2E,0x64,0x17,0xA6,0xDF,0x14);
MIDL_DEFINE_GUID(IID, IID_IWMPClosedCaption,       0x350ba78b,0x6bc8,0x4113,0xa5,0xf5,0x31,0x20,0x56,0x93,0x4e,0xb6);
//IID_IWMPPlaylist
//IID_IWMPCdrom
//IID_IWMPCdromCollection
//IID_IWMPStringCollection
//IID_IWMPMediaCollection
//IID_IWMPPlaylistArray
//IID_IWMPPlaylistCollection
MIDL_DEFINE_GUID(IID, IID_IWMPNetwork,             0xEC21B779,0xEDEF,0x462D,0xBB,0xA4,0xAD,0x9D,0xDE,0x2B,0x29,0xA7);
MIDL_DEFINE_GUID(IID, IID_IWMPCore,                0xD84CCA99,0xCCE2,0x11D2,0x9E,0xCC,0x00,0x00,0xF8,0x08,0x59,0x81);
MIDL_DEFINE_GUID(IID, IID_IWMPPlayer,              0x6BF52A4F,0x394A,0x11D3,0xB1,0x53,0x00,0xC0,0x4F,0x79,0xFA,0xA6);
MIDL_DEFINE_GUID(IID, IID_IWMPPlayer2,             0x0E6B01D1,0xD407,0x4C85,0xBF,0x5F,0x1C,0x01,0xF6,0x15,0x02,0x80);
//IID_IWMPMedia2
//IID_IWMPControls2
//IID_IWMPDVD
MIDL_DEFINE_GUID(IID, IID_IWMPCore2,               0xBC17E5B7,0x7561,0x4C18,0xBB,0x90,0x17,0xD4,0x85,0x77,0x56,0x59);
//IID_IWMPPlayer3
//IID_IWMPErrorItem2
MIDL_DEFINE_GUID(IID, IID_IWMPRemoteMediaServices, 0xCBB92747,0x741F,0x44fe,0xAB,0x5B,0xF1,0xA4,0x8F,0x3B,0x2A,0x59 );
MIDL_DEFINE_GUID(CLSID, CLSID_WMPSkinManager,      0xB2A7FD52,0x301F,0x4348,0xB9,0x3A,0x63,0x8C,0x6D,0xE4,0x92,0x29 );
//IID_IWMPSkinManager
//IID_IWMPMetadataPicture
//IID_IWMPMetadataText
//IID_IWMPMedia3
//IID_IWMPSettings2
//IID_IWMPControls3
//IID_IWMPClosedCaption2
MIDL_DEFINE_GUID(IID, IID_IWMPPlayerApplication,   0x40897764,0xCEAB,0x47BE,0xAD,0x4A,0x8E,0x28,0x53,0x7F,0x9B,0xBF);
MIDL_DEFINE_GUID(IID, IID_IWMPCore3,               0x7587C667,0x628F,0x499F,0x88,0xE7,0x6A,0x6F,0x4E,0x88,0x84,0x64);
MIDL_DEFINE_GUID(IID, IID_IWMPPlayer4,             0x6C497D62,0x8919,0x413c,0x82,0xDB,0xE9,0x35,0xFB,0x3E,0xC5,0x84);
//IID_IWMPPlayerServices
//IID_IWMPSyncDevice
//IID_IWMPSyncServices
//IID_IWMPPlayerServices2
//IID_IWMPCdromRip
//IID_IWMPCdromBurn
//IID_IWMPQuery
//IID_IWMPMediaCollection2
//IID_IWMPStringCollection2
//IID_IWMPLibrary
//IID_IWMPLibraryServices
//IID_IWMPLibrarySharingServices
//IID_IWMPFolderMonitorServices
//IID_IWMPSyncDevice2
//IID_IWMPSyncDevice3
//IID_IWMPLibrary2
//LIBID_WMPLib
//IID_IWMPEvents
//IID_IWMPEvents2
//IID_IWMPEvents3
//IID_IWMPEvents4
//DIID__WMPOCXEvents
MIDL_DEFINE_GUID(CLSID, CLSID_WindowsMediaPlayer,  0x6BF52A52,0x394A,0x11D3,0xB1,0x53,0x00,0xC0,0x4F,0x79,0xFA,0xA6);
//MIDL_DEFINE_GUID(CLSID, CLSID_WindowsMediaPlayer,  0x6BF52A50,0x394A,0x11D3,0xB1,0x53,0x00,0xC0,0x4F,0x79,0xFA,0xA6); //Alterntive CLSID?






#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif
