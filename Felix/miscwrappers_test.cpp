#include "stdafx.h"
#include "miscwrappers.h"
#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( misc_wrappers_date, init)
	{
		misc_wrappers::date thedate ;
		ASSERT_EQUALS_V(thedate.wYear, 0) ;
		ASSERT_EQUALS_V(thedate.wMonth, 0) ;
		ASSERT_EQUALS_V(thedate.wDay, 0) ;
		ASSERT_EQUALS_V(thedate.wHour, 0) ;
		ASSERT_EQUALS_V(thedate.wMinute, 0) ;
		ASSERT_EQUALS_V(thedate.wSecond, 0) ;
		ASSERT_EQUALS_V(thedate.wMilliseconds, 0) ;
	}
	// year
	TEST( misc_wrappers_date_set, year_2009)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"2009")) ;
		ASSERT_EQUALS_V(thedate.wYear, 2009) ;
	}
	TEST( misc_wrappers_date_set, year_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_year(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wYear, 0) ;
	}
	// month
	TEST( misc_wrappers_date_set, month_10)
	{
		misc_wrappers::date thedate ;
		thedate.set_month(wstring(L"10")) ;
		ASSERT_EQUALS_V(thedate.wMonth, 10) ;
	}
	TEST( misc_wrappers_date_set, month_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_month(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wMonth, 0) ;
	}
	// day
	TEST( misc_wrappers_date_set, day_13)
	{
		misc_wrappers::date thedate ;
		thedate.set_day(wstring(L"13")) ;
		ASSERT_EQUALS_V(thedate.wDay, 13) ;
	}
	TEST( misc_wrappers_date_set, day_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_day(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wDay, 0) ;
	}
	// hour
	TEST( misc_wrappers_date_set, hour_20)
	{
		misc_wrappers::date thedate ;
		thedate.set_hour(wstring(L"20")) ;
		ASSERT_EQUALS_V(thedate.wHour, 20) ;
	}
	TEST( misc_wrappers_date_set, hour_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_hour(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wHour, 0) ;
	}
	// minute
	TEST( misc_wrappers_date_set, minute_20)
	{
		misc_wrappers::date thedate ;
		thedate.set_minute(wstring(L"20")) ;
		ASSERT_EQUALS_V(thedate.wMinute, 20) ;
	}
	TEST( misc_wrappers_date_set, minute_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_minute(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wMinute, 0) ;
	}
	// second
	TEST( misc_wrappers_date_set, second_30)
	{
		misc_wrappers::date thedate ;
		thedate.set_second(wstring(L"30")) ;
		ASSERT_EQUALS_V(thedate.wSecond, 30) ;
	}
	TEST( misc_wrappers_date_set, second_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_second(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wSecond, 0) ;
	}
	// msec
	TEST( misc_wrappers_date_set, msec_600)
	{
		misc_wrappers::date thedate ;
		thedate.set_milliseconds(wstring(L"600")) ;
		ASSERT_EQUALS_V(thedate.wMilliseconds, 600) ;
	}
	TEST( misc_wrappers_date_set, msec_empty)
	{
		misc_wrappers::date thedate ;
		thedate.set_milliseconds(wstring(L"")) ;
		ASSERT_EQUALS_V(thedate.wMilliseconds, 0) ;
	}


}

#endif