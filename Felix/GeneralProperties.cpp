// GeneralProperties.cpp : Implementation of CGeneralProperties
#include "StdAfx.h"
#include "Felix.h"
#include "GeneralProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGeneralProperties

STDMETHODIMP CGeneralProperties::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IGeneralProperties ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IGeneralProperties ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IGeneralProperties ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IGeneralProperties ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE ;
}

STDMETHODIMP CGeneralProperties::get_Language(short *pVal)
{
	*pVal = get_mainframe().get_current_gui_language() ;
	return S_OK;
}

STDMETHODIMP CGeneralProperties::put_Language(short newVal)
{
	get_mainframe().SetUILanguage( newVal ) ;
	return S_OK;
}
