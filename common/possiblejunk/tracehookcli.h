// tracehookcli.h
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999 Chris Sells and Simon Fell
// All rights reserved.
//
// History:
// 9/2/2001  - Simon Fell fixes problem with tracing calls with really long strings
// 1/26/2000 - Simon Fell finds (and fixes) the method names bug for 
//			 typelibrary defined interfaces
//
// 8/15/99 - Simon Fell finally gets around to sorting out the perl scripts
//  1. Bug Fix for tracing BSTR's (now checks for NULL)
//  2. Perl Scripts and AddToDefs code packaged 
//
// 3/30/99 - Simon Fell makes 1 major change + plus a few minor ones
//	1. Adds support for dumping method names for standard interfaces
//  2. Now dumps the ThreadID at the start of the string
//  3. Adds support for IPeristStream so that the hook can be MBV'd with the UD
//			(although the current methods for attaching the hook do not request MBV)
//  4. Adds support for IPersistPropertyBag so that DECLARE_CLASSFACTORY_INTERCEPTOR
//			can be used.
//  5. Adds DECLARE_CLASSFACTORY_DEBUGTRACER as an alternative means of adding the hook
//  6. Fixes a potential problem in CTraceMethodHook::DelegatorPreprocess 
//			by making TCHAR sz[] bigger
//
// 1/25/99 - Simon Fell makes three major changes:
//  1. Adds support for stack dumping for oleautomation-compliant
//     data types, either directly or via GetIDsOfNames/Invoke in
//     the case of a dual.
//  2. Added support for tracing when objects are created via script.
//  3. Fixed Chris's UNICODE bugs.
//
// 1/8/99 - Initial release.
//
// Known Issues:
//  Doesn't output [out] params.
//
// NO WARRANTIES ARE EXTENDED. USE AT YOUR OWN RISK.
//
// To contact the author with suggestions or comments, use
// csells@sellsbrothers.com or simon@fell.com
/////////////////////////////////////////////////////////////////////////////
// This file containes the client-side definition of a delegator hook to
// extend Keith Brown's (http://www.develop.com/kbrown) most excellent
// Universal Delegator (UD). This hook traces calls to COM objects, dumping
// the results to debug output. This tracing happens without object intervention
// and based on type information.
/////////////////////////////////////////////////////////////////////////////
// The file provides a single macro for use in tracing COM object method
// calls w/o changing the source of the object being traced. Since it's a
// macro, the arguments have no types, but you can imagine it would be
// declared with the following parameterized-IDL:
//
// template <typename Itf>
// void TRACEHOOK([in] LPCOLESTR pszObjectName, [in, out] Itf** ppItf);
//
//  pszObjectName:  Name of the object being traced. Every subsequent interface
//                  obtained from this object from the traced interface via QI
//                  will also be wrapped in the trace hook. The pszObjectName
//                  argument will always be used to compose the trace output,
//                  regardless of the interface or method being traced.
//
// ppItf:           Interface on the object to trace. On return, this interface
//                  will actually point to an instance of a Universal Delegator,
//                  augmented with the Trace Hook.
//
// Note: TRACEHOOK is actually a macro that will perform it's magic when _DEBUG
//       is defined. In a release build, TRACEHOOK will fall away.
/////////////////////////////////////////////////////////////////////////////
// Usage:
/*
    // The following usage:

    // Get a COM object to wrap
    CComPtr<ICalc>  spCalc;
    spCalc.CoCreateInstance(CLSID_Calc);

    // Wrap it (when _DEBUG is defined only)
    TRACEHOOK(OLESTR("MyCalc"), &spCalc.p);

    // Use it
    spCalc->put_Sum(0);
    spCalc->Add(2);
    spCalc->Add(2);
    long nSum;
    spCalc->get_Sum(&nSum);

    // Results in the following debug output (with no change to the object's code):
    MyCalc, ICalc::put_Sum(0)
    MyCalc, ICalc::Add(2)
    MyCalc, ICalc::Add(2)
    MyCalc, ICalc::get_Sum()

// alternativly the definition of CCalc could be

	class ATL_NO_VTABLE CCalc : 
		public CComObjectRootEx<CComSingleThreadModel>,
		public CComCoClass<CCalc, &CLSID_Calc>,
		public IDispatchImpl<ICalc, &IID_ICalc, &LIBID_CALCSVRLib>
	{
	public:

		DECLARE_CLASSFACTORY_DEBUGTRACER(CClac, "Calc")

		// rest of def's
		
*/

#ifndef TRACEHOOKCLI_H
#define TRACEHOOKCLI_H

