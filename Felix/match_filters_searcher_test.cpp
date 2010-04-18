#include "stdafx.h"
#include "match_filters.h"
#include "record_local.h"
#include "memory_local.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( searcher_test )


	using namespace memory_searcher ;
	using namespace mem_engine ;

	record_pointer make_record_searcher(wstring source, 
										wstring trans,
										wstring context)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(source) ;
		rec->set_trans(trans) ;
		rec->set_context(context) ;
		return rec ;
	}
	record_pointer make_record_searcher(string source, 
										string trans,
										string context="")
	{
		return make_record_searcher(string2wstring(source), 
									string2wstring(trans),
									string2wstring(context)) ;
	}
	record_pointer add_record_searcher(memory_pointer &mem, 
							 string source, 
							 string trans,
							 string context="")
	{
		record_pointer rec = make_record_searcher(source, trans, context) ;
		mem->add_record(rec) ;
		return rec ;
	}

	BOOST_AUTO_TEST_CASE( add_term)
	{
		search_runner searcher ;
		searcher.add_term(L"source") ;

		BOOST_CHECK_EQUAL(1, static_cast<int>(searcher.get_terms().size())) ;
	}

	BOOST_AUTO_TEST_CASE( remove_term)
	{
		search_runner searcher ;
		searcher.add_term(L"source") ;
		searcher.remove_term(0) ;

		BOOST_CHECK_EQUAL(0, static_cast<int>(searcher.get_terms().size())) ;
	}


	BOOST_AUTO_TEST_CASE( empty_memory_no_terms)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}

	BOOST_AUTO_TEST_CASE( one_record_no_terms)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;

		add_record_searcher(mem, "source", "trans", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}

	BOOST_AUTO_TEST_CASE( one_record_one_term_match)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"source") ;

		add_record_searcher(mem, "source", "trans", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}

	BOOST_AUTO_TEST_CASE( one_record_one_term_nomatch)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"eggs") ;

		add_record_searcher(mem, "source", "trans", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}

	BOOST_AUTO_TEST_CASE( one_record_one_term_nomatch_erased)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"eggs") ;
		searcher.remove_term(0) ;

		add_record_searcher(mem, "source", "trans", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}

	BOOST_AUTO_TEST_CASE( one_record_two_terms_match)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"source:source") ;
		searcher.add_term(L"egg") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}

	// Text Fields
	// source: 	Search in source field
	BOOST_AUTO_TEST_CASE( source_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"source:source") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( source_two_of_three)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"source:source") ;

		add_record_searcher(mem, "source a", "egg", "context") ;
		add_record_searcher(mem, "a source", "egg", "context") ;
		add_record_searcher(mem, "egg", "source", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(2, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( source_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"source:egg") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( source_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"source:") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	// trans: 	Search in translation field
	BOOST_AUTO_TEST_CASE( trans_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"trans:egg") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( trans_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"trans:") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( trans_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"trans:source") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// context: 	Search in context field
	BOOST_AUTO_TEST_CASE( context_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"context:context") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( context_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"context:context") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( context_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"context:source") ;

		add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// Created
	// created-by: 	Search in creator field
	BOOST_AUTO_TEST_CASE( created_by_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-by:Ryan") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_creator(L"Bob") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-by:") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_creator(L"Bob") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-by:Ryan") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_creator(L"Ryan") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by_partial)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-by:Ryan") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_creator(L"Ryan Ginstrom") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	// created: 	Search for records created on date (YYYY-MM-DD format)
	BOOST_AUTO_TEST_CASE( created_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created:2000-10-10") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created:") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created:2000-10-11") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// created-before: 	Search for records created before date (YYYY-MM-DD format)
	BOOST_AUTO_TEST_CASE( created_before_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-before:2000-10-10") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(1999, 10, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_before_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-before:1999-10-11") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// created-after: 	Search for records created after date (YYYY-MM-DD format)
	BOOST_AUTO_TEST_CASE( created_after_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-after:1999/10/10") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2009, 10, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( created_after_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"created-after:2009-10-1") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2008, 3, 10) ;
		rec->set_created(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// Modified
	// modified-by: 	Search in modifier field
	BOOST_AUTO_TEST_CASE( modified_by_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-by:Ryan") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_modified_by(L"Ryan") ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( modified_by_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-by:") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_modified_by(L"Ryan") ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( modified_by_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-by:Bob") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_modified_by(L"Ryan") ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// modified: 	Search for records modified on date (YYYY-MM-DD format)
	BOOST_AUTO_TEST_CASE( modified_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified:2000-10-10") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( modified_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified:") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( modified_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified:2000-10-11") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// modified-before: 	Search for records modified before date (YYYY-MM-DD format)
	BOOST_AUTO_TEST_CASE( modified_before_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-before:2000-10-10") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(1999, 10, 10) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( modified_before_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-before:1995-10-11") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2000, 10, 10) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// modified-after: 	Search for records modified after date (YYYY-MM-DD format)
	BOOST_AUTO_TEST_CASE( modified_after_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-after:1999/10/10") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2010, 10, 10) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( modified_after_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"modified-after:2009-10-1") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		misc_wrappers::date thedate(2009, 10, 1) ;
		rec->set_modified(thedate) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// Reliability
	// reliability: 	Search for records with the specified reliability
	BOOST_AUTO_TEST_CASE( reliability_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( reliability_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability:") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( reliability_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability:4") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}

	// reliability-gt: 	Search for records with greater than the specified reliability
	BOOST_AUTO_TEST_CASE( reliability_gt_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-gt:4") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( reliability_gt_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-gt:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// reliability-gte: 	Search for records with at least the specified reliability
	BOOST_AUTO_TEST_CASE( reliability_gte_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-gte:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( reliability_gte_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-gte:9") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// reliability-lt: 	Search for records with less than the specified reliability
	BOOST_AUTO_TEST_CASE( reliability_lt_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-lt:1") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(0) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( reliability_lt_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-lt:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// reliability-lte: 	Search for records with no more than than the specified reliability
	BOOST_AUTO_TEST_CASE( reliability_lte_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-lte:7") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( reliability_lte_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"reliability-lte:6") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_reliability(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	// Validated
	// validated: 	Search for records that are validated ("true") or not validated ("false")
	BOOST_AUTO_TEST_CASE( validated_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"validated:false") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( validated_empty)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"validated:") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( validated_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"validated:true") ;
		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// Reference Count
	// refcount: 	Search for records with the specified refcount
	BOOST_AUTO_TEST_CASE( refcount_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( refcount_emtpy)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount:") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( refcount_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount:4") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}

	// refcount-gt: 	Search for records with greater than the specified refcount
	BOOST_AUTO_TEST_CASE( refcount_gt_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-gt:4") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( refcount_gt_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-gt:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// refcount-gte: 	Search for records with at least the specified refcount
	BOOST_AUTO_TEST_CASE( refcount_gte_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-gte:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( refcount_gte_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-gte:6") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// refcount-lt: 	Search for records with less than the specified refcount
	BOOST_AUTO_TEST_CASE( refcount_lt_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-lt:1") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(0) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( refcount_lt_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-lt:5") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}
	// refcount-lte: 	Search for records with no more than than the specified refcount
	BOOST_AUTO_TEST_CASE( refcount_lte_true)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-lte:3") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(1, static_cast<int>(matches.size())) ;
	}
	BOOST_AUTO_TEST_CASE( refcount_lte_false)
	{
		memory_pointer mem(new memory_local(0.0f)) ;

		search_match_container matches ;
		search_runner searcher ;
		searcher.add_term(L"refcount-lte:6") ;

		record_pointer rec = add_record_searcher(mem, "source", "egg", "context") ;
		rec->set_refcount(5) ;

		searcher.get_matches(mem, matches) ;
		BOOST_CHECK_EQUAL(0, static_cast<int>(matches.size())) ;
	}

BOOST_AUTO_TEST_SUITE_END()
