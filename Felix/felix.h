

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sat Nov 27 22:24:05 2010
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Felix_h__
#define __Felix_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IRecord_FWD_DEFINED__
#define __IRecord_FWD_DEFINED__
typedef interface IRecord IRecord;
#endif 	/* __IRecord_FWD_DEFINED__ */


#ifndef __ISearchResult_FWD_DEFINED__
#define __ISearchResult_FWD_DEFINED__
typedef interface ISearchResult ISearchResult;
#endif 	/* __ISearchResult_FWD_DEFINED__ */


#ifndef __ISearchResults_FWD_DEFINED__
#define __ISearchResults_FWD_DEFINED__
typedef interface ISearchResults ISearchResults;
#endif 	/* __ISearchResults_FWD_DEFINED__ */


#ifndef __IRecords_FWD_DEFINED__
#define __IRecords_FWD_DEFINED__
typedef interface IRecords IRecords;
#endif 	/* __IRecords_FWD_DEFINED__ */


#ifndef __IMemory_FWD_DEFINED__
#define __IMemory_FWD_DEFINED__
typedef interface IMemory IMemory;
#endif 	/* __IMemory_FWD_DEFINED__ */


#ifndef __IMemories_FWD_DEFINED__
#define __IMemories_FWD_DEFINED__
typedef interface IMemories IMemories;
#endif 	/* __IMemories_FWD_DEFINED__ */


#ifndef __IMemoryWindow_FWD_DEFINED__
#define __IMemoryWindow_FWD_DEFINED__
typedef interface IMemoryWindow IMemoryWindow;
#endif 	/* __IMemoryWindow_FWD_DEFINED__ */


#ifndef __IGlossaryWindow_FWD_DEFINED__
#define __IGlossaryWindow_FWD_DEFINED__
typedef interface IGlossaryWindow IGlossaryWindow;
#endif 	/* __IGlossaryWindow_FWD_DEFINED__ */


#ifndef __IApp2_FWD_DEFINED__
#define __IApp2_FWD_DEFINED__
typedef interface IApp2 IApp2;
#endif 	/* __IApp2_FWD_DEFINED__ */


#ifndef __IApp_FWD_DEFINED__
#define __IApp_FWD_DEFINED__
typedef interface IApp IApp;
#endif 	/* __IApp_FWD_DEFINED__ */


#ifndef __IFelixMemDocUIHandler_FWD_DEFINED__
#define __IFelixMemDocUIHandler_FWD_DEFINED__
typedef interface IFelixMemDocUIHandler IFelixMemDocUIHandler;
#endif 	/* __IFelixMemDocUIHandler_FWD_DEFINED__ */


#ifndef __App_FWD_DEFINED__
#define __App_FWD_DEFINED__

#ifdef __cplusplus
typedef class App App;
#else
typedef struct App App;
#endif /* __cplusplus */

#endif 	/* __App_FWD_DEFINED__ */


#ifndef ___IApp2Events_FWD_DEFINED__
#define ___IApp2Events_FWD_DEFINED__
typedef interface _IApp2Events _IApp2Events;
#endif 	/* ___IApp2Events_FWD_DEFINED__ */


#ifndef __App2_FWD_DEFINED__
#define __App2_FWD_DEFINED__

#ifdef __cplusplus
typedef class App2 App2;
#else
typedef struct App2 App2;
#endif /* __cplusplus */

#endif 	/* __App2_FWD_DEFINED__ */


#ifndef ___IRecordEvents_FWD_DEFINED__
#define ___IRecordEvents_FWD_DEFINED__
typedef interface _IRecordEvents _IRecordEvents;
#endif 	/* ___IRecordEvents_FWD_DEFINED__ */


#ifndef __Record_FWD_DEFINED__
#define __Record_FWD_DEFINED__

#ifdef __cplusplus
typedef class Record Record;
#else
typedef struct Record Record;
#endif /* __cplusplus */

#endif 	/* __Record_FWD_DEFINED__ */


#ifndef __SearchResult_FWD_DEFINED__
#define __SearchResult_FWD_DEFINED__

#ifdef __cplusplus
typedef class SearchResult SearchResult;
#else
typedef struct SearchResult SearchResult;
#endif /* __cplusplus */

#endif 	/* __SearchResult_FWD_DEFINED__ */


#ifndef __FelixMemDocUIHandler_FWD_DEFINED__
#define __FelixMemDocUIHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class FelixMemDocUIHandler FelixMemDocUIHandler;
#else
typedef struct FelixMemDocUIHandler FelixMemDocUIHandler;
#endif /* __cplusplus */

#endif 	/* __FelixMemDocUIHandler_FWD_DEFINED__ */


#ifndef __SearchResults_FWD_DEFINED__
#define __SearchResults_FWD_DEFINED__

#ifdef __cplusplus
typedef class SearchResults SearchResults;
#else
typedef struct SearchResults SearchResults;
#endif /* __cplusplus */

#endif 	/* __SearchResults_FWD_DEFINED__ */


#ifndef __ComMemory_FWD_DEFINED__
#define __ComMemory_FWD_DEFINED__

#ifdef __cplusplus
typedef class ComMemory ComMemory;
#else
typedef struct ComMemory ComMemory;
#endif /* __cplusplus */

#endif 	/* __ComMemory_FWD_DEFINED__ */


#ifndef __ComRecords_FWD_DEFINED__
#define __ComRecords_FWD_DEFINED__

#ifdef __cplusplus
typedef class ComRecords ComRecords;
#else
typedef struct ComRecords ComRecords;
#endif /* __cplusplus */

#endif 	/* __ComRecords_FWD_DEFINED__ */


#ifndef __MemoryWindow_FWD_DEFINED__
#define __MemoryWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class MemoryWindow MemoryWindow;
#else
typedef struct MemoryWindow MemoryWindow;
#endif /* __cplusplus */

#endif 	/* __MemoryWindow_FWD_DEFINED__ */


#ifndef __GlossaryWindow_FWD_DEFINED__
#define __GlossaryWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class GlossaryWindow GlossaryWindow;
#else
typedef struct GlossaryWindow GlossaryWindow;
#endif /* __cplusplus */

