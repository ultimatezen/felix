// ComMemory.cpp : Implementation of CComMemory

#include "stdafx.h"
#include "ComMemory.h"

using namespace except ;
using namespace mem_engine;

// current matches
STDMETHODIMP CComMemory::get_Records(IRecords **pVal)
{
	try
	{
		if ( m_records )
		{
			m_records->set_records(this->m_memory->get_records()) ;
			return m_records->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_Records")

	return E_FAIL ;
}
STDMETHODIMP CComMemory::AddRecord(IRecord *pVal)
{
	try
	{
		m_memory->add_record(comrec2rec(pVal)) ;
	}
	FELIX_AUTO_CATCH("AddRecord")

	return E_FAIL ;
}
STDMETHODIMP CComMemory::Save()
{
	try
	{
		m_memory->save_memory() ;
	}
	FELIX_AUTO_CATCH("Save")

	return E_FAIL ;
}
STDMETHODIMP CComMemory::SaveAs(BSTR location)
{
	try
	{
		m_memory->set_location(CString(location)) ;
		m_memory->save_memory() ;
	}
	FELIX_AUTO_CATCH("SaveAs")

	return E_FAIL ;
}

void CComMemory::set_memory( mem_engine::memory_pointer mem )
{
	m_memory = mem ;
}

// creator
STDMETHODIMP CComMemory::get_Creator(BSTR* creator)
{
	CHECK_OUT_PTR( creator ) ;

	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		wstring wcreator = info->get_creator() ;
		*creator = ::SysAllocStringLen( wcreator.c_str(), wcreator.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_Creator" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_Creator(BSTR creator)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_creator(BSTR2wstring( creator )) ;
	}
	FELIX_AUTO_CATCH( "put_Creator" ) ;

	return S_OK ;
}
// modified_by
STDMETHODIMP CComMemory::get_ModifiedBy(BSTR* modified_by)
{
	CHECK_OUT_PTR( modified_by ) ;

	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		wstring val = info->get_modified_by() ;
		*modified_by = ::SysAllocStringLen( val.c_str(), val.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_ModifiedBy" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_ModifiedBy(BSTR modified_by)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_modified_by(BSTR2wstring( modified_by )) ;
	}
	FELIX_AUTO_CATCH( "put_ModifiedBy" ) ;

	return S_OK ;
}
// field
STDMETHODIMP CComMemory::get_Field(BSTR* creator)
{
	CHECK_OUT_PTR( creator ) ;

	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		wstring val = info->get_field() ;
		*creator = ::SysAllocStringLen( val.c_str(), val.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_Field" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_Field(BSTR modified_by)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_field(BSTR2wstring( modified_by )) ;
	}
	FELIX_AUTO_CATCH( "put_Field" ) ;

	return S_OK ;
}
// client
STDMETHODIMP CComMemory::get_Client(BSTR* client)
{
	CHECK_OUT_PTR( client ) ;

	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		wstring val = info->get_client() ;
		*client = ::SysAllocStringLen( val.c_str(), val.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_Client" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_Client(BSTR client)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_client(BSTR2wstring( client )) ;
	}
	FELIX_AUTO_CATCH( "put_Client" ) ;

	return S_OK ;
}
// source_lang
STDMETHODIMP CComMemory::get_SourceLanguage(BSTR* language)
{
	CHECK_OUT_PTR( language ) ;

	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		wstring val = info->get_source_language() ;
		*language = ::SysAllocStringLen( val.c_str(), val.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_SourceLanguage" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_SourceLanguage(BSTR language)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_source_language(BSTR2wstring( language )) ;
	}
	FELIX_AUTO_CATCH( "put_SourceLanguage" ) ;

	return S_OK ;
}
// target_lang
STDMETHODIMP CComMemory::get_TargetLanguage(BSTR* language)
{
	CHECK_OUT_PTR( language ) ;

	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		wstring val = info->get_target_language() ;
		*language = ::SysAllocStringLen( val.c_str(), val.size() ) ;
	}
	FELIX_AUTO_CATCH( "get_TargetLanguage" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_TargetLanguage(BSTR language)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_target_language(BSTR2wstring( language )) ;
	}
	FELIX_AUTO_CATCH( "put_TargetLanguage" ) ;

	return S_OK ;
}
// created_on
STDMETHODIMP CComMemory::get_CreatedOn(DATE* date_created)
{
	CHECK_OUT_PTR( date_created ) ;
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		misc_wrappers::date created(info->get_created_on()) ;
		::SystemTimeToVariantTime(&created, date_created) ;
	}
	FELIX_AUTO_CATCH( "get_CreatedOn" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_CreatedOn(DATE date_created)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		misc_wrappers::date created ;
		::VariantTimeToSystemTime(date_created, &created) ;
		info->set_created_on(created.get_date_time_string()) ;
	}
	FELIX_AUTO_CATCH( "put_CreatedOn" ) ;

	return S_OK ;
}

// ModifiedOn
STDMETHODIMP CComMemory::get_ModifiedOn(DATE* date_modified)
{
	CHECK_OUT_PTR( date_modified ) ;
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		misc_wrappers::date modified(info->get_modified_on()) ;
		::SystemTimeToVariantTime(&modified, date_modified) ;
	}
	FELIX_AUTO_CATCH( "get_ModifiedOn" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_ModifiedOn(DATE date_modified)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		misc_wrappers::date modified ;
		::VariantTimeToSystemTime(date_modified, &modified) ;
		info->set_modified_on(modified.get_date_time_string()) ;
	}
	FELIX_AUTO_CATCH( "put_ModifiedOn" ) ;

	return S_OK ;
}

// CComMemory

