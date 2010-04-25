// SearchResult.cpp : Implementation of CSearchResult

#include "StdAfx.h"
#include "SearchResult.h"

// To use this macro, derive from CAutomationExceptionHandler
#define FELIX_AUTO_CATCH( func_str ) \
	catch( _com_error &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }  \
	catch( CWinException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CComException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CSWException &e )    { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CException &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch ( std::exception &e ) { CString Func( func_str ) ; return handle_exception( e, Func ) ; }

using namespace except ;

STDMETHODIMP CSearchResult::get_Record(IRecord **pVal)
{
	try
	{
		if ( m_record )
		{
			return m_record->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_Record")

	return E_FAIL ;
}
STDMETHODIMP CSearchResult::put_Record(IRecord *pVal)
{
	try
	{
		pVal ;
	}
	FELIX_AUTO_CATCH("put_Record")

	return S_OK ;
}
STDMETHODIMP CSearchResult::get_Score(DOUBLE* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_match->get_score() ;
	}
	FELIX_AUTO_CATCH( "get_Score" ) ;

	return S_OK;
}
STDMETHODIMP CSearchResult::get_MemoryName(BSTR* pVal)
{
	try
	{
		const wstring memory_location = m_match->get_memory_location() ;
		*pVal = ::SysAllocStringLen( memory_location.c_str(), memory_location.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_MemoryName" ) ;
	return S_OK;
}


// CSearchResult