#endif 	/* __GlossaryWindow_FWD_DEFINED__ */


#ifndef __Memories_FWD_DEFINED__
#define __Memories_FWD_DEFINED__

#ifdef __cplusplus
typedef class Memories Memories;
#else
typedef struct Memories Memories;
#endif /* __cplusplus */

#endif 	/* __Memories_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "atliface.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IRecord_INTERFACE_DEFINED__
#define __IRecord_INTERFACE_DEFINED__

/* interface IRecord */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRecord;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6195F62F-B419-4C98-B168-BDA736ADC407")
    IRecord : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Source( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Source( 
            /* [in] */ BSTR source_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Trans( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Trans( 
            /* [in] */ BSTR trans_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Context( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Context( 
            /* [in] */ BSTR context_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlainSource( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlainTrans( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlainContext( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CreatedBy( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CreatedBy( 
            /* [in] */ BSTR created_by_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ModifiedBy( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ModifiedBy( 
            /* [in] */ BSTR modified_by_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DateCreated( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DateCreated( 
            /* [in] */ DATE created_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastModified( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LastModified( 
            /* [in] */ DATE modified_val) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RefCount( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RefCount( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Id( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Validated( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Validated( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Reliability( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Reliability( 
            /* [in] */ ULONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRecordVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRecord * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRecord * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRecord * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRecord * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRecord * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRecord * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRecord * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Source )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Source )( 
            IRecord * This,
            /* [in] */ BSTR source_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Trans )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Trans )( 
            IRecord * This,
            /* [in] */ BSTR trans_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Context )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Context )( 
            IRecord * This,
            /* [in] */ BSTR context_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlainSource )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlainTrans )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlainContext )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CreatedBy )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CreatedBy )( 
            IRecord * This,
            /* [in] */ BSTR created_by_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ModifiedBy )( 
            IRecord * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ModifiedBy )( 
            IRecord * This,
            /* [in] */ BSTR modified_by_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DateCreated )( 
            IRecord * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DateCreated )( 
            IRecord * This,
            /* [in] */ DATE created_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastModified )( 
            IRecord * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LastModified )( 
            IRecord * This,
            /* [in] */ DATE modified_val);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RefCount )( 
            IRecord * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RefCount )( 
            IRecord * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IRecord * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IRecord * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Validated )( 
            IRecord * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Validated )( 
            IRecord * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Reliability )( 
            IRecord * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Reliability )( 
            IRecord * This,
            /* [in] */ ULONG newVal);
        
        END_INTERFACE
    } IRecordVtbl;

    interface IRecord
    {
        CONST_VTBL struct IRecordVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRecord_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRecord_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRecord_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRecord_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRecord_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRecord_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRecord_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRecord_get_Source(This,pVal)	\
    ( (This)->lpVtbl -> get_Source(This,pVal) ) 

#define IRecord_put_Source(This,source_val)	\
    ( (This)->lpVtbl -> put_Source(This,source_val) ) 

#define IRecord_get_Trans(This,pVal)	\
    ( (This)->lpVtbl -> get_Trans(This,pVal) ) 

#define IRecord_put_Trans(This,trans_val)	\
    ( (This)->lpVtbl -> put_Trans(This,trans_val) ) 

#define IRecord_get_Context(This,pVal)	\
    ( (This)->lpVtbl -> get_Context(This,pVal) ) 

#define IRecord_put_Context(This,context_val)	\
    ( (This)->lpVtbl -> put_Context(This,context_val) ) 

#define IRecord_get_PlainSource(This,pVal)	\
    ( (This)->lpVtbl -> get_PlainSource(This,pVal) ) 

#define IRecord_get_PlainTrans(This,pVal)	\
    ( (This)->lpVtbl -> get_PlainTrans(This,pVal) ) 

#define IRecord_get_PlainContext(This,pVal)	\
    ( (This)->lpVtbl -> get_PlainContext(This,pVal) ) 

#define IRecord_get_CreatedBy(This,pVal)	\
    ( (This)->lpVtbl -> get_CreatedBy(This,pVal) ) 

#define IRecord_put_CreatedBy(This,created_by_val)	\
    ( (This)->lpVtbl -> put_CreatedBy(This,created_by_val) ) 

#define IRecord_get_ModifiedBy(This,pVal)	\
    ( (This)->lpVtbl -> get_ModifiedBy(This,pVal) ) 

#define IRecord_put_ModifiedBy(This,modified_by_val)	\
    ( (This)->lpVtbl -> put_ModifiedBy(This,modified_by_val) ) 

#define IRecord_get_DateCreated(This,pVal)	\
    ( (This)->lpVtbl -> get_DateCreated(This,pVal) ) 

#define IRecord_put_DateCreated(This,created_val)	\
    ( (This)->lpVtbl -> put_DateCreated(This,created_val) ) 

#define IRecord_get_LastModified(This,pVal)	\
    ( (This)->lpVtbl -> get_LastModified(This,pVal) ) 

#define IRecord_put_LastModified(This,modified_val)	\
    ( (This)->lpVtbl -> put_LastModified(This,modified_val) ) 

#define IRecord_get_RefCount(This,pVal)	\
    ( (This)->lpVtbl -> get_RefCount(This,pVal) ) 

#define IRecord_put_RefCount(This,newVal)	\
    ( (This)->lpVtbl -> put_RefCount(This,newVal) ) 

#define IRecord_get_Id(This,pVal)	\
    ( (This)->lpVtbl -> get_Id(This,pVal) ) 

#define IRecord_put_Id(This,newVal)	\
    ( (This)->lpVtbl -> put_Id(This,newVal) ) 

#define IRecord_get_Validated(This,pVal)	\
    ( (This)->lpVtbl -> get_Validated(This,pVal) ) 

#define IRecord_put_Validated(This,newVal)	\
    ( (This)->lpVtbl -> put_Validated(This,newVal) ) 

#define IRecord_get_Reliability(This,pVal)	\
    ( (This)->lpVtbl -> get_Reliability(This,pVal) ) 

#define IRecord_put_Reliability(This,newVal)	\
    ( (This)->lpVtbl -> put_Reliability(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRecord_INTERFACE_DEFINED__ */


#ifndef __ISearchResult_INTERFACE_DEFINED__
#define __ISearchResult_INTERFACE_DEFINED__

/* interface ISearchResult */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISearchResult;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("61647BA5-8AEA-487B-97B6-442CA9D0C433")
    ISearchResult : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Record( 
            /* [retval][out] */ IRecord **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Record( 
            /* [in] */ IRecord *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Score( 
            /* [retval][out] */ DOUBLE *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MemoryName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISearchResultVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISearchResult * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISearchResult * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISearchResult * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISearchResult * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISearchResult * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISearchResult * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISearchResult * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Record )( 
            ISearchResult * This,
            /* [retval][out] */ IRecord **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Record )( 
            ISearchResult * This,
            /* [in] */ IRecord *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Score )( 
            ISearchResult * This,
            /* [retval][out] */ DOUBLE *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MemoryName )( 
            ISearchResult * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } ISearchResultVtbl;

    interface ISearchResult
    {
        CONST_VTBL struct ISearchResultVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISearchResult_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISearchResult_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISearchResult_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISearchResult_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISearchResult_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISearchResult_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISearchResult_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISearchResult_get_Record(This,pVal)	\
    ( (This)->lpVtbl -> get_Record(This,pVal) ) 

#define ISearchResult_put_Record(This,newVal)	\
    ( (This)->lpVtbl -> put_Record(This,newVal) ) 

#define ISearchResult_get_Score(This,pVal)	\
    ( (This)->lpVtbl -> get_Score(This,pVal) ) 

#define ISearchResult_get_MemoryName(This,pVal)	\
    ( (This)->lpVtbl -> get_MemoryName(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISearchResult_INTERFACE_DEFINED__ */


#ifndef __ISearchResults_INTERFACE_DEFINED__
#define __ISearchResults_INTERFACE_DEFINED__

/* interface ISearchResults */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISearchResults;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("654061B0-533B-43C5-A631-3EDAB236238C")
    ISearchResults : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long n,
            /* [retval][out] */ ISearchResult **ppSearchResult) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISearchResultsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISearchResults * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISearchResults * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISearchResults * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISearchResults * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISearchResults * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISearchResults * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISearchResults * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ISearchResults * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ISearchResults * This,
            /* [in] */ long n,
            /* [retval][out] */ ISearchResult **ppSearchResult);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ISearchResults * This,
            /* [retval][out] */ IUnknown **ppEnum);
        
        END_INTERFACE
    } ISearchResultsVtbl;

    interface ISearchResults
    {
        CONST_VTBL struct ISearchResultsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISearchResults_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISearchResults_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISearchResults_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISearchResults_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISearchResults_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISearchResults_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISearchResults_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISearchResults_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define ISearchResults_get_Item(This,n,ppSearchResult)	\
    ( (This)->lpVtbl -> get_Item(This,n,ppSearchResult) ) 

#define ISearchResults_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISearchResults_INTERFACE_DEFINED__ */


#ifndef __IRecords_INTERFACE_DEFINED__
#define __IRecords_INTERFACE_DEFINED__

/* interface IRecords */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRecords;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("08CCF650-1CC9-47B1-8304-D9863E83C8F5")
    IRecords : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long n,
            /* [retval][out] */ IRecord **ppRecord) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRecordsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRecords * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRecords * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRecords * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRecords * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRecords * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRecords * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRecords * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IRecords * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IRecords * This,
            /* [in] */ long n,
            /* [retval][out] */ IRecord **ppRecord);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IRecords * This,
            /* [retval][out] */ IUnknown **ppEnum);
        
        END_INTERFACE
    } IRecordsVtbl;

    interface IRecords
    {
        CONST_VTBL struct IRecordsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRecords_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRecords_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRecords_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRecords_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRecords_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRecords_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRecords_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRecords_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IRecords_get_Item(This,n,ppRecord)	\
    ( (This)->lpVtbl -> get_Item(This,n,ppRecord) ) 

#define IRecords_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRecords_INTERFACE_DEFINED__ */


#ifndef __IMemory_INTERFACE_DEFINED__
#define __IMemory_INTERFACE_DEFINED__

/* interface IMemory */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMemory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F17938E6-D888-4760-970C-C1B4707B67A5")
    IMemory : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Records( 
            /* [retval][out] */ IRecords **newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Creator( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Creator( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ModifiedBy( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ModifiedBy( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Field( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Field( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Client( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Client( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SourceLanguage( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SourceLanguage( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TargetLanguage( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TargetLanguage( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CreatedOn( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CreatedOn( 
            /* [in] */ DATE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ModifiedOn( 
            /* [retval][out] */ DATE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ModifiedOn( 
            /* [in] */ DATE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsMemory( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsMemory( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsLocked( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsLocked( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddRecord( 
            /* [in] */ IRecord *newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveRecord( 
            /* [in] */ IRecord *newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [retval][out] */ ULONG *memSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveAs( 
            /* [in] */ BSTR location) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMemoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMemory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMemory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMemory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMemory * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMemory * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMemory * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMemory * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Records )( 
            IMemory * This,
            /* [retval][out] */ IRecords **newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Creator )( 
            IMemory * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Creator )( 
            IMemory * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ModifiedBy )( 
            IMemory * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ModifiedBy )( 
            IMemory * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Field )( 
            IMemory * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Field )( 
            IMemory * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Client )( 
            IMemory * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Client )( 
            IMemory * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SourceLanguage )( 
            IMemory * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SourceLanguage )( 
            IMemory * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TargetLanguage )( 
            IMemory * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TargetLanguage )( 
            IMemory * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CreatedOn )( 
            IMemory * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CreatedOn )( 
            IMemory * This,
            /* [in] */ DATE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ModifiedOn )( 
            IMemory * This,
            /* [retval][out] */ DATE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ModifiedOn )( 
            IMemory * This,
            /* [in] */ DATE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsMemory )( 
            IMemory * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsMemory )( 
            IMemory * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsLocked )( 
            IMemory * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsLocked )( 
            IMemory * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddRecord )( 
            IMemory * This,
            /* [in] */ IRecord *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveRecord )( 
            IMemory * This,
            /* [in] */ IRecord *newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IMemory * This,
            /* [retval][out] */ ULONG *memSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IMemory * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveAs )( 
            IMemory * This,
            /* [in] */ BSTR location);
        
        END_INTERFACE
    } IMemoryVtbl;

    interface IMemory
    {
        CONST_VTBL struct IMemoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMemory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMemory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMemory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMemory_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMemory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMemory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMemory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMemory_get_Records(This,newVal)	\
    ( (This)->lpVtbl -> get_Records(This,newVal) ) 

#define IMemory_get_Creator(This,pVal)	\
    ( (This)->lpVtbl -> get_Creator(This,pVal) ) 

#define IMemory_put_Creator(This,newVal)	\
    ( (This)->lpVtbl -> put_Creator(This,newVal) ) 

#define IMemory_get_ModifiedBy(This,pVal)	\
    ( (This)->lpVtbl -> get_ModifiedBy(This,pVal) ) 

#define IMemory_put_ModifiedBy(This,newVal)	\
    ( (This)->lpVtbl -> put_ModifiedBy(This,newVal) ) 

#define IMemory_get_Field(This,pVal)	\
    ( (This)->lpVtbl -> get_Field(This,pVal) ) 

#define IMemory_put_Field(This,newVal)	\
    ( (This)->lpVtbl -> put_Field(This,newVal) ) 

#define IMemory_get_Client(This,pVal)	\
    ( (This)->lpVtbl -> get_Client(This,pVal) ) 

#define IMemory_put_Client(This,newVal)	\
    ( (This)->lpVtbl -> put_Client(This,newVal) ) 

#define IMemory_get_SourceLanguage(This,pVal)	\
    ( (This)->lpVtbl -> get_SourceLanguage(This,pVal) ) 

#define IMemory_put_SourceLanguage(This,newVal)	\
    ( (This)->lpVtbl -> put_SourceLanguage(This,newVal) ) 

#define IMemory_get_TargetLanguage(This,pVal)	\
    ( (This)->lpVtbl -> get_TargetLanguage(This,pVal) ) 

#define IMemory_put_TargetLanguage(This,newVal)	\
    ( (This)->lpVtbl -> put_TargetLanguage(This,newVal) ) 

#define IMemory_get_CreatedOn(This,pVal)	\
    ( (This)->lpVtbl -> get_CreatedOn(This,pVal) ) 

#define IMemory_put_CreatedOn(This,newVal)	\
    ( (This)->lpVtbl -> put_CreatedOn(This,newVal) ) 

#define IMemory_get_ModifiedOn(This,pVal)	\
    ( (This)->lpVtbl -> get_ModifiedOn(This,pVal) ) 

#define IMemory_put_ModifiedOn(This,newVal)	\
    ( (This)->lpVtbl -> put_ModifiedOn(This,newVal) ) 

#define IMemory_get_IsMemory(This,pVal)	\
    ( (This)->lpVtbl -> get_IsMemory(This,pVal) ) 

#define IMemory_put_IsMemory(This,newVal)	\
    ( (This)->lpVtbl -> put_IsMemory(This,newVal) ) 

#define IMemory_get_IsLocked(This,pVal)	\
    ( (This)->lpVtbl -> get_IsLocked(This,pVal) ) 

#define IMemory_put_IsLocked(This,newVal)	\
    ( (This)->lpVtbl -> put_IsLocked(This,newVal) ) 

#define IMemory_AddRecord(This,newVal)	\
    ( (This)->lpVtbl -> AddRecord(This,newVal) ) 

#define IMemory_RemoveRecord(This,newVal)	\
    ( (This)->lpVtbl -> RemoveRecord(This,newVal) ) 

#define IMemory_GetSize(This,memSize)	\
    ( (This)->lpVtbl -> GetSize(This,memSize) ) 

#define IMemory_Save(This)	\
    ( (This)->lpVtbl -> Save(This) ) 

#define IMemory_SaveAs(This,location)	\
    ( (This)->lpVtbl -> SaveAs(This,location) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMemory_INTERFACE_DEFINED__ */


#ifndef __IMemories_INTERFACE_DEFINED__
#define __IMemories_INTERFACE_DEFINED__

/* interface IMemories */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMemories;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A1E62CF4-8737-414E-B8C6-D4A276F2C121")
    IMemories : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppEnum) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long n,
            /* [retval][out] */ IMemory **ppMemory) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [retval][out] */ IMemory **newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR location,
            /* [retval][out] */ IMemory **newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMemoriesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMemories * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMemories * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMemories * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMemories * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMemories * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMemories * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMemories * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IMemories * This,
            /* [retval][out] */ IUnknown **ppEnum);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IMemories * This,
            /* [in] */ long n,
            /* [retval][out] */ IMemory **ppMemory);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IMemories * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IMemories * This,
            /* [retval][out] */ IMemory **newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IMemories * This,
            /* [in] */ BSTR location,
            /* [retval][out] */ IMemory **newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IMemories * This);
        
        END_INTERFACE
    } IMemoriesVtbl;

    interface IMemories
    {
        CONST_VTBL struct IMemoriesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMemories_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMemories_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMemories_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMemories_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMemories_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMemories_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMemories_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMemories_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#define IMemories_get_Item(This,n,ppMemory)	\
    ( (This)->lpVtbl -> get_Item(This,n,ppMemory) ) 

#define IMemories_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IMemories_Add(This,newVal)	\
    ( (This)->lpVtbl -> Add(This,newVal) ) 

#define IMemories_Load(This,location,newVal)	\
    ( (This)->lpVtbl -> Load(This,location,newVal) ) 

#define IMemories_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMemories_INTERFACE_DEFINED__ */


#ifndef __IMemoryWindow_INTERFACE_DEFINED__
#define __IMemoryWindow_INTERFACE_DEFINED__

/* interface IMemoryWindow */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMemoryWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42569D58-8B46-4A9F-869F-C839C5054ECA")
    IMemoryWindow : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Raise( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMemoryWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMemoryWindow * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMemoryWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMemoryWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMemoryWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMemoryWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMemoryWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMemoryWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IMemoryWindow * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IMemoryWindow * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IMemoryWindow * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IMemoryWindow * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IMemoryWindow * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IMemoryWindow * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IMemoryWindow * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IMemoryWindow * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Raise )( 
            IMemoryWindow * This);
        
        END_INTERFACE
    } IMemoryWindowVtbl;

    interface IMemoryWindow
    {
        CONST_VTBL struct IMemoryWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMemoryWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMemoryWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMemoryWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMemoryWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMemoryWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMemoryWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMemoryWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMemoryWindow_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IMemoryWindow_put_Height(This,newVal)	\
    ( (This)->lpVtbl -> put_Height(This,newVal) ) 

#define IMemoryWindow_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IMemoryWindow_put_Width(This,newVal)	\
    ( (This)->lpVtbl -> put_Width(This,newVal) ) 

#define IMemoryWindow_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IMemoryWindow_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IMemoryWindow_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IMemoryWindow_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IMemoryWindow_Raise(This)	\
    ( (This)->lpVtbl -> Raise(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMemoryWindow_INTERFACE_DEFINED__ */


#ifndef __IGlossaryWindow_INTERFACE_DEFINED__
#define __IGlossaryWindow_INTERFACE_DEFINED__

/* interface IGlossaryWindow */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IGlossaryWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BA98FAC9-3027-46EB-B870-0769A250229C")
    IGlossaryWindow : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IGlossaryWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGlossaryWindow * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGlossaryWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGlossaryWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGlossaryWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGlossaryWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGlossaryWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGlossaryWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IGlossaryWindowVtbl;

    interface IGlossaryWindow
    {
        CONST_VTBL struct IGlossaryWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGlossaryWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGlossaryWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGlossaryWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGlossaryWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGlossaryWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGlossaryWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGlossaryWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGlossaryWindow_INTERFACE_DEFINED__ */


#ifndef __IApp2_INTERFACE_DEFINED__
#define __IApp2_INTERFACE_DEFINED__

/* interface IApp2 */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IApp2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CC9981F5-4D27-4F01-A789-40FA4DFE5399")
    IApp2 : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentMatch( 
            /* [retval][out] */ ISearchResult **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentMatch( 
            /* [in] */ ISearchResult *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentMatches( 
            /* [retval][out] */ ISearchResults **newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentGlossMatches( 
            /* [retval][out] */ ISearchResults **newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReflectChanges( 
            /* [in] */ ULONG RecId,
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReviewTranslation( 
            /* [in] */ ULONG RecId,
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveMemory( 
            /* [retval][out] */ IMemory **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveGlossary( 
            /* [retval][out] */ IMemory **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MemoryWindow( 
            /* [retval][out] */ IMemoryWindow **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GlossaryWindow( 
            /* [retval][out] */ IMemoryWindow **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Memories( 
            /* [retval][out] */ IMemories **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Glossaries( 
            /* [retval][out] */ IMemories **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApp2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApp2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApp2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApp2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IApp2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IApp2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IApp2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IApp2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentMatch )( 
            IApp2 * This,
            /* [retval][out] */ ISearchResult **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentMatch )( 
            IApp2 * This,
            /* [in] */ ISearchResult *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentMatches )( 
            IApp2 * This,
            /* [retval][out] */ ISearchResults **newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentGlossMatches )( 
            IApp2 * This,
            /* [retval][out] */ ISearchResults **newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReflectChanges )( 
            IApp2 * This,
            /* [in] */ ULONG RecId,
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReviewTranslation )( 
            IApp2 * This,
            /* [in] */ ULONG RecId,
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveMemory )( 
            IApp2 * This,
            /* [retval][out] */ IMemory **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveGlossary )( 
            IApp2 * This,
            /* [retval][out] */ IMemory **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MemoryWindow )( 
            IApp2 * This,
            /* [retval][out] */ IMemoryWindow **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GlossaryWindow )( 
            IApp2 * This,
            /* [retval][out] */ IMemoryWindow **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Memories )( 
            IApp2 * This,
            /* [retval][out] */ IMemories **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Glossaries )( 
            IApp2 * This,
            /* [retval][out] */ IMemories **pVal);
        
        END_INTERFACE
    } IApp2Vtbl;

    interface IApp2
    {
        CONST_VTBL struct IApp2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApp2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IApp2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IApp2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IApp2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IApp2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IApp2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IApp2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IApp2_get_CurrentMatch(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentMatch(This,pVal) ) 

#define IApp2_put_CurrentMatch(This,newVal)	\
    ( (This)->lpVtbl -> put_CurrentMatch(This,newVal) ) 

#define IApp2_get_CurrentMatches(This,newVal)	\
    ( (This)->lpVtbl -> get_CurrentMatches(This,newVal) ) 

#define IApp2_get_CurrentGlossMatches(This,newVal)	\
    ( (This)->lpVtbl -> get_CurrentGlossMatches(This,newVal) ) 

#define IApp2_ReflectChanges(This,RecId,Source,Trans)	\
    ( (This)->lpVtbl -> ReflectChanges(This,RecId,Source,Trans) ) 

#define IApp2_ReviewTranslation(This,RecId,Source,Trans)	\
    ( (This)->lpVtbl -> ReviewTranslation(This,RecId,Source,Trans) ) 

#define IApp2_get_ActiveMemory(This,pVal)	\
    ( (This)->lpVtbl -> get_ActiveMemory(This,pVal) ) 

#define IApp2_get_ActiveGlossary(This,pVal)	\
    ( (This)->lpVtbl -> get_ActiveGlossary(This,pVal) ) 

#define IApp2_get_MemoryWindow(This,pVal)	\
    ( (This)->lpVtbl -> get_MemoryWindow(This,pVal) ) 

#define IApp2_get_GlossaryWindow(This,pVal)	\
    ( (This)->lpVtbl -> get_GlossaryWindow(This,pVal) ) 

#define IApp2_get_Memories(This,pVal)	\
    ( (This)->lpVtbl -> get_Memories(This,pVal) ) 

#define IApp2_get_Glossaries(This,pVal)	\
    ( (This)->lpVtbl -> get_Glossaries(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IApp2_INTERFACE_DEFINED__ */


#ifndef __IApp_INTERFACE_DEFINED__
#define __IApp_INTERFACE_DEFINED__

/* interface IApp */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IApp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C9063D7F-5F1F-4155-9666-C349030FC4B3")
    IApp : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Quit( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Lookup( 
            /* [in] */ BSTR Query) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LookupTrans( 
            /* [in] */ BSTR Trans) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Concordance( 
            /* [in] */ BSTR Query) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransConcordance( 
            BSTR Trans) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Score( 
            /* [retval][out] */ DOUBLE *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Query( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Query( 
            /* [in] */ BSTR pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Trans( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Trans( 
            /* [in] */ BSTR pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NextTrans( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrevTrans( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GlossMatch( 
            /* [in] */ SHORT Index,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumGlossMatches( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadMemory( 
            /* [in] */ BSTR MemoryName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadGlossary( 
            /* [in] */ BSTR GlossaryName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearMemories( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearGlossaries( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowMarkup( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowMarkup( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddMemoryEntry( 
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans,
            /* [in] */ BSTR EntryContext) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddGlossaryEntry( 
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans,
            /* [in] */ BSTR EntryContext) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteMemEntry( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CorrectTrans( 
            /* [in] */ BSTR Trans) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumMatches( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_App2( 
            /* [retval][out] */ IApp2 **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LookupDeferred( 
            /* [in] */ BSTR Query) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LookupTransDeferred( 
            /* [in] */ BSTR Trans) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAppVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApp * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApp * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IApp * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IApp * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IApp * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IApp * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IApp * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IApp * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Quit )( 
            IApp * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Lookup )( 
            IApp * This,
            /* [in] */ BSTR Query);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LookupTrans )( 
            IApp * This,
            /* [in] */ BSTR Trans);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Concordance )( 
            IApp * This,
            /* [in] */ BSTR Query);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransConcordance )( 
            IApp * This,
            BSTR Trans);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Score )( 
            IApp * This,
            /* [retval][out] */ DOUBLE *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Query )( 
            IApp * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Query )( 
            IApp * This,
            /* [in] */ BSTR pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Trans )( 
            IApp * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Trans )( 
            IApp * This,
            /* [in] */ BSTR pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IApp * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NextTrans )( 
            IApp * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PrevTrans )( 
            IApp * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GlossMatch )( 
            IApp * This,
            /* [in] */ SHORT Index,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumGlossMatches )( 
            IApp * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadMemory )( 
            IApp * This,
            /* [in] */ BSTR MemoryName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadGlossary )( 
            IApp * This,
            /* [in] */ BSTR GlossaryName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearMemories )( 
            IApp * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearGlossaries )( 
            IApp * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowMarkup )( 
            IApp * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowMarkup )( 
            IApp * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddMemoryEntry )( 
            IApp * This,
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans,
            /* [in] */ BSTR EntryContext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddGlossaryEntry )( 
            IApp * This,
            /* [in] */ BSTR Source,
            /* [in] */ BSTR Trans,
            /* [in] */ BSTR EntryContext);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteMemEntry )( 
            IApp * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CorrectTrans )( 
            IApp * This,
            /* [in] */ BSTR Trans);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumMatches )( 
            IApp * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_App2 )( 
            IApp * This,
            /* [retval][out] */ IApp2 **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LookupDeferred )( 
            IApp * This,
            /* [in] */ BSTR Query);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LookupTransDeferred )( 
            IApp * This,
            /* [in] */ BSTR Trans);
        
        END_INTERFACE
    } IAppVtbl;

    interface IApp
    {
        CONST_VTBL struct IAppVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApp_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IApp_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IApp_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IApp_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IApp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IApp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IApp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IApp_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IApp_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IApp_Quit(This)	\
    ( (This)->lpVtbl -> Quit(This) ) 

#define IApp_Lookup(This,Query)	\
    ( (This)->lpVtbl -> Lookup(This,Query) ) 

#define IApp_LookupTrans(This,Trans)	\
    ( (This)->lpVtbl -> LookupTrans(This,Trans) ) 

#define IApp_Concordance(This,Query)	\
    ( (This)->lpVtbl -> Concordance(This,Query) ) 

#define IApp_TransConcordance(This,Trans)	\
    ( (This)->lpVtbl -> TransConcordance(This,Trans) ) 

#define IApp_get_Score(This,pVal)	\
    ( (This)->lpVtbl -> get_Score(This,pVal) ) 

#define IApp_get_Query(This,pVal)	\
    ( (This)->lpVtbl -> get_Query(This,pVal) ) 

#define IApp_put_Query(This,pVal)	\
    ( (This)->lpVtbl -> put_Query(This,pVal) ) 

#define IApp_get_Trans(This,pVal)	\
    ( (This)->lpVtbl -> get_Trans(This,pVal) ) 

#define IApp_put_Trans(This,pVal)	\
    ( (This)->lpVtbl -> put_Trans(This,pVal) ) 

#define IApp_Save(This)	\
    ( (This)->lpVtbl -> Save(This) ) 

#define IApp_NextTrans(This)	\
    ( (This)->lpVtbl -> NextTrans(This) ) 

#define IApp_PrevTrans(This)	\
    ( (This)->lpVtbl -> PrevTrans(This) ) 

#define IApp_get_GlossMatch(This,Index,pVal)	\
    ( (This)->lpVtbl -> get_GlossMatch(This,Index,pVal) ) 

#define IApp_get_NumGlossMatches(This,pVal)	\
    ( (This)->lpVtbl -> get_NumGlossMatches(This,pVal) ) 

#define IApp_LoadMemory(This,MemoryName)	\
    ( (This)->lpVtbl -> LoadMemory(This,MemoryName) ) 

#define IApp_LoadGlossary(This,GlossaryName)	\
    ( (This)->lpVtbl -> LoadGlossary(This,GlossaryName) ) 

#define IApp_ClearMemories(This)	\
    ( (This)->lpVtbl -> ClearMemories(This) ) 

#define IApp_ClearGlossaries(This)	\
    ( (This)->lpVtbl -> ClearGlossaries(This) ) 

#define IApp_get_ShowMarkup(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowMarkup(This,pVal) ) 

#define IApp_put_ShowMarkup(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowMarkup(This,newVal) ) 

#define IApp_AddMemoryEntry(This,Source,Trans,EntryContext)	\
    ( (This)->lpVtbl -> AddMemoryEntry(This,Source,Trans,EntryContext) ) 

#define IApp_AddGlossaryEntry(This,Source,Trans,EntryContext)	\
    ( (This)->lpVtbl -> AddGlossaryEntry(This,Source,Trans,EntryContext) ) 

#define IApp_DeleteMemEntry(This)	\
    ( (This)->lpVtbl -> DeleteMemEntry(This) ) 

#define IApp_CorrectTrans(This,Trans)	\
    ( (This)->lpVtbl -> CorrectTrans(This,Trans) ) 

#define IApp_get_NumMatches(This,pVal)	\
    ( (This)->lpVtbl -> get_NumMatches(This,pVal) ) 

#define IApp_get_App2(This,pVal)	\
    ( (This)->lpVtbl -> get_App2(This,pVal) ) 

#define IApp_LookupDeferred(This,Query)	\
    ( (This)->lpVtbl -> LookupDeferred(This,Query) ) 

#define IApp_LookupTransDeferred(This,Trans)	\
    ( (This)->lpVtbl -> LookupTransDeferred(This,Trans) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IApp_INTERFACE_DEFINED__ */


#ifndef __IFelixMemDocUIHandler_INTERFACE_DEFINED__
#define __IFelixMemDocUIHandler_INTERFACE_DEFINED__

/* interface IFelixMemDocUIHandler */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IFelixMemDocUIHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("13AA9D46-D1BD-4531-95F7-00357EB31D8D")
    IFelixMemDocUIHandler : public IDocHostUIHandlerDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IFelixMemDocUIHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFelixMemDocUIHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFelixMemDocUIHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFelixMemDocUIHandler * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *ShowContextMenu )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ DWORD dwID,
            /* [in] */ DWORD x,
            /* [in] */ DWORD y,
            /* [in] */ IUnknown *pcmdtReserved,
            /* [in] */ IDispatch *pdispReserved,
            /* [retval][out] */ HRESULT *dwRetVal);
        
        HRESULT ( STDMETHODCALLTYPE *GetHostInfo )( 
            IFelixMemDocUIHandler * This,
            /* [out][in] */ DWORD *pdwFlags,
            /* [out][in] */ DWORD *pdwDoubleClick);
        
        HRESULT ( STDMETHODCALLTYPE *ShowUI )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ DWORD dwID,
            /* [in] */ IUnknown *pActiveObject,
            /* [in] */ IUnknown *pCommandTarget,
            /* [in] */ IUnknown *pFrame,
            /* [in] */ IUnknown *pDoc,
            /* [retval][out] */ HRESULT *dwRetVal);
        
        HRESULT ( STDMETHODCALLTYPE *HideUI )( 
            IFelixMemDocUIHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateUI )( 
            IFelixMemDocUIHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnableModeless )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ VARIANT_BOOL fEnable);
        
        HRESULT ( STDMETHODCALLTYPE *OnDocWindowActivate )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ VARIANT_BOOL fActivate);
        
        HRESULT ( STDMETHODCALLTYPE *OnFrameWindowActivate )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ VARIANT_BOOL fActivate);
        
        HRESULT ( STDMETHODCALLTYPE *ResizeBorder )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ long left,
            /* [in] */ long top,
            /* [in] */ long right,
            /* [in] */ long bottom,
            /* [in] */ IUnknown *pUIWindow,
            /* [in] */ VARIANT_BOOL fFrameWindow);
        
        HRESULT ( STDMETHODCALLTYPE *TranslateAccelerator )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ DWORD_PTR hWnd,
            /* [in] */ DWORD nMessage,
            /* [in] */ DWORD_PTR wParam,
            /* [in] */ DWORD_PTR lParam,
            /* [in] */ BSTR bstrGuidCmdGroup,
            /* [in] */ DWORD nCmdID,
            /* [retval][out] */ HRESULT *dwRetVal);
        
        HRESULT ( STDMETHODCALLTYPE *GetOptionKeyPath )( 
            IFelixMemDocUIHandler * This,
            /* [out] */ BSTR *pbstrKey,
            /* [in] */ DWORD dw);
        
        HRESULT ( STDMETHODCALLTYPE *GetDropTarget )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ IUnknown *pDropTarget,
            /* [out] */ IUnknown **ppDropTarget);
        
        HRESULT ( STDMETHODCALLTYPE *GetExternal )( 
            IFelixMemDocUIHandler * This,
            /* [out] */ IDispatch **ppDispatch);
        
        HRESULT ( STDMETHODCALLTYPE *TranslateUrl )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ DWORD dwTranslate,
            /* [in] */ BSTR bstrURLIn,
            /* [out] */ BSTR *pbstrURLOut);
        
        HRESULT ( STDMETHODCALLTYPE *FilterDataObject )( 
            IFelixMemDocUIHandler * This,
            /* [in] */ IUnknown *pDO,
            /* [out] */ IUnknown **ppDORet);
        
        END_INTERFACE
    } IFelixMemDocUIHandlerVtbl;

    interface IFelixMemDocUIHandler
    {
        CONST_VTBL struct IFelixMemDocUIHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFelixMemDocUIHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFelixMemDocUIHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFelixMemDocUIHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFelixMemDocUIHandler_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFelixMemDocUIHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFelixMemDocUIHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFelixMemDocUIHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IFelixMemDocUIHandler_ShowContextMenu(This,dwID,x,y,pcmdtReserved,pdispReserved,dwRetVal)	\
    ( (This)->lpVtbl -> ShowContextMenu(This,dwID,x,y,pcmdtReserved,pdispReserved,dwRetVal) ) 

#define IFelixMemDocUIHandler_GetHostInfo(This,pdwFlags,pdwDoubleClick)	\
    ( (This)->lpVtbl -> GetHostInfo(This,pdwFlags,pdwDoubleClick) ) 

#define IFelixMemDocUIHandler_ShowUI(This,dwID,pActiveObject,pCommandTarget,pFrame,pDoc,dwRetVal)	\
    ( (This)->lpVtbl -> ShowUI(This,dwID,pActiveObject,pCommandTarget,pFrame,pDoc,dwRetVal) ) 

#define IFelixMemDocUIHandler_HideUI(This)	\
    ( (This)->lpVtbl -> HideUI(This) ) 

#define IFelixMemDocUIHandler_UpdateUI(This)	\
    ( (This)->lpVtbl -> UpdateUI(This) ) 

#define IFelixMemDocUIHandler_EnableModeless(This,fEnable)	\
    ( (This)->lpVtbl -> EnableModeless(This,fEnable) ) 

#define IFelixMemDocUIHandler_OnDocWindowActivate(This,fActivate)	\
    ( (This)->lpVtbl -> OnDocWindowActivate(This,fActivate) ) 

#define IFelixMemDocUIHandler_OnFrameWindowActivate(This,fActivate)	\
    ( (This)->lpVtbl -> OnFrameWindowActivate(This,fActivate) ) 

#define IFelixMemDocUIHandler_ResizeBorder(This,left,top,right,bottom,pUIWindow,fFrameWindow)	\
    ( (This)->lpVtbl -> ResizeBorder(This,left,top,right,bottom,pUIWindow,fFrameWindow) ) 

#define IFelixMemDocUIHandler_TranslateAccelerator(This,hWnd,nMessage,wParam,lParam,bstrGuidCmdGroup,nCmdID,dwRetVal)	\
    ( (This)->lpVtbl -> TranslateAccelerator(This,hWnd,nMessage,wParam,lParam,bstrGuidCmdGroup,nCmdID,dwRetVal) ) 

#define IFelixMemDocUIHandler_GetOptionKeyPath(This,pbstrKey,dw)	\
    ( (This)->lpVtbl -> GetOptionKeyPath(This,pbstrKey,dw) ) 

#define IFelixMemDocUIHandler_GetDropTarget(This,pDropTarget,ppDropTarget)	\
    ( (This)->lpVtbl -> GetDropTarget(This,pDropTarget,ppDropTarget) ) 

#define IFelixMemDocUIHandler_GetExternal(This,ppDispatch)	\
    ( (This)->lpVtbl -> GetExternal(This,ppDispatch) ) 

#define IFelixMemDocUIHandler_TranslateUrl(This,dwTranslate,bstrURLIn,pbstrURLOut)	\
    ( (This)->lpVtbl -> TranslateUrl(This,dwTranslate,bstrURLIn,pbstrURLOut) ) 

#define IFelixMemDocUIHandler_FilterDataObject(This,pDO,ppDORet)	\
    ( (This)->lpVtbl -> FilterDataObject(This,pDO,ppDORet) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFelixMemDocUIHandler_INTERFACE_DEFINED__ */



#ifndef __Felix_LIBRARY_DEFINED__
#define __Felix_LIBRARY_DEFINED__

/* library Felix */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Felix;

EXTERN_C const CLSID CLSID_App;

#ifdef __cplusplus

class DECLSPEC_UUID("4948E973-2FE7-4E6B-914E-55417CCFDF22")
App;
#endif

#ifndef ___IApp2Events_DISPINTERFACE_DEFINED__
#define ___IApp2Events_DISPINTERFACE_DEFINED__

/* dispinterface _IApp2Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IApp2Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C75C9294-5D81-43B9-BD63-B89C4CB0F8D9")
    _IApp2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IApp2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IApp2Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IApp2Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IApp2Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IApp2Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IApp2Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IApp2Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IApp2Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IApp2EventsVtbl;

    interface _IApp2Events
    {
        CONST_VTBL struct _IApp2EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IApp2Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IApp2Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IApp2Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IApp2Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IApp2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IApp2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IApp2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IApp2Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_App2;

#ifdef __cplusplus

class DECLSPEC_UUID("90BB5766-0F79-4E15-8866-D42634EE28E9")
App2;
#endif

#ifndef ___IRecordEvents_DISPINTERFACE_DEFINED__
#define ___IRecordEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IRecordEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IRecordEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4DB873D3-E714-45D7-A209-2E9389470F51")
    _IRecordEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IRecordEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IRecordEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IRecordEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IRecordEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IRecordEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IRecordEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IRecordEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IRecordEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IRecordEventsVtbl;

    interface _IRecordEvents
    {
        CONST_VTBL struct _IRecordEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IRecordEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IRecordEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IRecordEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IRecordEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IRecordEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IRecordEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IRecordEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IRecordEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Record;

#ifdef __cplusplus

class DECLSPEC_UUID("FFC55E59-833C-469D-82EB-B1ABF765793D")
Record;
#endif

EXTERN_C const CLSID CLSID_SearchResult;

#ifdef __cplusplus

class DECLSPEC_UUID("BD1341E4-5BDB-4866-875E-5BB208AADEE4")
SearchResult;
#endif

EXTERN_C const CLSID CLSID_FelixMemDocUIHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("022ECF0F-E006-44D7-B9BA-ED4786FBDE76")
FelixMemDocUIHandler;
#endif

EXTERN_C const CLSID CLSID_SearchResults;

#ifdef __cplusplus

class DECLSPEC_UUID("AE39B643-6534-42D9-A4A1-9C5F414BD6D8")
SearchResults;
#endif

EXTERN_C const CLSID CLSID_ComMemory;

#ifdef __cplusplus

class DECLSPEC_UUID("0CEE03C8-86F5-437C-940E-E30FB40FD3FA")
ComMemory;
#endif

EXTERN_C const CLSID CLSID_ComRecords;

#ifdef __cplusplus

class DECLSPEC_UUID("8B991DDC-940F-42B1-A779-81CF64C94B6E")
ComRecords;
#endif

EXTERN_C const CLSID CLSID_MemoryWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("E63B3CE1-D643-4062-9349-8A96A162BE6A")
MemoryWindow;
#endif

EXTERN_C const CLSID CLSID_GlossaryWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("182930DF-8DED-4F9C-A330-2C8A39CBE79B")
GlossaryWindow;
#endif

EXTERN_C const CLSID CLSID_Memories;

#ifdef __cplusplus

class DECLSPEC_UUID("27A55855-3A19-4C57-B7CA-F49AC6A8E82C")
Memories;
#endif
#endif /* __Felix_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


