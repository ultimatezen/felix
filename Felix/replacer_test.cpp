
#include "StdAfx.h"
#include "replacer.h"
#include "miscwrappers.h"
#include "easyunit/testharness.h"
#include "record_local.h"

#ifdef UNIT_TEST


namespace easyunit
{
	using namespace memory_engine;

	SimpleString tss(wstring value)
	{
		return SimpleString(string2string(value).c_str()) ;
	}

	TEST(mod_date_tests, year_only)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010") ;
		ASSERT_EQUALS_V(2010, thedate.wYear) ;
		ASSERT_EQUALS_V(10, thedate.wMonth) ;
		ASSERT_EQUALS_V(1, thedate.wDay) ;
	}
	TEST(mod_date_tests, year_month_slash)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010/8") ;
		ASSERT_EQUALS_V(2010, thedate.wYear) ;
		ASSERT_EQUALS_V(8, thedate.wMonth) ;
		ASSERT_EQUALS_V(1, thedate.wDay) ;
	}
	TEST(mod_date_tests, year_month_dash)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010-12") ;
		ASSERT_EQUALS_V(2010, thedate.wYear) ;
		ASSERT_EQUALS_V(12, thedate.wMonth) ;
		ASSERT_EQUALS_V(1, thedate.wDay) ;
	}
	TEST(mod_date_tests, year_month_day)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;

		replacer::mod_date(thedate, L"2010-1-23") ;
		ASSERT_EQUALS_V(2010, thedate.wYear) ;
		ASSERT_EQUALS_V(1, thedate.wMonth) ;
		ASSERT_EQUALS_V(23, thedate.wDay) ;
	}


	TEST(replacer_tests, source)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;

		replacer::source(rec, L"spam", L"egg"); 

		ASSERT_EQUALS_V("egg", tss(rec->get_source_rich())) ;
		ASSERT_EQUALS_V("", tss(rec->get_trans_rich())) ;
	}
	TEST(replacer_tests, trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_trans(L"spam") ;

		replacer::trans(rec, L"spam", L"egg"); 

		ASSERT_EQUALS_V("egg", tss(rec->get_trans_rich())) ;
		ASSERT_EQUALS_V("", tss(rec->get_source_rich())) ;
	}
	TEST(replacer_tests, context)
	{
		record_pointer rec(new record_local) ;
		rec->set_context(L"spam") ;

		replacer::context(rec, L"spam", L"egg"); 

		ASSERT_EQUALS_V("egg", tss(rec->get_context_rich())) ;
		ASSERT_EQUALS_V("", tss(rec->get_source_rich())) ;
	}
	TEST(replacer_tests, none_specified)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"a spam") ;
		rec->set_trans(L"b spam") ;
		rec->set_context(L"c spam") ;

		replacer::none_specified(rec, L"spam", L"egg"); 

		ASSERT_EQUALS_V("a egg", tss(rec->get_source_rich())) ;
		ASSERT_EQUALS_V("b egg", tss(rec->get_trans_rich())) ;
		ASSERT_EQUALS_V("c egg", tss(rec->get_context_rich())) ;
	}
	TEST(replacer_tests, created_by)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::created_by(rec, L"Ryan", L"Reiko"); 

		ASSERT_EQUALS_V("Reiko Ginstrom", tss(rec->get_creator())) ;
	}
	TEST(replacer_tests, created_by_asterisk)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::created_by(rec, L"*", L"Reiko"); 

		ASSERT_EQUALS_V("Reiko", tss(rec->get_creator())) ;
	}
	TEST(replacer_tests, validated_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_off() ;

		replacer::validated(rec, L"true"); 

		ASSERT_TRUE(rec->is_validated()) ;
	}
	TEST(replacer_tests, validated_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_on() ;

		replacer::validated(rec, L"false"); 

		ASSERT_TRUE(!rec->is_validated()) ;
	}
	TEST(replacer_tests, reliability)
	{
		record_pointer rec(new record_local) ;
		rec->set_reliability(5) ;

		replacer::reliability(rec, L"8"); 

		ASSERT_EQUALS_V(8, (int)rec->get_reliability()) ;
	}
	TEST(replacer_tests, refcount)
	{
		record_pointer rec(new record_local) ;
		rec->set_refcount(0) ;

		replacer::refcount(rec, L"100"); 

		ASSERT_EQUALS_V(100, (int)rec->get_refcount()) ;
	}
	TEST(replacer_tests, created)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_created(thedate) ;

		replacer::date_created(rec, L"1999/9/9") ;

		misc_wrappers::date newdate = rec->get_created() ;

		ASSERT_EQUALS_V(1999, newdate.wYear) ;
		ASSERT_EQUALS_V(9, newdate.wMonth) ;
		ASSERT_EQUALS_V(9, newdate.wDay) ;
	}
	TEST(replacer_tests, modified)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_modified(thedate) ;

		replacer::date_modified(rec, L"1999-9-9") ;

		misc_wrappers::date newdate = rec->get_modified() ;

		ASSERT_EQUALS_V(1999, newdate.wYear) ;
		ASSERT_EQUALS_V(9, newdate.wMonth) ;
		ASSERT_EQUALS_V(9, newdate.wDay) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// do_replace
	//////////////////////////////////////////////////////////////////////////


	TEST(do_replace_tests, source)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;

		replacer::do_replace(rec, L"source:spam", L"egg") ;

		ASSERT_EQUALS_V("egg", tss(rec->get_source_rich())) ;
		ASSERT_EQUALS_V("", tss(rec->get_trans_rich())) ;
	}
	TEST(do_replace_tests, trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_trans(L"spam") ;

		replacer::do_replace(rec, L"trans:spam", L"egg") ;

		ASSERT_EQUALS_V("egg", tss(rec->get_trans_rich())) ;
		ASSERT_EQUALS_V("", tss(rec->get_source_rich())) ;
	}
	TEST(do_replace_tests, context)
	{
		record_pointer rec(new record_local) ;
		rec->set_context(L"spam") ;

		replacer::do_replace(rec, L"context:spam", L"egg") ;

		ASSERT_EQUALS_V("egg", tss(rec->get_context_rich())) ;
		ASSERT_EQUALS_V("", tss(rec->get_source_rich())) ;
	}
	TEST(do_replace_tests, none_specified)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"a spam") ;
		rec->set_trans(L"b spam") ;
		rec->set_context(L"c spam") ;

		replacer::do_replace(rec, L"spam", L"egg") ;

		ASSERT_EQUALS_V("a egg", tss(rec->get_source_rich())) ;
		ASSERT_EQUALS_V("b egg", tss(rec->get_trans_rich())) ;
		ASSERT_EQUALS_V("c egg", tss(rec->get_context_rich())) ;
	}
	TEST(do_replace_tests, created_by)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::do_replace(rec, L"created-by:Ryan", L"Reiko") ;

		ASSERT_EQUALS_V("Reiko Ginstrom", tss(rec->get_creator())) ;
	}
	TEST(do_replace_tests, created_by_asterisk)
	{
		record_pointer rec(new record_local) ;
		rec->set_creator(L"Ryan Ginstrom") ;

		replacer::do_replace(rec, L"created-by:*", L"Reiko") ;

		ASSERT_EQUALS_V("Reiko", tss(rec->get_creator())) ;
	}
	TEST(do_replace_tests, validated_true)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_off() ;

		replacer::do_replace(rec, L"validated:spam", L"true") ;

		ASSERT_TRUE(rec->is_validated()) ;
	}
	TEST(do_replace_tests, validated_false)
	{
		record_pointer rec(new record_local) ;
		rec->set_validated_on() ;

		replacer::do_replace(rec, L"validated:", L"false") ;

		ASSERT_TRUE(!rec->is_validated()) ;
	}
	TEST(do_replace_tests, reliability)
	{
		record_pointer rec(new record_local) ;
		rec->set_reliability(5) ;

		replacer::do_replace(rec, L"reliability:spam", L"3") ;

		ASSERT_EQUALS_V(3, (int)rec->get_reliability()) ;
	}
	TEST(do_replace_tests, refcount)
	{
		record_pointer rec(new record_local) ;
		rec->set_refcount(0) ;

		replacer::do_replace(rec, L"refcount:", L"9") ;

		ASSERT_EQUALS_V(9, (int)rec->get_refcount()) ;
	}
	TEST(do_replace_tests, created)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_created(thedate) ;

		replacer::do_replace(rec, L"created:", L"1999-9-9") ;

		misc_wrappers::date newdate = rec->get_created() ;

		ASSERT_EQUALS_V(1999, newdate.wYear) ;
		ASSERT_EQUALS_V(9, newdate.wMonth) ;
		ASSERT_EQUALS_V(9, newdate.wDay) ;
	}
	TEST(do_replace_tests, modified)
	{
		misc_wrappers::date thedate ;
		thedate.wYear = 2000 ;
		thedate.wMonth = 10 ; 
		thedate.wDay = 1 ;
		record_pointer rec(new record_local) ;
		rec->set_modified(thedate) ;

		replacer::do_replace(rec, L"modified:", L"1999-9-9") ;

		misc_wrappers::date newdate = rec->get_modified() ;

		ASSERT_EQUALS_V(1999, newdate.wYear) ;
		ASSERT_EQUALS_V(9, newdate.wMonth) ;
		ASSERT_EQUALS_V(9, newdate.wDay) ;
	}
}

#endif 


