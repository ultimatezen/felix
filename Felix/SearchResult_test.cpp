#include "StdAfx.h"
#include "SearchResult.h"
#include "query.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestSearchResult )


	using namespace mem_engine;
	typedef CComObject< CSearchResult > result_obj ;
	typedef CComPtr< result_obj > result_ptr ;

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		result_ptr result ;
		HRESULT hr = result_obj::CreateInstance( &result ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( result ) ;
	}
	BOOST_AUTO_TEST_CASE( get_record )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		CComPtr<IRecord> rec ;
		HRESULT hr = result->get_Record(&rec) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( rec ) ;
	}
	BOOST_AUTO_TEST_CASE( get_score )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		double score = 5.0 ;
		result->get_Score(&score) ;
		BOOST_CHECK_CLOSE(0.0, score, 0.00001);
	}
	BOOST_AUTO_TEST_CASE( set_match )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		search_match_ptr match(new search_match) ;
		match->set_base_score(0.5) ;
		result->set_match(match) ;
		double score = 1.0 ;
		result->get_Score(&score) ;
		BOOST_CHECK_CLOSE(0.5, score, 0.00001);
	}
	BOOST_AUTO_TEST_CASE( get_name )
	{
		result_ptr result ;
		result_obj::CreateInstance( &result ) ;
		search_match_ptr match(new search_match) ;
		CComBSTR name ;
		match->set_base_score(0.5) ;
		const wstring loc = L"foo.ftm" ;
		match->set_memory_location(loc) ;
		result->set_match(match) ;
		result->get_MemoryName(&name) ;
		string expected = "foo.ftm" ;
		string actual = CW2A(name) ;
		BOOST_CHECK_EQUAL(expected, actual);
	}
BOOST_AUTO_TEST_SUITE_END()

#endif