// Memories.cpp : Implementation of CMemories

#include "stdafx.h"
#include "Memories.h"
#include "TranslationMemory.h"
#include "logging.h"

// CMemories
using namespace mem_engine;
using namespace except ;

STDMETHODIMP CMemories::Add(IMemory **pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		mem_engine::memory_pointer mem = m_mem_model->add_memory() ;
		COM_ENFORCE(this->add_memory(mem), _T("Failed to add memory")) ;
		size_t i = m_coll.size() -1 ;
		this->get_Item(i, pVal); 
	}
	FELIX_AUTO_CATCH( "Add" ) ;

	return S_OK ;
}
STDMETHODIMP CMemories::Load(BSTR location, IMemory **pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		mem_engine::memory_pointer mem = m_mem_model->add_memory() ;
		COM_ENFORCE(this->add_memory(mem), _T("Failed to add memory")) ;
		mem->load(CString(location)) ;
		size_t i = m_coll.size() -1 ;
		this->get_Item(i, pVal); 
	}
	FELIX_AUTO_CATCH( "Add" ) ;

	return S_OK ;
}
HRESULT CMemories::set_model( FelixModelInterface *model )
{
	m_mem_model = model ;

	return load_memories(m_mem_model) ;
}

HRESULT CMemories::load_memories( FelixModelInterface *model )
{
	m_coll.clear() ;

	for (size_t i = 0 ; i < model->size() ; ++i)
	{
		HRESULT hr = add_memory(model->memory_at(i));
		if (FAILED(hr))
		{
			return hr ;
		}

	}
	return S_OK ;
}

HRESULT CMemories::add_memory( memory_pointer mem )
{
	CComObject< CComMemory > *result_object; 
	HRESULT hr = CComObject<CComMemory>::CreateInstance(&result_object) ;
	if (FAILED(hr))
	{
		except::CComException error(_T("Failed to create COM Memory instance"), hr) ;
		logging::log_error("CMemories::load_records -- Failed to create COM Memory instance") ;
		logging::log_exception(error) ;
		return hr ;
	}
	result_object->set_memory(mem) ;
	CComPtr<IMemory> result_ptr ;
	hr = result_object->QueryInterface(&result_ptr) ;
	if (FAILED(hr))
	{
		except::CComException error(_T("Failed to create COM Memory instance"), hr) ;
		logging::log_error("CMemories::load_records -- Failed to create COM Memory instance") ;
		logging::log_exception(error) ;
		return hr ;
	}
	m_coll.push_back(result_ptr) ;
	return S_OK ;
}