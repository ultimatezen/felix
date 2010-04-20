#include "stdafx.h"
#include "miscwrappers.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_misc_wrappers_date )

	BOOST_AUTO_TEST_CASE( init)
	{
		misc_wrappers::date thedate ;
		BOOST_CHECK_EQUAL(thedate.wYear, 0) ;
		BOOST_CHECK_EQUAL(thedate.wMonth, 0) ;
		BOOST_CHECK_EQUAL(thedate.wDay, 0) ;
		BOOST_CHECK_EQUAL(thedate.wHour, 0) ;
		BOOST_CHECK_EQUAL(thedate.wMinute, 0) ;
		BOOST_CHECK_EQUAL(thedate.wSecond, 0) ;
		BOOST_CHECK_EQUAL(thedate.wMilliseconds, 0) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// set values
	//////////////////////////////////////////////////////////////////////////

	// year
	BOOST_AUTO_TEST_CASE( year_2009)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2009")) ;
		BOOST_CHECK_EQUAL(thedate.wYear, 2009) ;
	}
	BOOST_AUTO_TEST_CASE( year_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wYear, 0) ;
	}
	// month
	BOOST_AUTO_TEST_CASE( month_10)
	{
		misc_wrappers::date thedate ;
		thedate.set_month(wstring(L"10")) ;
		BOOST_CHECK_EQUAL(thedate.wMonth, 10) ;
	}
	BOOST_AUTO_TEST_CASE( month_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_month(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wMonth, 0) ;
	}
	// day
	BOOST_AUTO_TEST_CASE( day_13)
	{
		misc_wrappers::date thedate ;
		thedate.set_day(wstring(L"13")) ;
		BOOST_CHECK_EQUAL(thedate.wDay, 13) ;
	}
	BOOST_AUTO_TEST_CASE( day_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_day(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wDay, 0) ;
	}
	// hour
	BOOST_AUTO_TEST_CASE( hour_20)
	{
		misc_wrappers::date thedate ;
		thedate.set_hour(wstring(L"20")) ;
		BOOST_CHECK_EQUAL(thedate.wHour, 20) ;
	}
	BOOST_AUTO_TEST_CASE( hour_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_hour(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wHour, 0) ;
	}
	// minute
	BOOST_AUTO_TEST_CASE( minute_20)
	{
		misc_wrappers::date thedate ;
		thedate.set_minute(wstring(L"20")) ;
		BOOST_CHECK_EQUAL(thedate.wMinute, 20) ;
	}
	BOOST_AUTO_TEST_CASE( minute_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_minute(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wMinute, 0) ;
	}
	// second
	BOOST_AUTO_TEST_CASE( second_30)
	{
		misc_wrappers::date thedate ;
		thedate.set_second(wstring(L"30")) ;
		BOOST_CHECK_EQUAL(thedate.wSecond, 30) ;
	}
	BOOST_AUTO_TEST_CASE( second_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_second(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wSecond, 0) ;
	}
	// msec
	BOOST_AUTO_TEST_CASE( msec_600)
	{
		misc_wrappers::date thedate ;
		thedate.set_milliseconds(wstring(L"600")) ;
		BOOST_CHECK_EQUAL(thedate.wMilliseconds, 600) ;
	}
	BOOST_AUTO_TEST_CASE( msec_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_milliseconds(wstring(L"")) ;
		BOOST_CHECK_EQUAL(thedate.wMilliseconds, 0) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Parse date/time
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( init_2009_10_01_5_13_1)
	{
		const wstring datestr = L"2009/10/01 5:13:01" ;
		misc_wrappers::date thedate(datestr) ;

		BOOST_CHECK_EQUAL(thedate.wYear, 2009) ;
		BOOST_CHECK_EQUAL(thedate.wMonth, 10) ;
		BOOST_CHECK_EQUAL(thedate.wDay, 1) ;
		BOOST_CHECK_EQUAL(thedate.wHour, 5) ;
		BOOST_CHECK_EQUAL(thedate.wMinute, 13) ;
		BOOST_CHECK_EQUAL(thedate.wSecond, 1) ;
		BOOST_CHECK_EQUAL(thedate.wMilliseconds, 0) ;
	}
	BOOST_AUTO_TEST_CASE( bad_date_throws)
	{
		try
		{
			const wstring first_datestr = L"foo/bar/baz" ;
			misc_wrappers::date first(first_datestr) ;
			BOOST_FAIL("Should have thrown on bad date format") ;
		}
		catch (except::CException& e)
		{
			e ;
			BOOST_CHECK(TRUE) ;			
		}
	}
BOOST_AUTO_TEST_SUITE_END()

#endif