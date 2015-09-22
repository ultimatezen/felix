// FelixMemDocUIHandler.h : Declaration of the CFelixMemDocUIHandler

#pragma once
#include "resource.h"       // main symbols
#include "Atliface.h"
#include "Felix.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


//! Handles right-clicks in Felix HTML views
//! Implements IDocHostUIHandlerDispatch -- An interface to the Microsoft HTML parsing and rendering engine.
class ATL_NO_VTABLE CFelixMemDocUIHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFelixMemDocUIHandler, &CLSID_FelixMemDocUIHandler>,
	public IDispatchImpl<IFelixMemDocUIHandler, &IID_IFelixMemDocUIHandler, &LIBID_Felix, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	DECLARE_SENSING_VAR ;

	boost::function< HRESULT() > m_get_menu ;

	CFelixMemDocUIHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FELIXMEMDOCUIHANDLER)


BEGIN_COM_MAP(CFelixMemDocUIHandler)
	COM_INTERFACE_ENTRY(IFelixMemDocUIHandler)
	COM_INTERFACE_ENTRY(IDocHostUIHandlerDispatch)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	HRESULT STDMETHODCALLTYPE ShowContextMenu( 
		/* [in] */ DWORD dwID,
		/* [in] */ DWORD x,
		/* [in] */ DWORD y,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved,
		/* [retval][out] */ HRESULT *dwRetVal);

	HRESULT STDMETHODCALLTYPE GetHostInfo( 
		/* [out][in] */ DWORD *pdwFlags,
		/* [out][in] */ DWORD *pdwDoubleClick);

	HRESULT STDMETHODCALLTYPE ShowUI( 
		/* [in] */ DWORD dwID,
		/* [in] */ IUnknown *pActiveObject,
		/* [in] */ IUnknown *pCommandTarget,
		/* [in] */ IUnknown *pFrame,
		/* [in] */ IUnknown *pDoc,
		/* [retval][out] */ HRESULT *dwRetVal);

	HRESULT STDMETHODCALLTYPE HideUI( void);

	HRESULT STDMETHODCALLTYPE UpdateUI( void);

	HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ VARIANT_BOOL fEnable);

	HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
		/* [in] */ VARIANT_BOOL fActivate);

	HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
		/* [in] */ VARIANT_BOOL fActivate);

	HRESULT STDMETHODCALLTYPE ResizeBorder( 
		/* [in] */ long left,
		/* [in] */ long top,
		/* [in] */ long right,
		/* [in] */ long bottom,
		/* [in] */ IUnknown *pUIWindow,
		/* [in] */ VARIANT_BOOL fFrameWindow);

	HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ DWORD_PTR hWnd,
		/* [in] */ DWORD nMessage,
		/* [in] */ DWORD_PTR wParam,
		/* [in] */ DWORD_PTR lParam,
		/* [in] */ BSTR bstrGuidCmdGroup,
		/* [in] */ DWORD nCmdID,
		/* [retval][out] */ HRESULT *dwRetVal);

	HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
		/* [out] */ BSTR *pbstrKey,
		/* [in] */ DWORD dw);

	HRESULT STDMETHODCALLTYPE GetDropTarget( 
		/* [in] */ IUnknown *pDropTarget,
		/* [out] */ IUnknown **ppDropTarget);

	HRESULT STDMETHODCALLTYPE GetExternal( 
		/* [out] */ IDispatch **ppDispatch);

	HRESULT STDMETHODCALLTYPE TranslateUrl( 
		/* [in] */ DWORD dwTranslate,
		/* [in] */ BSTR bstrURLIn,
		/* [out] */ BSTR *pbstrURLOut);

	HRESULT STDMETHODCALLTYPE FilterDataObject( 
		/* [in] */ IUnknown *pDO,
		/* [out] */ IUnknown **ppDORet);
};

OBJECT_ENTRY_AUTO(__uuidof(FelixMemDocUIHandler), CFelixMemDocUIHandler)
