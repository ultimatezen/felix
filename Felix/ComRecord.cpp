// ComRecord.cpp : Implementation of CRecord

#include "stdafx.h"
#include "ComRecord.h"
#include "record_local.h"

using namespace except ;

mem_engine::record_pointer comrec2rec(CComPtr<IRecord> comrec)
{
	mem_engine::record_pointer rec(new mem_engine::record_local) ;

	// source
	CComBSTR source ;
	comrec->get_Source(&source) ;
	rec->set_source(BSTR2wstring(source)) ;

	// trans
	CComBSTR trans ;
	comrec->get_Trans(&trans) ;
	rec->set_trans(BSTR2wstring(trans)) ;

	// context
	CComBSTR context ;
	comrec->get_Context(&context) ;
	rec->set_context(BSTR2wstring(context)) ;

	// validated
	VARIANT_BOOL validated ;
	comrec->get_Validated(&validated) ;
	rec->set_validated(validated != VARIANT_FALSE) ;

	// reliability
	ULONG reliability ;
	comrec->get_Reliability(&reliability) ;
	rec->set_reliability(reliability) ;

	// refcount
	ULONG refcount ;
	comrec->get_RefCount(&refcount) ;
	rec->set_refcount(refcount) ;

	// id
	ULONG recid ;
	comrec->get_Id(&recid) ;
	rec->set_id(recid) ;

	// created
	CComBSTR created_by ;
	comrec->get_CreatedBy(&created_by) ;
	rec->set_creator(BSTR2wstring(created_by)) ;

	DATE date_created ;
	comrec->get_DateCreated(&date_created) ;
	misc_wrappers::date created ;
	::VariantTimeToSystemTime(date_created, &created) ;
	rec->set_created(created) ;

	// modified
	CComBSTR modified_by ;
	comrec->get_ModifiedBy(&modified_by) ;
	rec->set_modified_by(BSTR2wstring(modified_by)) ;

	DATE date_modified ;
	comrec->get_LastModified(&date_modified) ;
	misc_wrappers::date modified ;
	::VariantTimeToSystemTime(date_modified, &modified) ;
	rec->set_modified(modified) ;

	return rec ;
}

STDMETHODIMP CRecord::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IRecord
	};

	return supports_error_info(arr, riid) ;
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
	FELIX_AUTO_CATCH( "get_Context" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Context(BSTR context)
{
	try
	{
		m_record->set_context(BSTR2wstring( context )) ;
	}
	FELIX_AUTO_CATCH( "put_Context" ) ;

	return S_OK ;
}

// validated
STDMETHODIMP CRecord::get_Validated(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		if (m_record->is_validated())
		{
			*pVal = VARIANT_TRUE ;
		}
		else
		{
			*pVal = VARIANT_FALSE ;
		}
	}
	FELIX_AUTO_CATCH( "get_Validated" ) ;

	return S_OK ;
}

STDMETHODIMP CRecord::put_Validated(VARIANT_BOOL validated)
{
	try
	{
		if (validated)
		{
			m_record->set_validated_on() ;
		}
		else
		{
			m_record->set_validated_off() ;
		}
	}
	FELIX_AUTO_CATCH( "put_Validated" ) ;

	return S_OK ;
}

// reliability
STDMETHODIMP CRecord::get_Reliability(ULONG* reliability)
{
	CHECK_OUT_PTR( reliability ) ;

	try
	{
		*reliability = m_record->get_reliability() ;
	}
	FELIX_AUTO_CATCH( "get_Reliability" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_Reliability(ULONG reliability)
{
	try
	{
		m_record->set_reliability(reliability) ;
	}
	FELIX_AUTO_CATCH( "put_Reliability" ) ;

	return S_OK ;
}

// created by
STDMETHODIMP CRecord::get_CreatedBy(BSTR* created_by)
{
	CHECK_OUT_PTR( created_by ) ;

	try
	{
		wstring source = m_record->get_creator() ;
		*created_by = ::SysAllocStringLen( source.c_str(), source.size() ) ;
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
STDMETHODIMP CRecord::get_ModifiedBy(BSTR* modified_by)
{
	CHECK_OUT_PTR( modified_by ) ;

	try
	{
		wstring source = m_record->get_modified_by() ;
		*modified_by = ::SysAllocStringLen( source.c_str(), source.size() ) ;
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
STDMETHODIMP CRecord::get_DateCreated(DATE* date_created)
{
	CHECK_OUT_PTR( date_created ) ;
	try
	{
		misc_wrappers::date created = m_record->get_created() ;
		::SystemTimeToVariantTime(&created, date_created) ;
	}
	FELIX_AUTO_CATCH( "get_DateCreated" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_DateCreated(DATE date_created)
{
	try
	{
		misc_wrappers::date created ;
		::VariantTimeToSystemTime(date_created, &created) ;
		m_record->set_created(created) ;
	}
	FELIX_AUTO_CATCH( "put_DateCreated" ) ;

	return S_OK ;
}

// modified
STDMETHODIMP CRecord::get_LastModified(DATE* last_modified)
{
	CHECK_OUT_PTR( last_modified ) ;
	try
	{
		misc_wrappers::date modified = m_record->get_modified() ;
		::SystemTimeToVariantTime(&modified, last_modified) ;
	}
	FELIX_AUTO_CATCH( "get_LastModified" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_LastModified(DATE last_modified)
{
	try
	{
		misc_wrappers::date modified ;
		::VariantTimeToSystemTime(last_modified, &modified) ;
		m_record->set_modified(modified) ;
	}
	FELIX_AUTO_CATCH( "put_LastModified" ) ;

	return S_OK ;
}

// refcount
STDMETHODIMP CRecord::get_RefCount(ULONG* refcount)
{
	CHECK_OUT_PTR( refcount ) ;

	try
	{
		*refcount = m_record->get_refcount() ;
	}
	FELIX_AUTO_CATCH( "get_RefCount" ) ;

	return S_OK ;
}
STDMETHODIMP CRecord::put_RefCount(ULONG refcount)
{
	try
	{
		m_record->set_refcount(refcount) ;
	}
	FELIX_AUTO_CATCH( "put_RefCount" ) ;

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

//////////////////////////////////////////////////////////////////////////
// plain values (stripped of tags)
//////////////////////////////////////////////////////////////////////////

// trans
STDMETHODIMP CRecord::get_PlainTrans(BSTR* plain_trans)
{
	CHECK_OUT_PTR( plain_trans ) ;

	try
	{
		wstring trans = m_record->get_trans_plain() ;
		*plain_trans = ::SysAllocStringLen( trans.c_str(), trans.size() ) ;
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

void CRecord::set_record( mem_engine::record_pointer record )
{
	m_record = record ;
}

HRESULT CRecord::FinalConstruct()
{
	return S_OK;
}

void CRecord::FinalRelease()
{

}