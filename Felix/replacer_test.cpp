
#include "StdAfx.h"
#include "replacer.h"
#include "miscwrappers.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( mod_date_tests )

	using namespace mem_engine;

	BOOST_AUTO_TEST_CASE( year_only)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010") ;
		BOOST_CHECK_EQUAL(2010, thedate.wYear) ;
		BOOST_CHECK_EQUAL(10, thedate.wMonth) ;
		BOOST_CHECK_EQUAL(1, thedate.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( year_month_slash)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010/8") ;
		BOOST_CHECK_EQUAL(2010, thedate.wYear) ;
		BOOST_CHECK_EQUAL(8, thedate.wMonth) ;
		BOOST_CHECK_EQUAL(1, thedate.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( year_month_dash)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010-12") ;
		BOOST_CHECK_EQUAL(2010, thedate.wYear) ;
		BOOST_CHECK_EQUAL(12, thedate.wMonth) ;
		BOOST_CHECK_EQUAL(1, thedate.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( year_month_day)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010-1-23") ;
		BOOST_CHECK_EQUAL(2010, thedate.wYear) ;
		BOOST_CHECK_EQUAL(1, thedate.wMonth) ;
		BOOST_CHECK_EQUAL(23, thedate.wDay) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( replacer_tests )

	using namespace mem_engine;

	BOOST_AUTO_TEST_CASE( source)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;

		replacer::source(rec, L"spam", L"egg"); 

		BOOST_CHECK_EQUAL(L"egg", rec->get_source_rich()) ;
		BOOST_CHECK_EQUAL("", string2string(rec->get_trans_rich())) ;
	}
	BOOST_AUTO_TEST_CASE(source_japanese)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"ひらがな") ;

		replacer::source(rec, L"ひらがな", L"カタカナ"); 

		BOOST_CHECK_EQUAL(L"カタカナ", rec->get_source_rich()) ;
	}
	BOOST_AUTO_TEST_CASE(source_regex)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam xxx egg") ;

		replacer::source(rec, L"regex:(\\s(\\w\\w\\w)\\s)", L" \\3-\\3 "); 

		wstring expected = L"spam xxx-xxx egg" ;
		BOOST_CHECK_EQUAL(expected, rec->get_source_rich()) ;
	}

	BOOST_AUTO_TEST_CASE( trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_trans(L"spam") ;

		replacer::trans(rec, L"spam", L"egg"); 

		BOOST_CHECK_EQUAL("egg", string2string(rec->get_trans_rich())) ;
		BOOST_CHECK_EQUAL("", string2string(rec->get_source_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( context)
	{
		record_pointer rec(new record_local) ;
		rec->set_context(L"spam") ;

		replacer::context(rec, L"spam", L"egg"); 

		BOOST_CHECK_EQUAL("egg", string2string(rec->get_context_rich())) ;
		BOOST_CHECK_EQUAL("", string2string(rec->get_source_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( none_specified)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"a spam") ;
		rec->set_trans(L"b spam") ;
		rec->set_context(L"c spam") ;

		replacer::none_specified(rec, L"spam", L"egg"); 

		BOOST_CHECK_EQUAL("a egg", string2string(rec->get_source_rich())) ;
		BOOST_CHECK_EQUAL("b egg", string2string(rec->get_trans_rich())) ;
		BOOST_CHECK_EQUAL("c egg", string2string(rec->get_context_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::created_by(rec, L"Ryan", L"Reiko"); 

		BOOST_CHECK_EQUAL("Reiko Ginstrom", string2string(rec->get_creator())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by_asterisk)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::created_by(rec, L"*", L"Reiko"); 

		BOOST_CHECK_EQUAL("Reiko", string2string(rec->get_creator())) ;
	}
	BOOST_AUTO_TEST_CASE( validated_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_off() ;

		replacer::validated(rec, L"true"); 

		BOOST_CHECK(rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( validated_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_on() ;

		replacer::validated(rec, L"false"); 

		BOOST_CHECK(!rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( reliability)
	{
		record_pointer rec(new record_local) ;
		rec->set_reliability(5) ;

		replacer::reliability(rec, L"8"); 

		BOOST_CHECK_EQUAL(8, (int)rec->get_reliability()) ;
	}
	BOOST_AUTO_TEST_CASE( refcount)
	{
		record_pointer rec(new record_local) ;
		rec->set_refcount(0) ;

		replacer::refcount(rec, L"100"); 

		BOOST_CHECK_EQUAL(100, (int)rec->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( created)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_created(thedate) ;

		replacer::date_created(rec, L"1999/9/9") ;

		misc_wrappers::date newdate = rec->get_created() ;

		BOOST_CHECK_EQUAL(1999, newdate.wYear) ;
		BOOST_CHECK_EQUAL(9, newdate.wMonth) ;
		BOOST_CHECK_EQUAL(9, newdate.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( modified)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_modified(thedate) ;

		replacer::date_modified(rec, L"1999-9-9") ;

		misc_wrappers::date newdate = rec->get_modified() ;

		BOOST_CHECK_EQUAL(1999, newdate.wYear) ;
		BOOST_CHECK_EQUAL(9, newdate.wMonth) ;
		BOOST_CHECK_EQUAL(9, newdate.wDay) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// do_replace
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( do_replace_tests )

using namespace mem_engine;

	BOOST_AUTO_TEST_CASE( source)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;

		replacer::do_replace(rec, L"source:spam", L"egg") ;

		BOOST_CHECK_EQUAL("egg", string2string(rec->get_source_rich())) ;
		BOOST_CHECK_EQUAL("", string2string(rec->get_trans_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_trans(L"spam") ;

		replacer::do_replace(rec, L"trans:spam", L"egg") ;

		BOOST_CHECK_EQUAL("egg", string2string(rec->get_trans_rich())) ;
		BOOST_CHECK_EQUAL("", string2string(rec->get_source_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( context)
	{
		record_pointer rec(new record_local) ;
		rec->set_context(L"spam") ;

		replacer::do_replace(rec, L"context:spam", L"egg") ;

		BOOST_CHECK_EQUAL("egg", string2string(rec->get_context_rich())) ;
		BOOST_CHECK_EQUAL("", string2string(rec->get_source_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( none_specified)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"a spam") ;
		rec->set_trans(L"b spam") ;
		rec->set_context(L"c spam") ;

		replacer::do_replace(rec, L"spam", L"egg") ;

		BOOST_CHECK_EQUAL("a egg", string2string(rec->get_source_rich())) ;
		BOOST_CHECK_EQUAL("b egg", string2string(rec->get_trans_rich())) ;
		BOOST_CHECK_EQUAL("c egg", string2string(rec->get_context_rich())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::do_replace(rec, L"created-by:Ryan", L"Reiko") ;

		BOOST_CHECK_EQUAL("Reiko Ginstrom", string2string(rec->get_creator())) ;
	}
	BOOST_AUTO_TEST_CASE( created_by_asterisk)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::do_replace(rec, L"created-by:*", L"Reiko") ;

		BOOST_CHECK_EQUAL("Reiko", string2string(rec->get_creator())) ;
	}
	BOOST_AUTO_TEST_CASE( validated_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_off() ;

		replacer::do_replace(rec, L"validated:spam", L"true") ;

		BOOST_CHECK(rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( validated_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_on() ;

		replacer::do_replace(rec, L"validated:", L"false") ;

		BOOST_CHECK(!rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( reliability)
	{
		record_pointer rec(new record_local) ;
		rec->set_reliability(5) ;

		replacer::do_replace(rec, L"reliability:spam", L"3") ;

		BOOST_CHECK_EQUAL(3, (int)rec->get_reliability()) ;
	}
	BOOST_AUTO_TEST_CASE( refcount)
	{
		record_pointer rec(new record_local) ;
		rec->set_refcount(0) ;

		replacer::do_replace(rec, L"refcount:", L"9") ;

		BOOST_CHECK_EQUAL(9, (int)rec->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( created)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_created(thedate) ;

		replacer::do_replace(rec, L"created:", L"1999-9-9") ;

		misc_wrappers::date newdate = rec->get_created() ;

		BOOST_CHECK_EQUAL(1999, newdate.wYear) ;
		BOOST_CHECK_EQUAL(9, newdate.wMonth) ;
		BOOST_CHECK_EQUAL(9, newdate.wDay) ;
	}
	BOOST_AUTO_TEST_CASE( modified)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_modified(thedate) ;

		replacer::do_replace(rec, L"modified:", L"1999-9-9") ;

		misc_wrappers::date newdate = rec->get_modified() ;

		BOOST_CHECK_EQUAL(1999, newdate.wYear) ;
		BOOST_CHECK_EQUAL(9, newdate.wMonth) ;
		BOOST_CHECK_EQUAL(9, newdate.wDay) ;
	}


BOOST_AUTO_TEST_SUITE_END()


#endif 


