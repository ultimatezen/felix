#include "StdAfx.h"
#include ".\match_tester.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

//////////////////////////////////////////////////////////////////////////
// search_match_tester_regex
//////////////////////////////////////////////////////////////////////////

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

		wstring expected = L"1" ;
		wstring actual = match1->get_markup()->GetSource() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(is_match_one_source)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"egg" ;
		search_match_tester_regex matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		wstring expected = L"<span class=\"concordance_match\">egg</span> 1" ;
		wstring actual = match1->get_markup()->GetSource() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE(match_num_regex_source_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg 13") ;
		rec->set_trans(L"egg 13 chicks") ;
		rec->set_context(L"13 eggs") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"\\d{2}" ;
		search_match_tester_regex matcher(params) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
		search_match_ptr match = matcher.get_search_match(rec) ;

		wstring expected = L"egg <span class=\"concordance_match\">13</span>" ;
		wstring actual = match->get_markup()->GetSource() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(match_num_regex_source_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg 1") ;
		rec->set_trans(L"egg 13 chicks") ;
		rec->set_context(L"13 eggs") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"\\d{2}" ;
		search_match_tester_regex matcher(params) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(match_num_regex_trans_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg 13") ;
		rec->set_trans(L"egg 13 chicks") ;
		rec->set_context(L"13 eggs") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_trans = L"\\d{2}" ;
		search_match_tester_regex matcher(params) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
		search_match_ptr match = matcher.get_search_match(rec) ;

		wstring expected = L"egg <span class=\"concordance_match\">13</span> chicks" ;
		wstring actual = match->get_markup()->GetTrans() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(match_num_regex_trans_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg 13") ;
		rec->set_trans(L"egg 1a3 chicks") ;
		rec->set_context(L"13 eggs") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_trans = L"\\d{2}" ;
		search_match_tester_regex matcher(params) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(match_num_regex_context_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg 13") ;
		rec->set_trans(L"egg 13 chicks") ;
		rec->set_context(L"13 eggs") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_context = L"\\d{2}" ;
		search_match_tester_regex matcher(params) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
		search_match_ptr match = matcher.get_search_match(rec) ;

		wstring expected = L"<span class=\"concordance_match\">13</span> eggs" ;
		wstring actual = match->get_markup()->GetContext() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(match_num_regex_context_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg 13") ;
		rec->set_trans(L"egg 13 chicks") ;
		rec->set_context(L"eggs") ;

		search_query_params params ;
		params.m_use_regex = true ;
		params.m_context = L"\\d{2}" ;
		search_match_tester_regex matcher(params) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
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

		wstring expected = L"<span class=\"concordance_match\">egg</span> 1" ;
		wstring actual = match1->get_markup()->GetSource() ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = L"<span class=\"concordance_match\">egg</span> 2" ;
		actual = match2->get_markup()->GetSource() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// search_match_tester
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_search_match_tester )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE(test_normalize_case)
	{
		search_query_params params ;
		params.m_ignore_case = true ;
		search_match_tester tester(params) ;

		wstring query = L"ABC" ;
		tester.normalize(query) ;

		wstring expected = L"abc" ;

		BOOST_CHECK_EQUAL(expected, query) ;
	}

	BOOST_AUTO_TEST_CASE(test_normalize_hira_kara)
	{
		search_query_params params ;
		params.m_ignore_case = true ;
		params.m_ignore_hira_kata = true ;
		search_match_tester tester(params) ;

		wstring query = L"Ç†Ç¢Ç§Ç¶Ç®" ;
		tester.normalize(query) ;

		wstring expected = L"ÉAÉCÉEÉGÉI" ;

		BOOST_CHECK_EQUAL(expected, query) ;
	}

	BOOST_AUTO_TEST_CASE(test_normalize_width)
	{
		search_query_params params ;
		params.m_ignore_width = true ;
		search_match_tester tester(params) ;

		wstring query = L"ÇPÇQÇR" ;
		tester.normalize(query) ;

		wstring expected = L"123" ;

		BOOST_CHECK_EQUAL(expected, query) ;
	}

	BOOST_AUTO_TEST_CASE(test_mark_up_string)
	{
		search_query_params params ;
		search_match_tester tester(params) ;

		wstring base = L"1234567" ;
		wstring sub = L"345" ;

		wstring actual = tester.mark_up_string(base,
											   base.find(sub),
											   sub.size()) ;

		wstring expected = L"12<span class=\"concordance_match\">345</span>67" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE(is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		string expected = "<span class=\"concordance_match\">egg</span> 1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(is_match_two)
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

		string expected = "<span class=\"concordance_match\">egg</span> 1" ;
		string actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = "<span class=\"concordance_match\">egg</span> 2" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// gloss_match_tester
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_gloss_match_tester )

	using namespace mem_engine ;
	// gloss_match_tester
	BOOST_AUTO_TEST_CASE(is_match_one)
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

#endif