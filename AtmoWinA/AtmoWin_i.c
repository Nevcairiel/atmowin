

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sat Jun 16 11:59:45 2012
 */
/* Compiler settings for .\AtmoWin.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


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

MIDL_DEFINE_GUID(IID, IID_IAtmoRemoteControl,0xe01e2044,0x5afc,0x11d3,0x8e,0x80,0x00,0x80,0x5f,0x91,0xcd,0xd9);


MIDL_DEFINE_GUID(IID, IID_IAtmoRemoteControl2,0xe01e2044,0x5bfc,0x11d4,0x8e,0x80,0x00,0x80,0x5f,0x91,0xcd,0xd9);


MIDL_DEFINE_GUID(IID, IID_IAtmoLiveViewControl,0xe01e2047,0x5afc,0x11d3,0x8e,0x80,0x00,0x80,0x5f,0x91,0xcd,0xd9);


MIDL_DEFINE_GUID(IID, LIBID_AtmoWinRemoteControl,0xe01e2041,0x5afc,0x11d3,0x8e,0x80,0x00,0x80,0x5f,0x91,0xcd,0xd9);


MIDL_DEFINE_GUID(CLSID, CLSID_AtmoRemoteControl,0xe01e2045,0x5afc,0x11d3,0x8e,0x80,0x00,0x80,0x5f,0x91,0xce,0xe9);


MIDL_DEFINE_GUID(CLSID, CLSID_AtmoLiveViewControl,0xe01e2044,0x5afc,0x11d3,0x8e,0x80,0x00,0x80,0x5f,0x91,0xce,0xe9);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

