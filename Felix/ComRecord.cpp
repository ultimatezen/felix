// ComRecord.cpp : Implementation of CRecord

#include "stdafx.h"
#include "ComRecord.h"


// To use this macro, derive from CAutomationExceptionHandler
#define TA_CATCH( func_str ) \
	catch( _com_error &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }  \
	catch( CWinException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CComException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CSWException &e )    { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( CException &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch ( std::exception &e ) { CString Func( func_str ) ; return handle_exception( e, Func ) ; }

// CRecord

STDMETHODIMP CRecord::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IRecord
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CRecord::get_Trans(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring trans = m_record->get_trans_rich() ;
		*pVal = ::SysAllocStringLen( trans.c_str(), trans.size() ) ;
	}
	TA_CATCH( "get_Trans" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Trans(BSTR pVal)
{
	try
	{
		m_record->set_trans(BSTR2wstring( pVal )) ;
	}
	TA_CATCH( "put_Trans" ) ;

	return S_OK ;
}
// source
STDMETHODIMP CRecord::get_Source(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring source = m_record->get_source_rich() ;
		*pVal = ::SysAllocStringLen( source.c_str(), source.size() ) ;
	}
	TA_CATCH( "get_Source" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Source(BSTR pVal)
{
	try
	{
		m_record->set_source(BSTR2wstring( pVal )) ;
	}
	TA_CATCH( "put_Source" ) ;

	return S_OK ;
}
// id
STDMETHODIMP CRecord::get_Id(ULONG* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_record->get_id() ;
	}
	TA_CATCH( "get_Id" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Id(ULONG pVal)
{
	try
	{
		m_record->set_id(pVal) ;
	}
	TA_CATCH( "put_Id" ) ;

	return S_OK ;
}
