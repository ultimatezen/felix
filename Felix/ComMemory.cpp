// ComMemory.cpp : Implementation of CComMemory

#include "stdafx.h"
#include "ComMemory.h"

using namespace except ;

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

void CComMemory::set_memory( mem_engine::memory_pointer mem )
{
	m_memory = mem ;
}
// CComMemory

