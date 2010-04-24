// FelixMemDocUIHandler.cpp : Implementation of CFelixMemDocUIHandler

#include "stdafx.h"
#include "FelixMemDocUIHandler.h"


// CFelixMemDocUIHandler


HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::ShowContextMenu( /* [in] */ DWORD /*dwID*/, 
																 /* [in] */ DWORD /*x*/, 
																 /* [in] */ DWORD /*y*/, 
																 /* [in] */ IUnknown * /*pcmdtReserved*/, 
																 /* [in] */ IDispatch * /*pdispReserved*/, 
																 /* [retval][out] */ HRESULT *dwRetVal )
{
	SENSE("ShowContextMenu") ;
	*dwRetVal = S_OK;      //This is what the WebBrowser control is looking for.
	return this->m_get_menu() ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::GetHostInfo( /* [out][in] */ DWORD *pdwFlags, 
															 /* [out][in] */ DWORD * /*pdwDoubleClick*/ )
{
	SENSE("GetHostInfo") ;
	const DWORD _DOCHOSTUIFLAG_THEME = 0x40000;
	*pdwFlags = DOCHOSTUIFLAG_NO3DBORDER | _DOCHOSTUIFLAG_THEME;
	return S_OK ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::ShowUI( /* [in] */ DWORD /*dwID*/, 
														/* [in] */ IUnknown * /*pActiveObject*/, 
														/* [in] */ IUnknown * /*pCommandTarget*/, 
														/* [in] */ IUnknown * /*pFrame*/, 
														/* [in] */ IUnknown * /*pDoc*/, 
														/* [retval][out] */ HRESULT * /*dwRetVal */)
{
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::HideUI( void )
{
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::UpdateUI( void )
{
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::EnableModeless( /* [in] */ VARIANT_BOOL /*fEnable*/ )
{
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::OnDocWindowActivate( /* [in] */ VARIANT_BOOL /*fActivate*/ )
{
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::OnFrameWindowActivate( /* [in] */ VARIANT_BOOL /*fActivate*/ )
{
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::ResizeBorder( /* [in] */ long /*left*/, 
															  /* [in] */ long /*top*/, 
															  /* [in] */ long /*right*/, 
															  /* [in] */ long /*bottom*/, 
															  /* [in] */ IUnknown * /*pUIWindow*/, 
															  /* [in] */ VARIANT_BOOL /*fFrameWindow*/ )
{
	SENSE("ResizeBorder") ;
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::TranslateAccelerator( /* [in] */ DWORD_PTR /*hWnd*/, 
																	  /* [in] */ DWORD /*nMessage*/, 
																	  /* [in] */ DWORD_PTR /*wParam*/, 
																	  /* [in] */ DWORD_PTR /*lParam*/, 
																	  /* [in] */ BSTR /*bstrGuidCmdGroup*/, 
																	  /* [in] */ DWORD /*nCmdID*/, 
																	  /* [retval][out] */ HRESULT * /*dwRetVal*/ )
{
	SENSE("TranslateAccelerator") ;
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::GetOptionKeyPath( /* [out] */ BSTR * /*pbstrKey*/, 
																  /* [in] */ DWORD /*dw*/ )
{
	SENSE("GetOptionKeyPath") ;
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::GetDropTarget( /* [in] */ IUnknown * /*pDropTarget*/, 
															   /* [out] */ IUnknown ** /*ppDropTarget*/ )
{
	SENSE("GetDropTarget") ;
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::GetExternal( /* [out] */ IDispatch ** /*ppDispatch*/ )
{
	SENSE("GetExternal") ;
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::TranslateUrl( /* [in] */ DWORD /*dwTranslate*/, 
															  /* [in] */ BSTR /*bstrURLIn*/, 
															  /* [out] */ BSTR * /*pbstrURLOut */)
{
	SENSE("TranslateUrl") ;
	return E_NOTIMPL ;
}

HRESULT STDMETHODCALLTYPE CFelixMemDocUIHandler::FilterDataObject( /* [in] */ IUnknown * /*pDO*/, 
																  /* [out] */ IUnknown ** /*ppDORet*/ )
{
	SENSE("FilterDataObject") ;
	return E_NOTIMPL ;
}
