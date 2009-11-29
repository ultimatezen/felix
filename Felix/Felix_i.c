

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Sat Nov 14 22:33:58 2009
 */
/* Compiler settings for Felix.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

MIDL_DEFINE_GUID(IID, IID_IRecord,0x6195F62F,0xB419,0x4C98,0xB1,0x68,0xBD,0xA7,0x36,0xAD,0xC4,0x07);


MIDL_DEFINE_GUID(IID, IID_ISearchResult,0x61647BA5,0x8AEA,0x487B,0x97,0xB6,0x44,0x2C,0xA9,0xD0,0xC4,0x33);


MIDL_DEFINE_GUID(IID, IID_IApp2,0xCC9981F5,0x4D27,0x4F01,0xA7,0x89,0x40,0xFA,0x4D,0xFE,0x53,0x99);


MIDL_DEFINE_GUID(IID, IID_IApp,0xC9063D7F,0x5F1F,0x4155,0x96,0x66,0xC3,0x49,0x03,0x0F,0xC4,0xB3);


MIDL_DEFINE_GUID(IID, IID_IFelixMemDocUIHandler,0x13AA9D46,0xD1BD,0x4531,0x95,0xF7,0x00,0x35,0x7E,0xB3,0x1D,0x8D);


MIDL_DEFINE_GUID(IID, LIBID_Felix,0x47D0B52B,0x7365,0x4A47,0x82,0x87,0xF5,0x41,0xC1,0x47,0x07,0xA5);


MIDL_DEFINE_GUID(CLSID, CLSID_App,0x4948E973,0x2FE7,0x4E6B,0x91,0x4E,0x55,0x41,0x7C,0xCF,0xDF,0x22);


MIDL_DEFINE_GUID(IID, DIID__IApp2Events,0xC75C9294,0x5D81,0x43B9,0xBD,0x63,0xB8,0x9C,0x4C,0xB0,0xF8,0xD9);


MIDL_DEFINE_GUID(CLSID, CLSID_App2,0x90BB5766,0x0F79,0x4E15,0x88,0x66,0xD4,0x26,0x34,0xEE,0x28,0xE9);


MIDL_DEFINE_GUID(IID, DIID__IRecordEvents,0x4DB873D3,0xE714,0x45D7,0xA2,0x09,0x2E,0x93,0x89,0x47,0x0F,0x51);


MIDL_DEFINE_GUID(CLSID, CLSID_Record,0xFFC55E59,0x833C,0x469D,0x82,0xEB,0xB1,0xAB,0xF7,0x65,0x79,0x3D);


MIDL_DEFINE_GUID(CLSID, CLSID_SearchResult,0xBD1341E4,0x5BDB,0x4866,0x87,0x5E,0x5B,0xB2,0x08,0xAA,0xDE,0xE4);


MIDL_DEFINE_GUID(CLSID, CLSID_FelixMemDocUIHandler,0x022ECF0F,0xE006,0x44D7,0xB9,0xBA,0xED,0x47,0x86,0xFB,0xDE,0x76);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



