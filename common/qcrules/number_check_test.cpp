/*!
	@brief Tests the Array class
 */

#include "StdAfx.h"
#include "Array.h"
#include "qcrules/number_check.h"

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>

#ifdef UNIT_TEST

using namespace qc ;
using namespace std ;

BOOST_AUTO_TEST_SUITE( NumberCheckTests )

	BOOST_AUTO_TEST_CASE( ensure_init )
	{
		NumberCheckRule rule ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL( 0u, messages.size() ) ;
	}
	BOOST_AUTO_TEST_CASE( empty_source_and_target )
	{
		NumberCheckRule rule ;
		wstring source = L"" ;
		wstring target = L"" ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( no_numbers )
	{
		NumberCheckRule rule ;
		wstring source = L"I have no numbers." ;
		wstring target = L"I have no numbers." ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( matching_numbers )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 5 numbers." ;
		wstring target = L"I have 5 numbers." ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_numbers )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 5 numbers." ;
		wstring target = L"I have 7 numbers." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_numbers_messages )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 5 numbers." ;
		wstring target = L"I have 7 numbers." ;
		rule.check(source, target) ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(1u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"Number Check: Number 5 in source but not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}
	BOOST_AUTO_TEST_CASE( two_non_matching_numbers )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 5 and 7 numbers." ;
		wstring target = L"I have 3 and 9 numbers." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(2u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"Number Check: Number 5 in source but not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
		message = messages[1] ;
		expected = L"Number Check: Number 7 in source but not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_with_decimal )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 5.5 numbers." ;
		wstring target = L"I have 7.7 numbers." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;

		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(1u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"Number Check: Number 5.5 in source but not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_numbers_ends_with_period )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 5." ;
		wstring target = L"I have 7." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_with_commas )
	{
		NumberCheckRule rule ;
		wstring source = L"I have 1,500 numbers." ;
		wstring target = L"I have 2,600 numbers." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;

		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(1u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"Number Check: Number 1,500 in source but not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
