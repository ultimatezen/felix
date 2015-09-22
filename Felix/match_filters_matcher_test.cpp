#include "stdafx.h"
#include "matcher.h"
#include "record_local.h"
#include "replacer.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_mem_search_matcher )

	using namespace mem_search ;
	using namespace mem_engine ;
	using namespace replacer ;

	record_pointer make_record(string source, 
					   		   string trans,
							   string context="")
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		rec->set_context(string2wstring(context)) ;
		return rec ;
	}

	// source
	BOOST_AUTO_TEST_CASE(test_match_source_text_true)
	{
		matcher_source matcher(L"foo", text_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_source_text_false)
	{
		matcher_source matcher(L"bar", text_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_source_regex_true)
	{
		matcher_source matcher(L"\\wo\\w", regex_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_source_regex_false)
	{
		matcher_source matcher(L"\\w\\d\\w", regex_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}

	// trans
	BOOST_AUTO_TEST_CASE(test_match_trans_text_true)
	{
		matcher_trans matcher(L"foo", text_match) ;

		record_pointer rec = make_record("aaa", "foo", "spam") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_trans_text_false)
	{
		matcher_trans matcher(L"bar", text_match) ;

		record_pointer rec = make_record("bar", "xxx", "spam") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_trans_regex_true)
	{
		matcher_trans matcher(L"\\wo\\w", regex_match) ;

		record_pointer rec = make_record("aaa", "xox", "spam") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_trans_regex_false)
	{
		matcher_trans matcher(L"\\w\\d\\w", regex_match) ;

		record_pointer rec = make_record("xxx", "XoX", "spam") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}

	// context
	BOOST_AUTO_TEST_CASE(test_match_context_text_true)
	{
		matcher_context matcher(L"aaa", text_match) ;

		record_pointer rec = make_record("xxx", "bar", "bbb aaa bb") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_context_text_false)
	{
		matcher_context matcher(L"foo", text_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_context_regex_true)
	{
		matcher_context matcher(L"\\wo\\w", regex_match) ;

		record_pointer rec = make_record("xxx", "bar", "foo") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_context_regex_false)
	{
		matcher_context matcher(L"\\w\\d\\w", regex_match) ;

		record_pointer rec = make_record("x1x", "b4r", "xxx") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}

	// creator
	BOOST_AUTO_TEST_CASE(test_match_creator_text_true)
	{
		matcher_created_by matcher(L"aaa", text_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_creator(L"foo aaa bar") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_creator_text_false)
	{
		matcher_created_by matcher(L"foo", text_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;
		rec->set_creator(L"xxx") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_creator_regex_true)
	{
		matcher_created_by matcher(L"\\wo\\w", regex_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_creator(L"pop") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_creator_regex_false)
	{
		matcher_created_by matcher(L"\\w\\d\\w", regex_match) ;

		record_pointer rec = make_record("x1x", "b4r", "x5x") ;
		rec->set_creator(L"xxx") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	// modified_by
	BOOST_AUTO_TEST_CASE(test_match_modified_by_text_true)
	{
		matcher_modified_by matcher(L"aaa", text_match) ;

		record_pointer rec = make_record("xxx", "bar", "") ;
		rec->set_modified_by(L"bb aaa bb") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_modified_by_text_false)
	{
		matcher_modified_by matcher(L"foo", text_match) ;

		record_pointer rec = make_record("foo", "bar", "spam") ;
		rec->set_modified_by(L"bb aaa bb") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_modified_by_regex_true)
	{
		matcher_modified_by matcher(L"\\wo\\w", regex_match) ;

		record_pointer rec = make_record("xxx", "bar", "zz") ;
		rec->set_modified_by(L"wow") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_modified_by_regex_false)
	{
		matcher_modified_by matcher(L"\\w\\d\\w", regex_match) ;

		record_pointer rec = make_record("x1x", "b4r", "x8x") ;
		rec->set_modified_by(L"bb aaa bb") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}
	// generic
	BOOST_AUTO_TEST_CASE(test_generic_source_text_true)
	{
		matcher_generic matcher(L"xxx", text_match) ;

		record_pointer rec = make_record("xxx", "bar", "bbb aaa bb") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_generic_context_text_true)
	{
		matcher_generic matcher(L"aaa", text_match) ;

		record_pointer rec = make_record("xxx", "bar", "bbb aaa bb") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}	
	BOOST_AUTO_TEST_CASE(test_generic_text_false)
	{
		matcher_generic matcher(L"foo", text_match) ;

		record_pointer rec = make_record("xxx", "yyy", "zzz") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE(test_generic_source_regex_true)
	{
		matcher_generic matcher(L"\\w\\w\\w", regex_match) ;

		record_pointer rec = make_record("xxx", "yy", "zz") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_generic_trans_regex_true)
	{
		matcher_generic matcher(L"\\w\\w\\w", regex_match) ;

		record_pointer rec = make_record("xx", "yyu", "zz") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_generic_regex_false)
	{
		matcher_generic matcher(L"\\w\\w\\w", regex_match) ;

		record_pointer rec = make_record("zz", "yy", "xx") ;

		BOOST_CHECK(! matcher.is_match(rec)) ;
	}

	// refcount
	BOOST_AUTO_TEST_CASE(test_match_refcount_equal_true)
	{
		matcher_refcount matcher(8u, sizet_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_equal_false)
	{
		matcher_refcount matcher(7u, sizet_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_lt_true)
	{
		matcher_refcount matcher(7u, std::less<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(6u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_lt_false)
	{
		matcher_refcount matcher(9u, std::less<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(10u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_lte_true)
	{
		matcher_refcount matcher(8u, std::less_equal<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_lte_false)
	{
		matcher_refcount matcher(9u, std::less_equal<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(18u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_gt_true)
	{
		matcher_refcount matcher(17u, std::greater<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(18u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_gt_false)
	{
		matcher_refcount matcher(16u, std::greater<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}


	// reliability
	BOOST_AUTO_TEST_CASE(test_match_reliability_equal_true)
	{
		matcher_reliability matcher(8u, sizet_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_equal_false)
	{
		matcher_reliability matcher(7u, sizet_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_lt_true)
	{
		matcher_reliability matcher(17u, std::less<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_lt_false)
	{
		matcher_reliability matcher(9u, std::less<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(18u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_lte_true)
	{
		matcher_reliability matcher(8u, std::less_equal<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_lte_false)
	{
		matcher_reliability matcher(4u, std::less_equal<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_gt_true)
	{
		matcher_reliability matcher(4u, std::greater<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(9u) ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_gt_false)
	{
		matcher_reliability matcher(5u, std::greater<size_t>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(3u) ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	// validated
	BOOST_AUTO_TEST_CASE(test_match_validated_true)
	{
		matcher_validated matcher(true) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_validated_on() ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_validated_false)
	{
		matcher_validated matcher(true) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_validated_off() ;

		BOOST_CHECK(!matcher.is_match(rec)) ;
	}
	// created on
	BOOST_AUTO_TEST_CASE(test_match_created)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2010")) ;
		matcher_created matcher(thedate, dates_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_created(L"2010/08/10") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_created_before)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2010")) ;
		thedate.set_month(wstring(L"10")) ;
		matcher_created matcher(thedate, std::less<const misc_wrappers::date>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_created(L"2010/9/10") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_created_after)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2010")) ;
		thedate.set_month(wstring(L"10")) ;
		matcher_created matcher(thedate, date_after) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_created(L"2010/11/10") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	// modified on
	BOOST_AUTO_TEST_CASE(test_match_modified)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2010")) ;
		matcher_modified matcher(thedate, dates_match) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_created(L"2010/08/10") ;
		rec->set_modified(L"2010/08/10") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_modified_before)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2010")) ;
		thedate.set_month(wstring(L"10")) ;
		matcher_modified matcher(thedate, std::less<const misc_wrappers::date>()) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_modified(L"2010/9/10") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_modified_after)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2010")) ;
		thedate.set_month(wstring(L"10")) ;
		matcher_modified matcher(thedate, date_after) ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_modified(L"2010/11/10") ;

		BOOST_CHECK(matcher.is_match(rec)) ;
	}

	// get_text_compare

	BOOST_AUTO_TEST_CASE(test_get_text_compare_text)
	{
		wstring query = L"\\w\\w" ;
		wstring haystack = L"\\w\\w" ;
		text_cmp_result result = get_text_compare(query) ;

		BOOST_CHECK(result.second(result.first, haystack)) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_text_compare_regex)
	{
		wstring query = L"regex:\\w\\w" ;
		wstring haystack = L"zz" ;
		text_cmp_result result = get_text_compare(query) ;

		BOOST_CHECK(result.second(result.first, haystack)) ;
	}

	// get_num_compare
	BOOST_AUTO_TEST_CASE(test_get_num_compare_equal)
	{
		BOOST_CHECK(get_num_compare(L"refcount")(8u, 8u)) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_num_compare_lt)
	{
		BOOST_CHECK(get_num_compare(L"refcount-lt")(5u, 8u)) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_num_compare_lte)
	{
		BOOST_CHECK(get_num_compare(L"reliability-lte")(6u, 8u)) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_num_compare_gt)
	{
		BOOST_CHECK(get_num_compare(L"refcount-gt")(10u, 8u)) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_num_compare_gte)
	{
		BOOST_CHECK(get_num_compare(L"reliability-gte")(8u, 8u)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// get_matcher
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_mem_search_get_matcher )

	using namespace mem_search ;
	using namespace mem_engine ;
	record_pointer make_record(string source, 
							   string trans,
							   string context="")
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		rec->set_context(string2wstring(context)) ;
		return rec ;
	}

	// source
	BOOST_AUTO_TEST_CASE(test_match_source_text_true)
	{
		matcher_ptr matcher = get_matcher(L"source:foo") ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE(test_match_source_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"source:regex:\\wo\\w") ;

		record_pointer rec = make_record("foo", "bar", "spam") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}


	// trans
	BOOST_AUTO_TEST_CASE(test_match_trans_text_true)
	{
		matcher_ptr matcher = get_matcher(L"trans:foo") ;

		record_pointer rec = make_record("aaa", "foo", "spam") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE(test_match_trans_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"trans:regex:\\wo\\w") ;

		record_pointer rec = make_record("aaa", "xox", "spam") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}


	// context
	BOOST_AUTO_TEST_CASE(test_match_context_text_true)
	{
		matcher_ptr matcher = get_matcher(L"context:bbb") ;

		record_pointer rec = make_record("xxx", "bar", "bbb aaa bb") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE(test_match_context_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"context:regex:\\wo\\w") ;

		record_pointer rec = make_record("xxx", "bar", "foo") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	// generic
	BOOST_AUTO_TEST_CASE(test_generic_source_text_true)
	{
		matcher_ptr matcher = get_matcher(L"xxx") ;

		record_pointer rec = make_record("xxx", "bar", "bbb aaa bb") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_generic_context_text_true)
	{
		matcher_ptr matcher = get_matcher(L"aaa") ;

		record_pointer rec = make_record("xxx", "bar", "bbb aaa bb") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}	

	BOOST_AUTO_TEST_CASE(test_generic_source_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"regex:\\w\\w\\w") ;

		record_pointer rec = make_record("xxx", "yy", "zz") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_generic_trans_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"regex:\\w\\w\\w") ;

		record_pointer rec = make_record("xx", "yyu", "zz") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	// creator
	BOOST_AUTO_TEST_CASE(test_match_creator_text_true)
	{
		matcher_ptr matcher = get_matcher(L"created-by:aaa") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_creator(L"foo aaa bar") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE(test_match_creator_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"created-by:regex:\\wX\\w") ;

		record_pointer rec = make_record("oto", "bar", "zzz") ;
		rec->set_creator(L"pXp") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	// modified_by
	BOOST_AUTO_TEST_CASE(test_match_modified_by_text_true)
	{
		matcher_ptr matcher = get_matcher(L"modified-by:aaa") ;

		record_pointer rec = make_record("xxx", "bar", "") ;
		rec->set_modified_by(L"bb aaa bb") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	BOOST_AUTO_TEST_CASE(test_match_modified_by_regex_true)
	{
		matcher_ptr matcher = get_matcher(L"modified-by:regex:\\w4\\w") ;

		record_pointer rec = make_record("xxx", "bar", "zz") ;
		rec->set_modified_by(L"w4w") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}

	// refcount
	BOOST_AUTO_TEST_CASE(test_match_refcount_equal_true)
	{
		matcher_ptr matcher = get_matcher(L"refcount:8") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_lt_true)
	{
		matcher_ptr matcher = get_matcher(L"refcount-lt:7") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(5u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_lte_true)
	{
		matcher_ptr matcher = get_matcher(L"refcount-lte:8") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_gt_true)
	{
		matcher_ptr matcher = get_matcher(L"refcount-gt:7") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_refcount_gte_true)
	{
		matcher_ptr matcher = get_matcher(L"refcount-gte:7") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_refcount(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}


	// reliability
	BOOST_AUTO_TEST_CASE(test_match_reliability_equal_true)
	{
		matcher_ptr matcher = get_matcher(L"reliability:8") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_lt_true)
	{
		matcher_ptr matcher = get_matcher(L"reliability-lt:10") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_lte_true)
	{
		matcher_ptr matcher = get_matcher(L"reliability-lte:10") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_gt_true)
	{
		matcher_ptr matcher = get_matcher(L"reliability-gt:5") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(8u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_reliability_gte_true)
	{
		matcher_ptr matcher = get_matcher(L"reliability-gte:5") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_reliability(5u) ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	// validated
	BOOST_AUTO_TEST_CASE(test_match_validated_true)
	{
		matcher_ptr matcher = get_matcher(L"validated:true") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_validated_on() ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	BOOST_AUTO_TEST_CASE(test_match_validated_false)
	{
		matcher_ptr matcher = get_matcher(L"validated:false") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_validated_on() ;

		BOOST_CHECK(!matcher->is_match(rec)) ;
	}
	// created on
	BOOST_AUTO_TEST_CASE(test_match_created)
	{
		matcher_ptr matcher = get_matcher(L"created:2010") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_created(L"2010/08/10") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
	// modified on
	BOOST_AUTO_TEST_CASE(test_match_modified)
	{
		matcher_ptr matcher = get_matcher(L"modified:2010") ;

		record_pointer rec = make_record("xxx", "bar", "zzz") ;
		rec->set_modified(L"2010/08/10") ;

		BOOST_CHECK(matcher->is_match(rec)) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif