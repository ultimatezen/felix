#include "StdAfx.h"
#include ".\match_tester.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{

	using namespace mem_engine ;

	// search_match_tester_regex

	TEST(test_search_match_tester_regex, basic_match)
	{
		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"spam" ;

		search_match_tester_regex matcher(params) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"I luv spam") ;

		ASSERT_TRUE(matcher.is_match(rec)) ;
	}
	TEST(test_search_match_tester_regex, basic_nomatch)
	{
		search_query_params params ;
		params.m_use_regex = true ;
		params.m_source = L"egg" ;

		search_match_tester_regex matcher(params) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"I luv spam") ;

		ASSERT_TRUE(! matcher.is_match(rec)) ;
	}
	TEST(test_search_match_tester_regex, get_search_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"1") ;
		search_query_params params ;
		search_match_tester_regex matcher(params) ;

		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		SimpleString expected = "1" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_search_match_tester_regex, is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester_regex matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		SimpleString expected = "egg 1" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_search_match_tester_regex, is_match_two)
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

		SimpleString expected = "egg 1" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

		expected = "egg 2" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	// search_match_tester
	TEST(test_search_match_tester, is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"egg 1") ;

		search_query_params params ;
		params.m_use_regex = false ;
		params.m_source = L"egg" ;
		search_match_tester matcher(params) ;

		matcher.is_match(rec1) ;
		search_match_ptr match1 = matcher.get_search_match(rec1) ;

		SimpleString expected = "<span style=\"background:#FFFF99\">egg</span> 1" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_search_match_tester, is_match_two)
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

		SimpleString expected = "<span style=\"background:#FFFF99\">egg</span> 1" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

		expected = "<span style=\"background:#FFFF99\">egg</span> 2" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	// gloss_match_tester
	TEST(test_gloss_match_tester, is_match_one)
	{
		record_pointer rec1(new record_local) ;
		rec1->set_source(L"spam") ;

		gloss_match_tester matcher(L"I like spam and eggs") ;

		matcher.test_source(rec1) ;
		search_match_ptr match1 = matcher.get_search_match() ;

		SimpleString expected = "spam" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_gloss_match_tester, is_match_two)
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

		SimpleString expected = "spam" ;
		SimpleString actual = CStringA(match1->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

		expected = "eggs" ;
		actual = (LPCSTR)CStringA(match2->get_markup()->GetSource().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

}


#endif