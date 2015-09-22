// SearchResults.cpp : Implementation of CSearchResults

#include "stdafx.h"
#include "SearchResults.h"


// CSearchResults


HRESULT CSearchResults::set_matches( mem_engine::felix_query *matches )
{
	m_coll.clear() ;

	for (size_t i = 0 ; i < matches->size() ; ++i)
	{
		mem_engine::search_match_ptr match = matches->at(i) ;

		CComObject< CSearchResult > *result_object ;
		HRESULT hr = CComObject< CSearchResult >::CreateInstance( &result_object ) ;
		if ( FAILED( hr ) )
		{
			return hr;
		}
		result_object->set_match(match) ;
		CComPtr< ISearchResult > result_ptr ;
		hr = result_object->QueryInterface( &result_ptr ) ;
		if ( FAILED( hr ) )
		{
			return hr ;
		}
		m_coll.push_back( result_ptr ) ;
	}

	return S_OK ;
}