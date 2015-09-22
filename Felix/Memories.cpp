// Memories.cpp : Implementation of CMemories

#include "stdafx.h"
#include "Memories.h"
#include "TranslationMemory.h"
#include "logging.h"

// CMemories
using namespace mem_engine;
using namespace except ;

STDMETHODIMP CMemories::Clear()
{
	try
	{
		m_mem_model->clear() ;
		m_coll.clear() ;
	}
	FELIX_AUTO_CATCH( "method Clear in class Memories" ) ;

	return S_OK ;
}

STDMETHODIMP CMemories::Add(IMemory **pVal)
{
	CHECK_OUT_PTR( pVal ) ;

	try
	{
		mem_engine::memory_pointer mem = m_mem_model->add_memory() ;
		COM_ENFORCE(this->add_memory(mem), _T("Failed to add memory")) ;
		this->get_Item(m_coll.size(), pVal); 
	}
	FELIX_AUTO_CATCH( "method Add in class Memories" ) ;

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
		this->get_Item(m_coll.size(), pVal); 
	}
	FELIX_AUTO_CATCH( "method Load in class Memories" ) ;

	return S_OK ;
}
HRESULT CMemories::set_model( model_iface_ptr model )
{
	m_mem_model = model ;

	return load_memories(m_mem_model) ;
}

HRESULT CMemories::load_memories( model_iface_ptr model )
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
		logging::log_error("CMemories::add_memory -- Failed to create COM Memory instance") ;
		logging::log_exception(error) ;
		return hr ;
	}
	result_object->set_memory(mem) ;
	CComPtr<IMemory> result_ptr ;
	hr = result_object->QueryInterface(&result_ptr) ;
	if (FAILED(hr))
	{
		except::CComException error(_T("QueryInterface on COM Memory instance failed"), hr) ;
		logging::log_error("CMemories::add_memory -- Failed to create COM Memory instance") ;
		logging::log_exception(error) ;
		return hr ;
	}
	m_coll.push_back(result_ptr) ;
	return S_OK ;
}