// ComMemory.cpp : Implementation of CComMemory

#include "stdafx.h"
#include "ComMemory.h"
#include "memory_local.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler

using namespace except ;
using namespace mem_engine;

// current matches
STDMETHODIMP CComMemory::get_Records(IRecords **pVal)
{
	try
	{
		if ( m_records )
		{
			m_records->set_records(m_memory->get_records()) ;
			return m_records->QueryInterface(pVal) ;
		}
	}
	FELIX_AUTO_CATCH("get_Records method in ComMemory class")

	return S_OK ;
}
STDMETHODIMP CComMemory::AddRecord(IRecord *pVal)
{
	try
	{
		m_memory->add_record(comrec2rec(pVal)) ;
	}
	FELIX_AUTO_CATCH("AddRecord method in ComMemory class")

	return S_OK ;
}

STDMETHODIMP CComMemory::RemoveRecord(IRecord *pVal)
{
	try
	{
		m_memory->erase(comrec2rec(pVal)) ;
	}
	FELIX_AUTO_CATCH("RemoveRecord method in ComMemory class")

	return S_OK ;
}

STDMETHODIMP CComMemory::GetSize(ULONG* memSize)
{
	try
	{
		*memSize = m_memory->size() ;
	}
	FELIX_AUTO_CATCH("GetSize method in ComMemory class")

	return S_OK ;
}


STDMETHODIMP CComMemory::Save()
{
	try
	{
		m_memory->save_memory() ;
	}
	FELIX_AUTO_CATCH("Save method in ComMemory class")

	return S_OK ;
}
STDMETHODIMP CComMemory::SaveAs(BSTR location)
{
	try
	{
		m_memory->set_location(CString(location)) ;
		m_memory->save_memory() ;
	}
	FELIX_AUTO_CATCH("SaveAs method in ComMemory class")

	return S_OK ;
}

void CComMemory::set_memory( memory_pointer mem )
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
	FELIX_AUTO_CATCH( "get_Creator method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_Creator(BSTR creator)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_creator(BSTR2wstring( creator )) ;
	}
	FELIX_AUTO_CATCH( "put_Creator method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_ModifiedBy method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_ModifiedBy(BSTR modified_by)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_modified_by(BSTR2wstring( modified_by )) ;
	}
	FELIX_AUTO_CATCH( "put_ModifiedBy method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_Field method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_Field(BSTR modified_by)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_field(BSTR2wstring( modified_by )) ;
	}
	FELIX_AUTO_CATCH( "put_Field method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_Client method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_Client(BSTR client)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_client(BSTR2wstring( client )) ;
	}
	FELIX_AUTO_CATCH( "put_Client method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_SourceLanguage method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_SourceLanguage(BSTR language)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_source_language(BSTR2wstring( language )) ;
	}
	FELIX_AUTO_CATCH( "put_SourceLanguage method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_TargetLanguage method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_TargetLanguage(BSTR language)
{
	try
	{
		MemoryInfo *info = m_memory->get_memory_info() ;
		info->set_target_language(BSTR2wstring( language )) ;
	}
	FELIX_AUTO_CATCH( "put_TargetLanguage method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_CreatedOn method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "put_CreatedOn method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "get_ModifiedOn method in ComMemory class" ) ;

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
	FELIX_AUTO_CATCH( "put_ModifiedOn method in ComMemory class" ) ;

	return S_OK ;
}

CComMemory::CComMemory() :
	m_records(NULL),
	m_props(new app_props::properties)
{
	m_memory = memory_pointer(new memory_local(m_props)) ;

	m_props->read_from_registry() ;
}


	// memory
STDMETHODIMP CComMemory::get_IsMemory(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		if (m_memory->get_is_memory())
		{
			*pVal = VARIANT_TRUE ;
		}
		else
		{
			*pVal = VARIANT_FALSE ;
		}
	}
	FELIX_AUTO_CATCH( "get_IsMemory method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_IsMemory(VARIANT_BOOL isMemory)
{
	try
	{
		if (isMemory)
		{
			m_memory->set_is_memory(true) ;
		}
		else
		{
			m_memory->set_is_memory(false) ;
		}
	}
	FELIX_AUTO_CATCH( "put_IsMemory method in ComMemory class" ) ;

	return S_OK ;
}

	// locked
STDMETHODIMP CComMemory::get_IsLocked(VARIANT_BOOL* pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		if (m_memory->is_locked())
		{
			*pVal = VARIANT_TRUE ;
		}
		else
		{
			*pVal = VARIANT_FALSE ;
		}
	}
	FELIX_AUTO_CATCH( "get_IsLocked method in ComMemory class" ) ;

	return S_OK ;
}
STDMETHODIMP CComMemory::put_IsLocked(VARIANT_BOOL isLocked)
{
	try
	{
		if (isLocked)
		{
			m_memory->set_locked_on() ;
		}
		else
		{
			m_memory->set_locked_off() ;
		}
	}
	FELIX_AUTO_CATCH( "put_IsLocked method in ComMemory class" ) ;

	return S_OK ;
}

HRESULT CComMemory::FinalConstruct()
{
	HRESULT hr = RecordsObject::CreateInstance( &m_records ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	m_records->AddRef() ;
	return hr;
}

void CComMemory::FinalRelease()
{
	if ( m_records )
	{
		m_records->Release() ;
	}
}
// CComMemory

