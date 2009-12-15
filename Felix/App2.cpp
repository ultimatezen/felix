// App2.cpp : Implementation of CApp2

#include "stdafx.h"
#include "App2.h"
#include "FelixApp.h"
#include "query.h"
#include "TranslationMemory.h"

// To use this macro, derive from CAutomationExceptionHandler
#define TA_CATCH( func_str ) \
	catch( _com_error &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }  \
	catch( CWinException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CComException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CSWException &e )    { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CException &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch ( std::exception &e ) { CString Func( func_str ) ; return handle_exception( e, Func ) ; }

using namespace except ;

// CApp2

STDMETHODIMP CApp2::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IApp2
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CApp2::get_CurrentMatch(ISearchResult **pVal)
{
	try
	{
		if ( m_current_match )
		{
			m_current_match->set_match(app::get_app().get_current_match());
			return m_current_match->QueryInterface(pVal) ;
		}
	}
	TA_CATCH("get_CurrentMatch")
		
	return E_FAIL ;
}
STDMETHODIMP CApp2::put_CurrentMatch(ISearchResult *pVal)
{
	try
	{
		pVal ;
		return S_OK ;
	}
	TA_CATCH("put_CurrentMatch")
	
	return E_FAIL ;
}
STDMETHODIMP CApp2::ReflectChanges(ULONG RecId, BSTR Source, BSTR Trans)
{
	try
	{
		app::get_app().add_by_id(static_cast<size_t>(RecId),
			BSTR2wstring(Source),
			BSTR2wstring(Trans)) ;
	}
	TA_CATCH( "ReflectChanges" ) ;

	return S_OK;
}
STDMETHODIMP CApp2::ReviewTranslation(ULONG RecId, BSTR Source, BSTR Trans)
{
	try
	{
		app::get_app().view_by_id(static_cast<size_t>(RecId),
			BSTR2wstring(Source),
			BSTR2wstring(Trans)) ;
	}
	TA_CATCH( "ReflectChanges" ) ;

	return S_OK;
}

