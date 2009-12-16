#include "stdafx.h"
#include "match_filters.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_searcher ;
	using namespace mem_engine ;

	// match_filter_generic
	TEST(runner_term_matches_test, generic_source_true)
	{
		wstring term(L"spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, generic_term_case_true)
	{
		wstring term(L"SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, generic_trans_true)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, generic_context_true)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, generic_false)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, generic_substring_true)
	{
		wstring term(L"pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, generic_case_true)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"SPAM") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	// source_filter
	TEST(runner_term_matches_test, source_true)
	{
		wstring term(L"source:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, source_term_case_true)
	{
		wstring term(L"source:SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, source_trans_false)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, source_context_false)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, source_false)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, source_substring_true)
	{
		wstring term(L"source:pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, source_case_true)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"SPAM") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}

	// trans_filter
	TEST(runner_term_matches_test, trans_true)
	{
		wstring term(L"trans:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"fafa") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, trans_term_case_true)
	{
		wstring term(L"trans:SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"gaga") ;
		rec->set_trans(L"sPAm") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, trans_source_false)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, trans_context_false)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, trans_false)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, trans_substring_true)
	{
		wstring term(L"trans:pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"gaga") ;
		rec->set_trans(L"I love spam!") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, trans_case_true)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"agg") ;
		rec->set_trans(L"SPAM") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}

	// context_filter
	TEST(runner_term_matches_test, context_true)
	{
		wstring term(L"context:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, context_term_case_true)
	{
		wstring term(L"context:SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"gaga") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, context_trans_false)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, context_source_false)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, context_false)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		ASSERT_TRUE(!runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, context_substring_true)
	{
		wstring term(L"context:pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"tata") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"SPAM") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(runner_term_matches_test, context_case_true)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"tata") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"SPAM") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}

	// match_filters
	TEST(match_filters_test, source_true_trans_true)
	{
		search_runner runner ;
		runner.add_term(L"source:spam") ;
		runner.add_term(L"trans:egg") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		ASSERT_TRUE(runner.is_match(rec)) ;
	}

	TEST(match_filters_test, source_true_trans_false)
	{
		search_runner runner ;
		runner.add_term(L"source:spam") ;
		runner.add_term(L"trans:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		ASSERT_TRUE(!runner.is_match(rec)) ;
	}
	TEST(match_filters_test, source_false_trans_true)
	{
		search_runner runner ;
		runner.add_term(L"source:egg") ;
		runner.add_term(L"trans:egg") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		ASSERT_TRUE(!runner.is_match(rec)) ;
	}
}

#endif