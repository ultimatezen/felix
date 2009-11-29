#include "StdAfx.h"
#include "SearchResult.h"
#include "query.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

using namespace memory_engine;

namespace easyunit
{
	typedef CComObject< CSearchResult > result_obj ;
	typedef CComPtr< result_obj > result_ptr ;

	TEST( TestSearchResult, Instantiate )
	{
		result_ptr result ;
		HRESULT hr = result_obj::CreateInstance( &result ) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( result ) ;
	}
	TEST( TestSearchResult, get_record )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		CComPtr<IRecord> rec ;
		HRESULT hr = result->get_Record(&rec) ;
		ASSERT_TRUE( SUCCEEDED( hr ) ) ;
		ASSERT_TRUE( rec ) ;
	}
	TEST( TestSearchResult, get_score )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		double score = 5.0 ;
		result->get_Score(&score) ;
		ASSERT_EQUALS_DELTA_V(0.0, score, 0.00001);
	}
	TEST( TestSearchResult, set_match )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		search_match_ptr match(new search_match) ;
		match->set_base_score(0.5) ;
		result->set_match(match) ;
		double score = 1.0 ;
		result->get_Score(&score) ;
		ASSERT_EQUALS_DELTA_V(0.5, score, 0.00001);
	}
}

#endif