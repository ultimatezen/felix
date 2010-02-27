#include "stdafx.h"
#include "match_filters.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_searcher ;
	using namespace mem_engine ;

	// dates_match
	TEST(match_filters_dates_match, year_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(dates_match(query, entry)) ;
	}
	TEST(match_filters_dates_match, year_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"2000") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(! dates_match(query, entry)) ;
	}
	TEST(match_filters_dates_match, month_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(dates_match(query, entry)) ;
	}
	TEST(match_filters_dates_match, month_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-06") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(! dates_match(query, entry)) ;
	}

	TEST(match_filters_dates_match, day_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-10") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(dates_match(query, entry)) ;
	}
	TEST(match_filters_dates_match, day_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-11") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(! dates_match(query, entry)) ;
	}

	// date_after
	TEST(match_filters_date_after, year_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1998") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(date_after(query, entry)) ;
	}
	TEST(match_filters_date_after, year_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"2000") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(! date_after(query, entry)) ;
	}
	TEST(match_filters_date_after, month_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 6, 10) ;

		ASSERT_TRUE(date_after(query, entry)) ;
	}
	TEST(match_filters_date_after, month_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-06") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 6, 10) ;

		ASSERT_TRUE(! date_after(query, entry)) ;
	}
	TEST(match_filters_date_after, day_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-10") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 11) ;

		ASSERT_TRUE(date_after(query, entry)) ;
	}
	TEST(match_filters_date_after, day_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-11") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		ASSERT_TRUE(! date_after(query, entry)) ;
	}


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

	//////////////////////////////////////////////////////////////////////////
	// created
	//////////////////////////////////////////////////////////////////////////

	// true
	TEST(match_filters_created_test, yyyy_mm_dd_true)
	{
		wstring term(L"created:2010-01-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_test, yyyy_mm_true)
	{
		wstring term(L"created:2010-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_test, yyyy_true)
	{
		wstring term(L"created:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_test, created_on_yyyy_true)
	{
		wstring term(L"created-on:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	// false
	TEST(match_filters_created_test, yyyy_mm_dd_false)
	{
		wstring term(L"created:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_test, yyyy_mm_false)
	{
		wstring term(L"created:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_test, yyyy_false)
	{
		wstring term(L"created:2009") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// modified
	//////////////////////////////////////////////////////////////////////////

	// true
	TEST(match_filters_modified_test, yyyy_mm_dd_true)
	{
		wstring term(L"modified:2010-01-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_modified_test, yyyy_mm_true)
	{
		wstring term(L"modified:2010-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_modified_test, yyyy_true)
	{
		wstring term(L"modified:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_modified_test, modified_on_yyyy_true)
	{
		wstring term(L"modified-on:2010/01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	// false
	TEST(match_filters_modified_test, yyyy_mm_dd_false)
	{
		wstring term(L"modified:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_modified_test, yyyy_mm_false)
	{
		wstring term(L"modified:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_modified_test, yyyy_false)
	{
		wstring term(L"modified:2009") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// created-before
	//////////////////////////////////////////////////////////////////////////

	// true
	TEST(match_filters_created_before_test, yyyy_mm_dd_true)
	{
		wstring term(L"created-before:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_before_test, yyyy_mm_true)
	{
		wstring term(L"created-before:2010-05") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/04/29") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_before_test, yyyy_true)
	{
		wstring term(L"created-before:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	// false
	TEST(match_filters_created_before_test, yyyy_mm_dd_false)
	{
		wstring term(L"created-before:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2011/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_before_test, yyyy_mm_false)
	{
		wstring term(L"created-before:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/02/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_before_test, yyyy_false)
	{
		wstring term(L"created-before:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// created-after
	//////////////////////////////////////////////////////////////////////////

	// true
	TEST(match_filters_created_after_test, yyyy_mm_dd_true)
	{
		wstring term(L"created-after:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/03") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_after_test, yyyy_mm_true)
	{
		wstring term(L"created-after:2010-05") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/06/29") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_after_test, yyyy_true)
	{
		wstring term(L"created-after:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2011/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(runner.term_matches(rec, term)) ;
	}
	// false
	TEST(match_filters_created_after_test, yyyy_mm_dd_false)
	{
		wstring term(L"created-after:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_after_test, yyyy_mm_false)
	{
		wstring term(L"created-after:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/02/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}
	TEST(match_filters_created_after_test, yyyy_false)
	{
		wstring term(L"created-after:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		ASSERT_TRUE(! runner.term_matches(rec, term)) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// match_filters
	//////////////////////////////////////////////////////////////////////////
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