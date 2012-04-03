// ComRecords.cpp : Implementation of CRecords

#include "stdafx.h"
#include "ComRecords.h"
#include "AutomationExceptionHandler.h" // CAutomationExceptionHandler


// CRecords


HRESULT CRecords::set_records( mem_engine::record_collection_type& records )
{
	m_coll.clear() ;

	foreach (mem_engine::record_pointer record, records | boost::adaptors::map_values)
	{
		CComObject< CRecord > *result_object ;
		HRESULT hr = CComObject< CRecord >::CreateInstance( &result_object ) ;
		if ( FAILED( hr ) )
		{
			return hr;
		}
		result_object->set_record(record) ;
		CComPtr< IRecord > result_ptr ;
		hr = result_object->QueryInterface( &result_ptr ) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_coll.push_back( result_ptr ) ;
	}
	return S_OK ;
}