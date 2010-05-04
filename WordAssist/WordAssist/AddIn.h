

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __AddIn_h__
#define __AddIn_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWord2HtmlParser_FWD_DEFINED__
#define __IWord2HtmlParser_FWD_DEFINED__
typedef interface IWord2HtmlParser IWord2HtmlParser;
#endif 	/* __IWord2HtmlParser_FWD_DEFINED__ */


#ifndef __Connect_FWD_DEFINED__
#define __Connect_FWD_DEFINED__

#ifdef __cplusplus
typedef class Connect Connect;
#else
typedef struct Connect Connect;
#endif /* __cplusplus */

#endif 	/* __Connect_FWD_DEFINED__ */


#ifndef __Word2HtmlParser_FWD_DEFINED__
#define __Word2HtmlParser_FWD_DEFINED__

#ifdef __cplusplus
typedef class Word2HtmlParser Word2HtmlParser;
#else
typedef struct Word2HtmlParser Word2HtmlParser;
#endif /* __cplusplus */

#endif 	/* __Word2HtmlParser_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWord2HtmlParser_INTERFACE_DEFINED__
#define __IWord2HtmlParser_INTERFACE_DEFINED__

/* interface IWord2HtmlParser */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWord2HtmlParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0C1F8208-FE35-46F0-A217-20D9567037E8")
    IWord2HtmlParser : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RangeToHtml( 
            /* [in] */ IDispatch *word_range,
            /* [retval][out] */ BSTR *html_text) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWord2HtmlParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWord2HtmlParser * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWord2HtmlParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWord2HtmlParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWord2HtmlParser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWord2HtmlParser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWord2HtmlParser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWord2HtmlParser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RangeToHtml )( 
            IWord2HtmlParser * This,
            /* [in] */ IDispatch *word_range,
            /* [retval][out] */ BSTR *html_text);
        
        END_INTERFACE
    } IWord2HtmlParserVtbl;

    interface IWord2HtmlParser
    {
        CONST_VTBL struct IWord2HtmlParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWord2HtmlParser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWord2HtmlParser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWord2HtmlParser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWord2HtmlParser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWord2HtmlParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWord2HtmlParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWord2HtmlParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWord2HtmlParser_RangeToHtml(This,word_range,html_text)	\
    ( (This)->lpVtbl -> RangeToHtml(This,word_range,html_text) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWord2HtmlParser_INTERFACE_DEFINED__ */



#ifndef __WordAssistLib_LIBRARY_DEFINED__
#define __WordAssistLib_LIBRARY_DEFINED__

/* library WordAssistLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WordAssistLib;

EXTERN_C const CLSID CLSID_Connect;

#ifdef __cplusplus

class DECLSPEC_UUID("E7986A6D-875D-459D-BD5C-69AB76E5A7D1")
Connect;
#endif

EXTERN_C const CLSID CLSID_Word2HtmlParser;

#ifdef __cplusplus

class DECLSPEC_UUID("D695331E-45B7-4EF2-9DC0-29EB314F68CB")
Word2HtmlParser;
#endif
#endif /* __WordAssistLib_LIBRARY_DEFINED__ */

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


