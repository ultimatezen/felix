// Memories.cpp : Implementation of CMemories
#include "StdAfx.h"
#include "Felix.h"
#include "Memories.h"

#include "mainfrm.h"

#include "MemoryIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


struct _CopyVariantFromMemory
{
	static HRESULT copy( VARIANT *lhs, const memory_pointer *rhs )
	{
		ATLASSERT( IsValidWritePointer( lhs ) ) ;
		CComObject< CMemory >* the_memory = 0 ;
		
		HRESULT hr = CComObject< CMemory >::CreateInstance( &the_memory ) ;
		ASSERT_HRESULT( hr ) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		
		the_memory->AddRef() ;
		
		the_memory->set_memory( *rhs ) ;
		
		if( SUCCEEDED( hr ) )
		{
			hr = the_memory->QueryInterface( &lhs->punkVal ) ;
			lhs->vt = VT_UNKNOWN ;
			ASSERT_HRESULT( hr ) ;
		}
		
		the_memory->Release() ;
		
		return hr ;
		
	}
	
	static void init( VARIANT *p ) 
	{
		ATLASSERT( IsValidWritePointer( p ) ) ;
		
		// return type is VOID
		VariantInit( p ) ; 
	}
	static void destroy( VARIANT *p ) 
	{ 
		ATLASSERT( IsValidWritePointer( p ) ) ;
		
		_HR0( VariantClear( p ) ) ;
	}
};

typedef CComEnumOnSTL< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
_CopyVariantFromMemory, memory_list > MemoryEnum ;


/////////////////////////////////////////////////////////////////////////////
// CMemories

STDMETHODIMP CMemories::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (
		( (PLONG) &IID_IMemories ) [0] == ( (PLONG) &riid ) [0] &&
		( (PLONG) &IID_IMemories ) [1] == ( (PLONG) &riid ) [1] &&
		( (PLONG) &IID_IMemories ) [2] == ( (PLONG) &riid ) [2] &&
		( (PLONG) &IID_IMemories ) [3] == ( (PLONG) &riid ) [3] )
		return S_OK ;
	
	return S_FALSE;
}

STDMETHODIMP CMemories::get_Count(long *pVal)
{
	ATLASSERT( IsValidWritePointer( pVal ) ) ;
	*pVal = get_memories().size() ;

	return S_OK;
}

STDMETHODIMP CMemories::get_Item(long index, IMemory **pVal)
{
	if ( index < 1 || static_cast< size_t >( index ) > get_memories().size() )
		return E_INVALIDARG ;
	
	CComObject< CMemory >* the_memory = 0 ;
	
	HRESULT hr = CComObject< CMemory >::CreateInstance( &the_memory ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_memory->AddRef() ;
	
	memory_iterator pos = get_memories().begin() ;
	
	std::advance( pos, index-1 ) ;
	
	the_memory->set_memory( *pos ) ;
	
	if( SUCCEEDED( hr ) )
	{
		hr = the_memory->QueryInterface( pVal ) ;
	}
	
	the_memory->Release() ;
	
	return hr ;
	
}

STDMETHODIMP CMemories::get__NewEnum(LPUNKNOWN *pVal)
{
	*pVal = 0 ;
	
	CComObject< MemoryEnum >* pe = 0 ;
	
	
	HRESULT hr = CComObject< MemoryEnum >::CreateInstance( &pe ) ;
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	pe->AddRef() ;
	
	pe->Init( this->GetUnknown(), get_memories() ) ;
	
	if( SUCCEEDED( hr ) )
	{
		hr = pe->QueryInterface( pVal ) ;
	}
	
	pe->Release() ;
	
	return hr ;
}

STDMETHODIMP CMemories::Remove(VARIANT item)
{
	USES_CONVERSION ;

	if ( item.vt == VT_UNKNOWN )
	{
		CComQIPtr< IMemory > mem( item.punkVal ) ;
		
		CComBSTR name ;
		mem->get_Name( &name ) ;

		CString loc( W2T( name ) ) ;

		remove_memory_by_name( loc ) ;
		
		return S_OK;
	}
	else if ( item.vt == VT_BSTR )
	{
		CString loc(  W2T( item.bstrVal ) ) ;

		remove_memory_by_name( loc ) ;

		return S_OK;
	}
	
	if ( item.lVal < 1 || static_cast< size_t >( item.lVal ) > get_memories().size() )
		return E_INVALIDARG ;

	memory_iterator pos = get_memories().begin() ;
	
	std::advance( pos, ( item.lVal -1 ) ) ;
	
	get_memories().erase( pos ) ;
	
	return S_OK;
}

STDMETHODIMP CMemories::Add(IMemory **Item)
{
	HRESULT hr ;
	try
	{
		memory_pointer mem ;


		mem->refresh_status() ;
		if ( mem->is_demo() )
		{
			size_t total_size(0) ;
			

			for ( memory_list::iterator pos = m_memories->begin() ; pos != m_memories->end() ; ++pos )
			{
					memory_pointer mem_for_counting = *pos ;
					total_size += mem_for_counting->size() ;
			}
		
			if( total_size >= MAX_MEMORY_SIZE_FOR_DEMO * 2 )
				return E_FAIL ;
		}
	}
	catch( CComException &e )
	{
		return e.code() ;
	}
	catch ( _com_error &e ) 
	{
		return e.Error() ;
	}
	catch( CWinException &e )
	{
		return HRESULT_FROM_WIN32( e.code() ) ;
	}
	catch( ... )
	{
		hr = E_FAIL ;
	}
	
	CComObject< CMemory >* the_memory = 0 ;
	
	hr = CComObject< CMemory >::CreateInstance( &the_memory ) ;
	
	if ( FAILED( hr ) )
	{
		return hr ;
	}
	
	the_memory->AddRef() ;

	memory_pointer mem ;

	m_memories->push_back(mem) ;

	the_memory->set_memory( mem ) ;

	if( SUCCEEDED( hr ) )
	{
		hr = the_memory->QueryInterface( Item ) ;
	}
	
	the_memory->Release() ;
	
	return hr ;
}

memory_list & CMemories::get_memories()
{
	return *m_memories ;
}
