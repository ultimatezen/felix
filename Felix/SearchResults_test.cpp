#include "StdAfx.h"
#include ".\SearchResults.h"
#include "record_local.h"
#include "search_match.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCSearchResults )

	using namespace mem_engine ;
	typedef CComPtr< CComObject< CSearchResults > > results_ptr ;

	search_match_ptr make_match_match(string source, string trans, int id=0)
	{
		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		match->set_memory_id(id) ;
		match->set_base_score(1.0) ;

		return match ;
	}

	BOOST_AUTO_TEST_CASE( Instantiate )
	{
		results_ptr results ;
		HRESULT hr = CComObject< CSearchResults >::CreateInstance( &results ) ;
		BOOST_CHECK( SUCCEEDED( hr ) ) ;
		BOOST_CHECK( results ) ;
	}
	BOOST_AUTO_TEST_CASE( init_coll_empty )
	{
		results_ptr results ;
		CComObject< CSearchResults >::CreateInstance( &results ) ;
		BOOST_CHECK_EQUAL(0u, results->m_coll.size()) ;
	}
	BOOST_AUTO_TEST_CASE( init_count_empty )
	{
		results_ptr results ;
		CComObject< CSearchResults >::CreateInstance( &results ) ;
		long count(0) ;
		results->get_Count(&count) ;
		BOOST_CHECK_EQUAL(0, count) ;
	}
	BOOST_AUTO_TEST_CASE( set_matches_empty )
	{
		results_ptr results ;
		CComObject< CSearchResults >::CreateInstance( &results ) ;
		translation_match_query matches ;
		results->set_matches(&matches) ;
		BOOST_CHECK_EQUAL(0u, results->m_coll.size()) ;
	}
	BOOST_AUTO_TEST_CASE( set_matches_2 )
	{
		results_ptr results ;
		CComObject< CSearchResults >::CreateInstance( &results ) ;

		search_match_ptr m1 = make_match_match("source 1", "trans 1", 1) ;
		search_match_ptr m2 = make_match_match("source 2", "trans 2", 1) ;

		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		translation_match_query search_matches; 
		search_matches.set_matches(matches) ;

		results->set_matches(&search_matches) ;
		BOOST_CHECK_EQUAL(2u, results->m_coll.size()) ;
	}
	BOOST_AUTO_TEST_CASE( retrieve_match )
	{
		results_ptr results ;
		CComObject< CSearchResults >::CreateInstance( &results ) ;

		search_match_ptr m1 = make_match_match("source 1", "trans 1", 1) ;
		search_match_ptr m2 = make_match_match("source 2", "trans 2", 1) ;

		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		translation_match_query search_matches; 
		search_matches.set_matches(matches) ;

		results->set_matches(&search_matches) ;

		CComPtr<ISearchResult> result ;
		results->get_Item(1, &result) ;
		CComPtr<IRecord> record ;
		result->get_Record(&record) ;
		CComBSTR source ;
		record->get_Source(&source) ;
		BOOST_CHECK_EQUAL(L"source 1", source) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif 
