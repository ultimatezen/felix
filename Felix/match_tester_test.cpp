#include "StdAfx.h"
#include ".\match_tester.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_search_match_tester_regex )

	using namespace mem_engine ;

	// search_match_tester_regex

	BOOST_AUTO_TEST_CASE( basic_match)
	{
		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"spam" ;

		search_match_tester_regex matcher(params) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"I luv spam") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE( basic_nomatch)
	{
		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"egg" ;

		search_match_tester_regex matcher(params) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"I luv spam") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE( get_search_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"1") ;
		search_query_params params ;
		search_match_tester_regex matcher(params) ;

		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		string expected = "1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester_regex matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		string expected = "egg 1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( is_match_two)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		record_pointer rec2(new record_local) ;
		rec2->set_source(L"egg 2") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester_regex matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;
		matcher.is_match(rec2) ;
		search_match_ptr match2 = matcher.get_search_match(rec2) ;

		string expected = "egg 1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = "egg 2" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	// search_match_tester

	BOOST_AUTO_TEST_SUITE( test_search_match_tester )

		using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		string expected = "<span style=\"background:#FFFF99\">egg</span> 1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( is_match_two)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		record_pointer rec2(new record_local) ;
		rec2->set_source(L"egg 2") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;
		matcher.is_match(rec2) ;
		search_match_ptr match2 = matcher.get_search_match(rec2) ;

		string expected = "<span style=\"background:#FFFF99\">egg</span> 1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = "<span style=\"background:#FFFF99\">egg</span> 2" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_SUITE_END()

		BOOST_AUTO_TEST_SUITE( test_gloss_match_tester )

		using namespace mem_engine ;
	// gloss_match_tester
	BOOST_AUTO_TEST_CASE( is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"spam") ;

		gloss_match_tester matcher(L"I like spam and eggs") ;

		matcher.test_source(rec1) ;
		search_match_ptr match1 = matcher.get_search_match() ;

		string expected = "spam" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( is_match_two)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"spam") ;

		record_pointer rec2(new record_local) ;
		rec2->set_source(L"eggs") ;

		gloss_match_tester matcher(L"I like spam and eggs") ;

		matcher.test_source(rec1) ;
		search_match_ptr match1 = matcher.get_search_match() ;
		matcher.test_source(rec2) ;
		search_match_ptr match2 = matcher.get_search_match() ;

		string expected = "spam" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = "eggs" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()