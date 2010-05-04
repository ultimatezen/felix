

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0499 */
/* at Mon May 03 07:52:49 2010
 */
/* Compiler settings for .\AddIn.idl:
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

MIDL_DEFINE_GUID(IID, IID_IWord2HtmlParser,0x0C1F8208,0xFE35,0x46F0,0xA2,0x17,0x20,0xD9,0x56,0x70,0x37,0xE8);


MIDL_DEFINE_GUID(IID, LIBID_WordAssistLib,0xD455DA16,0x6885,0x443C,0x87,0xEA,0x49,0x44,0xA4,0x20,0xEB,0x40);


MIDL_DEFINE_GUID(CLSID, CLSID_Connect,0xE7986A6D,0x875D,0x459D,0xBD,0x5C,0x69,0xAB,0x76,0xE5,0xA7,0xD1);


MIDL_DEFINE_GUID(CLSID, CLSID_Word2HtmlParser,0xD695331E,0x45B7,0x4EF2,0x9D,0xC0,0x29,0xEB,0x31,0x4F,0x68,0xCB);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