#define HR( x ) \
	{ ATLTRACE( "Test: [" #x "]\n" ) ;\
	HRESULT _hr = (x) ; \
	TRACE_HRESULT( _hr ) ; \
if ( FAILED( _hr ) ) return _hr ; } 

#define HR0( x ) \
	{ ATLTRACE( "Test: [" #x "]\n" ) ; \
	HRESULT _hr = (x) ; \
	TRACE_HRESULT( _hr ) ; \
if ( FAILED( _hr ) ) return ;  }

#define HRP( x ) \
	{ ATLTRACE( "Test: [" #x "]\n" ) ; \
	HRESULT _hr = (x) ; \
	TRACE_HRESULT( _hr ) ; \
if ( FAILED( _hr ) ) return NULL ;} 

#define HRB( x ) \
	{ ATLTRACE( "Test: [" #x "]\n" ) ; \
	HRESULT _hr = (x) ; \
	TRACE_HRESULT( _hr ) ; \
if ( FAILED( _hr ) ) return false ; }

#ifndef HR2
#define HR2(_e) HR0(_e) 
#endif  // HR2

#ifdef _DEBUG
inline void TraceHresult ( HRESULT hr )
{
	char buf[30] ; 
	ATLTRACE( "HRESULT = 0x%s", _itoa( hr, buf, 16 ) ) ; 		
	
	if ( hr == S_OK )
	{
		ATLTRACE( " (S_OK)\n" ) ;
		return ;
	}
	if ( hr == S_FALSE ) 
	{
		ATLTRACE( " (FALSE)\n" ) ;
		return ;
	}
	if ( SUCCEEDED( hr ) )
	{
		ATLTRACE( " (SUCCESS)\n" ) ;
		return ;
	}
	
	ATLTRACE( "\n" ) ;
	LPVOID buff(0)  ;
	if ( 0 == 
		::FormatMessage
		(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM ,
		NULL,
		(DWORD)hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &buff,
		0,
		NULL 
		)
		)
	{
		if ( buff != NULL )	::LocalFree( buff ) ;
		ATLTRACE( "*** Call to FormatMessage failed!\n" ) ;
		return ;
	}
	ATLASSERT( buff != NULL ) ;
	ATLTRACE( "Failure!\nDescription: %s\n", (TCHAR *)buff ) ;
	::LocalFree( buff ) ;
	::Sleep( 50 ) ;
}
#define TRACE_HRESULT( x ) TraceHresult( x ) ; 

#include <atlbase.h>        // CComPtr
#include <assert.h>         // assert
#include <delegate.h>
#include <udtracehooksvr.h>

#define TRACEHOOK(objname, ppItf) TraceHook(objname, ppItf)

inline void TraceHook(LPCOLESTR pszObjectName, REFIID riid, void** ppItf)
{
    // This parameter is [in, out]
    assert(ppItf && *ppItf);

    // Create an instance of the UD Trace Hook and hand it the object name
    CComPtr<ITraceHook> spHook;
    HR2(spHook.CoCreateInstance(OLESTR("UDTraceHookSvr.TraceHook")));
    HR2(spHook->SetObjectName(pszObjectName && *pszObjectName ? pszObjectName : OLESTR("<object>")));

    // Get the UD class object
    CComPtr<IDelegatorFactory>  spFactory;
    HR2(CoGetClassObject(__uuidof(CoDelegator21), CLSCTX_INPROC, 0, __uuidof(IDelegatorFactory), (void**)&spFactory));

    // Delegate!
    CComPtr<IUnknown>   spitfDelegator;
    IUnknown*           punk = reinterpret_cast<IUnknown*>(*ppItf);
    HR2(spFactory->CreateDelegator(0, punk, 0, spHook, 0, riid, (void**)&spitfDelegator));

    // Replace the original ppItf with the delegator
    punk->Release();
    *ppItf = spitfDelegator.Detach();
}

template <typename Itf>
inline void TraceHook(LPCOLESTR pszObjectName, Itf** ppItf)
{
    TraceHook(pszObjectName, __uuidof(Itf), (void**)ppItf);
}

template <class baseCF, class Obj>
class TracerClassFactory : public baseCF
{
	STDMETHOD(CreateInstance)(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
	{
		HRESULT hr = baseCF::CreateInstance(pUnkOuter, riid, ppvObj);
		if (SUCCEEDED(hr))
			TraceHook(Obj::DebugTracerName(), riid, ppvObj);
		return hr ;
	}
};

#define DECLARE_CLASSFACTORY_DEBUGTRACER(Obj, Name) \
			static const LPOLESTR DebugTracerName() { return L##Name ; } \
			typedef TracerClassFactory<CComClassFactory, Obj> _TCF ; \
			DECLARE_CLASSFACTORY_EX(_TCF) 

#else
#define TRACEHOOK(objname, ppItf) (void)0
#define TRACE_HRESULT( x )	(void)0

#define DECLARE_CLASSFACTORY_DEBUGTRACER(Obj, Name) \
			DECLARE_CLASSFACTORY_EX(CComClassFactory) ;
#endif


#endif  // TRACEHOOKCLI_H
