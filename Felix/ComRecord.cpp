// ComRecord.cpp : Implementation of CRecord

#include "stdafx.h"
#include "ComRecord.h"

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
	FELIX_AUTO_CATCH( "get_Trans" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Trans(BSTR trans)
{
	try
	{
		m_record->set_trans(BSTR2wstring( trans )) ;
	}
	FELIX_AUTO_CATCH( "put_Trans" ) ;

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
	FELIX_AUTO_CATCH( "get_Source" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Source(BSTR source)
{
	try
	{
		m_record->set_source(BSTR2wstring( source )) ;
	}
	FELIX_AUTO_CATCH( "put_Source" ) ;

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
	FELIX_AUTO_CATCH( "get_Trans" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Context(BSTR context)
{
	try
	{
		m_record->set_context(BSTR2wstring( context )) ;
	}
	FELIX_AUTO_CATCH( "put_Trans" ) ;

	return S_OK ;
}


// created by
STDMETHODIMP CRecord::get_CreatedBy(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring source = m_record->get_creator() ;
		*pVal = ::SysAllocStringLen( source.c_str(), source.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_CreatedBy" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_CreatedBy(BSTR created_by)
{
	try
	{
		m_record->set_creator(BSTR2wstring( created_by )) ;
	}
	FELIX_AUTO_CATCH( "put_CreatedBy" ) ;

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
	FELIX_AUTO_CATCH( "get_ModifiedBy" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_ModifiedBy(BSTR modified_by)
{
	try
	{
		m_record->set_modified_by(BSTR2wstring( modified_by )) ;
	}
	FELIX_AUTO_CATCH( "put_ModifiedBy" ) ;

	return S_OK ;
}

// created
STDMETHODIMP CRecord::get_DateCreated(DATE* pVal)
{
	CHECK_OUT_PTR( pVal ) ;
	try
	{
		misc_wrappers::date created = m_record->get_created() ;
		::SystemTimeToVariantTime(&created, pVal) ;
	}
	FELIX_AUTO_CATCH( "get_DateCreated" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_DateCreated(DATE pVal)
{
	try
	{
		misc_wrappers::date created ;
		::VariantTimeToSystemTime(pVal, &created) ;
		m_record->set_created(created) ;
	}
	FELIX_AUTO_CATCH( "put_DateCreated" ) ;

	return S_OK ;
}

// modified
STDMETHODIMP CRecord::get_LastModified(DATE* pVal)
{
	CHECK_OUT_PTR( pVal ) ;
	try
	{
		misc_wrappers::date modified = m_record->get_modified() ;
		::SystemTimeToVariantTime(&modified, pVal) ;
	}
	FELIX_AUTO_CATCH( "get_LastModified" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_LastModified(DATE pVal)
{
	try
	{
		misc_wrappers::date modified ;
		::VariantTimeToSystemTime(pVal, &modified) ;
		m_record->set_modified(modified) ;
	}
	FELIX_AUTO_CATCH( "put_LastModified" ) ;

	return S_OK ;
}

// refcount
STDMETHODIMP CRecord::get_RefCount(ULONG* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		*pVal = m_record->get_refcount() ;
	}
	FELIX_AUTO_CATCH( "get_Id" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_RefCount(ULONG pVal)
{
	try
	{
		m_record->set_refcount(pVal) ;
	}
	FELIX_AUTO_CATCH( "put_Id" ) ;

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
	FELIX_AUTO_CATCH( "get_Id" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Id(ULONG pVal)
{
	try
	{
		m_record->set_id(pVal) ;
	}
	FELIX_AUTO_CATCH( "put_Id" ) ;

	return S_OK ;
}

// trans
STDMETHODIMP CRecord::get_PlainTrans(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring trans = m_record->get_trans_plain() ;
		*pVal = ::SysAllocStringLen( trans.c_str(), trans.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_PlainTrans" ) ;

	return S_OK ;
}

// source
STDMETHODIMP CRecord::get_PlainSource(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring source = m_record->get_source_plain() ;
		*pVal = ::SysAllocStringLen( source.c_str(), source.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_PlainSource" ) ;

	return S_OK ;
}

// context
STDMETHODIMP CRecord::get_PlainContext(BSTR* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		wstring context = m_record->get_context_plain() ;
		*pVal = ::SysAllocStringLen( context.c_str(), context.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_PlainContext" ) ;

	return S_OK ;
}


