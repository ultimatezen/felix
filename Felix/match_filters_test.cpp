#include "stdafx.h"
#include "match_filters.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( match_filters_dates_match )

	using namespace memory_searcher ;
	using namespace mem_engine ;

	// dates_match
	BOOST_AUTO_TEST_CASE( year_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(dates_match(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( year_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"2000") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(! dates_match(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( month_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(dates_match(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( month_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-06") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(! dates_match(query, entry)) ;
	}

	BOOST_AUTO_TEST_CASE( day_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-10") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(dates_match(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( day_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-11") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(! dates_match(query, entry)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( match_filters_date_after )

using namespace memory_searcher ;
using namespace mem_engine ;

	// date_after
	BOOST_AUTO_TEST_CASE( year_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1998") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(date_after(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( year_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"2000") ;
		query.set_year(datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(! date_after(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( month_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 6, 10) ;

		BOOST_CHECK(date_after(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( month_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-06") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 6, 10) ;

		BOOST_CHECK(! date_after(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( day_true)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-10") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 11) ;

		BOOST_CHECK(date_after(query, entry)) ;
	}
	BOOST_AUTO_TEST_CASE( day_false)
	{
		misc_wrappers::date query ;
		wstring datestr(L"1999-05-11") ;
		mod_date(query, datestr) ;
		misc_wrappers::date entry(1999, 5, 10) ;

		BOOST_CHECK(! date_after(query, entry)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( runner_term_matches_test )

using namespace memory_searcher ;
using namespace mem_engine ;

	// match_filter_generic
	BOOST_AUTO_TEST_CASE( generic_source_true)
	{
		wstring term(L"spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( generic_term_case_true)
	{
		wstring term(L"SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( generic_trans_true)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( generic_context_true)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( generic_false)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( generic_substring_true)
	{
		wstring term(L"pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( generic_case_true)
	{
		wstring term(L"spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"SPAM") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	// source_filter
	BOOST_AUTO_TEST_CASE( source_true)
	{
		wstring term(L"source:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( source_term_case_true)
	{
		wstring term(L"source:SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( source_trans_false)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( source_context_false)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( source_false)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( source_substring_true)
	{
		wstring term(L"source:pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( source_case_true)
	{
		wstring term(L"source:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"SPAM") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}

	// trans_filter
	BOOST_AUTO_TEST_CASE( trans_true)
	{
		wstring term(L"trans:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"fafa") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( trans_term_case_true)
	{
		wstring term(L"trans:SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"gaga") ;
		rec->set_trans(L"sPAm") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( trans_source_false)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( trans_context_false)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( trans_false)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( trans_substring_true)
	{
		wstring term(L"trans:pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"gaga") ;
		rec->set_trans(L"I love spam!") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( trans_case_true)
	{
		wstring term(L"trans:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"agg") ;
		rec->set_trans(L"SPAM") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}

	// context_filter
	BOOST_AUTO_TEST_CASE( context_true)
	{
		wstring term(L"context:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( context_term_case_true)
	{
		wstring term(L"context:SPAM") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"gaga") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"spam") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( context_trans_false)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"spam") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( context_source_false)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( context_false)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"egg") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"egg") ;

		search_runner runner ;
		BOOST_CHECK(!runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( context_substring_true)
	{
		wstring term(L"context:pa") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"tata") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"SPAM") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( context_case_true)
	{
		wstring term(L"context:spam") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"tata") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"SPAM") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// created
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( match_filters_created_test )

using namespace memory_searcher ;
using namespace mem_engine ;

	// true
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_true)
	{
		wstring term(L"created:2010-01-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_true)
	{
		wstring term(L"created:2010-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_true)
	{
		wstring term(L"created:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_space_true)
	{
		wstring term(L"created: 2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( created_on_yyyy_true)
	{
		wstring term(L"created-on:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	// false
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_false)
	{
		wstring term(L"created:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_false)
	{
		wstring term(L"created:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_false)
	{
		wstring term(L"created:2009") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// modified
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( match_filters_modified_test )

using namespace memory_searcher ;
	using namespace mem_engine ;

	// true
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_true)
	{
		wstring term(L"modified:2010-01-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_true)
	{
		wstring term(L"modified:2010-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_space_true)
	{
		wstring term(L"modified: 2010-01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_true)
	{
		wstring term(L"modified:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( modified_on_yyyy_true)
	{
		wstring term(L"modified-on:2010/01") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	// false
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_false)
	{
		wstring term(L"modified:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_false)
	{
		wstring term(L"modified:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_false)
	{
		wstring term(L"modified:2009") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_modified(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// created-before
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( match_filters_created_before_test )

using namespace memory_searcher ;
using namespace mem_engine ;

	// true
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_true)
	{
		wstring term(L"created-before:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_true)
	{
		wstring term(L"created-before:2010-05") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/04/29") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_true)
	{
		wstring term(L"created-before:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_space_true)
	{
		wstring term(L"created-before: 2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	// false
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_false)
	{
		wstring term(L"created-before:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2011/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_false)
	{
		wstring term(L"created-before:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/02/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_false)
	{
		wstring term(L"created-before:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}

	BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// created-after
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( match_filters_created_after_test )

using namespace memory_searcher ;
	using namespace mem_engine ;

	// true
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_true)
	{
		wstring term(L"created-after:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/01/03") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_true)
	{
		wstring term(L"created-after:2010-05") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/06/29") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_true)
	{
		wstring term(L"created-after:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2011/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_space_after_true)
	{
		wstring term(L"created-after:2010 ") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2011/01/01") ;

		search_runner runner ;
		BOOST_CHECK(runner.term_matches(rec, term)) ;
	}
	// false
	BOOST_AUTO_TEST_CASE( yyyy_mm_dd_false)
	{
		wstring term(L"created-after:2010-01-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_mm_false)
	{
		wstring term(L"created-after:2010-02") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2010/02/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
	BOOST_AUTO_TEST_CASE( yyyy_false)
	{
		wstring term(L"created-after:2010") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_created(L"2009/01/01") ;

		search_runner runner ;
		BOOST_CHECK(! runner.term_matches(rec, term)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( match_filters_test )

using namespace memory_searcher ;
using namespace mem_engine ;

	//////////////////////////////////////////////////////////////////////////
	// match_filters
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( source_true_trans_true)
	{
		search_runner runner ;
		runner.add_term(L"source:spam") ;
		runner.add_term(L"trans:egg") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		BOOST_CHECK(runner.is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE( source_true_trans_false)
	{
		search_runner runner ;
		runner.add_term(L"source:spam") ;
		runner.add_term(L"trans:spam") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		BOOST_CHECK(!runner.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE( source_false_trans_true)
	{
		search_runner runner ;
		runner.add_term(L"source:egg") ;
		runner.add_term(L"trans:egg") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		BOOST_CHECK(!runner.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE( bad_date_false)
	{
		search_runner runner ;
		runner.add_term(L"created:blarg!") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		rec->set_context(L"bacon") ;

		BOOST_CHECK(!runner.is_match(rec)) ;
	}

BOOST_AUTO_TEST_SUITE_END()