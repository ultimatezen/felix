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

using namespace except ;

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
// trans
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
STDMETHODIMP CRecord::put_Trans(BSTR trans)
{
	try
	{
		m_record->set_trans(BSTR2wstring( trans )) ;
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
STDMETHODIMP CRecord::put_Source(BSTR source)
{
	try
	{
		m_record->set_source(BSTR2wstring( source )) ;
	}
	TA_CATCH( "put_Source" ) ;

	return S_OK ;
}
// created
STDMETHODIMP CRecord::get_CreatedBy(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring source = m_record->get_creator() ;
		*pVal = ::SysAllocStringLen( source.c_str(), source.size() ) ;
	}
	TA_CATCH( "get_CreatedBy" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_CreatedBy(BSTR created_by)
{
	try
	{
		m_record->set_creator(BSTR2wstring( created_by )) ;
	}
	TA_CATCH( "put_CreatedBy" ) ;

	return S_OK ;
}
// modified
STDMETHODIMP CRecord::get_ModifiedBy(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring source = m_record->get_modified_by() ;
		*pVal = ::SysAllocStringLen( source.c_str(), source.size() ) ;
	}
	TA_CATCH( "get_ModifiedBy" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_ModifiedBy(BSTR modified_by)
{
	try
	{
		m_record->set_modified_by(BSTR2wstring( modified_by )) ;
	}
	TA_CATCH( "put_ModifiedBy" ) ;

	return S_OK ;
}
// context
STDMETHODIMP CRecord::get_Context(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring context = m_record->get_context_rich() ;
		*pVal = ::SysAllocStringLen( context.c_str(), context.size() ) ;
	}
	TA_CATCH( "get_Trans" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Context(BSTR context)
{
	try
	{
		m_record->set_context(BSTR2wstring( context )) ;
	}
	TA_CATCH( "put_Trans" ) ;

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
