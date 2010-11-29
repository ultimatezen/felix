
#pragma once

class ATL_NO_VTABLE _IDocHostUIHandlerDispatch
{
public:
	//this method needs a different name than QueryInterface
	STDMETHOD(_LocDHUHQueryInterface)(REFIID riid, void ** ppvObject) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
};

class ATL_NO_VTABLE _IDocHostUIHandlerDispatchLocator : public _IDocHostUIHandlerDispatch
{
public:
};

class ATL_NO_VTABLE IDocHostUIHandlerDispatchImpl : public _IDocHostUIHandlerDispatchLocator
{
public:
// COM identity
	STDMETHOD(_LocDHUHQueryInterface)(REFIID riid, void ** ppvObject)
	{
		if (InlineIsEqualUnknown(riid) ||
			InlineIsEqualGUID(riid, IID_IDocHostUIHandlerDispatch))
		{
			if (ppvObject == NULL)
				return E_POINTER;
			*ppvObject = this;
			AddRef();
#ifdef _ATL_DEBUG_INTERFACES
			_Module.AddThunk((IUnknown**)ppvObject, _T("IDocHostUIHandlerDispatchImpl"), riid);
#endif // _ATL_DEBUG_INTERFACES
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}
	 ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}
	 ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

// IDispatch
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
	{return E_NOTIMPL;}

	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
	{return E_NOTIMPL;}

	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid)
	{return E_NOTIMPL;}

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD /*wFlags*/, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* /*pexcepinfo*/, UINT* /*puArgErr*/)
	{return E_NOTIMPL;}


// IDocHostUIHandlerDispatch
	STDMETHOD(ShowContextMenu)(
			/* [in] */ DWORD dwID,
            /* [in] */ DWORD x,
            /* [in] */ DWORD y,
            /* [in] */ IUnknown  *pcmdtReserved,
            /* [in] */ IDispatch  *pdispReserved,
            /* [retval][out] */ HRESULT  *dwRetVal)
	{return E_NOTIMPL;}
        
	STDMETHOD(GetHostInfo)( 
            /* [out][in] */ DWORD  *pdwFlags,
            /* [out][in] */ DWORD  *pdwDoubleClick)
	{return E_NOTIMPL;}
        
    STDMETHOD(SShowUI)( 
            /* [in] */ DWORD dwID,
            /* [in] */ IUnknown  *pActiveObject,
            /* [in] */ IUnknown  *pCommandTarget,
            /* [in] */ IUnknown  *pFrame,
            /* [in] */ IUnknown  *pDoc,
            /* [retval][out] */ HRESULT  *dwRetVal)
	{return E_NOTIMPL;}
        
    STDMETHOD(HideUI)(void)
	{return E_NOTIMPL;}
    STDMETHOD(UpdateUI)(void)
	{return E_NOTIMPL;}    
	STDMETHOD(EnableModeless)( 
           /* [in] */ VARIANT_BOOL fEnable)
	{return E_NOTIMPL;}   
    STDMETHOD(OnDocWindowActivate)( 
            /* [in] */ VARIANT_BOOL fActivate)
 	{return E_NOTIMPL;}       
    STDMETHOD(OnFrameWindowActivate)( 
            /* [in] */ VARIANT_BOOL fActivate)
   	{return E_NOTIMPL;}     
    STDMETHOD(ResizeBorder)( 
            /* [in] */ long left,
            /* [in] */ long top,
            /* [in] */ long right,
            /* [in] */ long bottom,
            /* [in] */ IUnknown  *pUIWindow,
            /* [in] */ VARIANT_BOOL fFrameWindow)
  	{return E_NOTIMPL;}      
    STDMETHOD(TranslateAccelerator)( 
            /* [in] */ DWORD hWnd,
            /* [in] */ DWORD nMessage,
            /* [in] */ DWORD wParam,
            /* [in] */ DWORD lParam,
            /* [in] */ BSTR bstrGuidCmdGroup,
            /* [in] */ DWORD nCmdID,
            /* [retval][out] */ HRESULT  *dwRetVal)
  	{return E_NOTIMPL;}      
     STDMETHOD(GetOptionKeyPath)( 
            /* [out] */ BSTR  *pbstrKey,
            /* [in] */ DWORD dw)
  	{return E_NOTIMPL;}
     STDMETHOD(GetDropTarget)( 
            /* [in] */ IUnknown  *pDropTarget,
            /* [out] */ IUnknown  **ppDropTarget)
    {return E_NOTIMPL;}    
	STDMETHOD(GetExternal)( 
            /* [out] */ IDispatch  **ppDispatch)
    {return E_NOTIMPL;} 
    STDMETHOD(TranslateUrl)( 
            /* [in] */ DWORD dwTranslate,
            /* [in] */ BSTR bstrURLIn,
            /* [out] */ BSTR  *pbstrURLOut)
	{return E_NOTIMPL;}  
    STDMETHOD(FilterDataObject)( 
            /* [in] */ IUnknown  *pDO,
            /* [out] */ IUnknown  **ppDORet)
	{return E_NOTIMPL;}

// Methods
	bool InstallExternalUIHandler(IAxWinHostWindow* pAxWinHostWindow)
	{
		HRESULT hr = pAxWinHostWindow->SetExternalUIHandler((IDocHostUIHandlerDispatch*)this);
		if (FAILED(hr))
			return false;

		return true;
	}

	bool UninstallExternalUIHandler(IAxWinHostWindow* pAxWinHostWindow)
	{
		HRESULT hr = pAxWinHostWindow->SetExternalUIHandler(NULL);
		if (FAILED(hr))
			return false;

		return true;
	}
};