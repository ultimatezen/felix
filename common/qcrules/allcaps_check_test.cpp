/*!
	@brief Tests the Array class
 */

#include "StdAfx.h"
#include "Array.h"

#include "allcaps_check.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

using namespace qc ;
using namespace std ;

BOOST_AUTO_TEST_SUITE( AllCapsCheckTests )

	BOOST_AUTO_TEST_CASE( ensure_init )
	{
		AllCapsCheckRule rule ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL( 0u, messages.size() ) ;
	}
	BOOST_AUTO_TEST_CASE( empty_source_and_target )
	{
		AllCapsCheckRule rule ;
		wstring source = L"" ;
		wstring target = L"" ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}

	BOOST_AUTO_TEST_CASE( no_all_caps )
	{
		AllCapsCheckRule rule ;
		wstring source = L"I have no all caps." ;
		wstring target = L"I have no numbers." ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( matching_all_capps )
	{
		AllCapsCheckRule rule ;
		wstring source = L"I have ALLCAPS." ;
		wstring target = L"I have ALLCAPS." ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_all_caps )
	{
		AllCapsCheckRule rule ;
		wstring source = L"I have ALLCAPS." ;
		wstring target = L"I have allcaps." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_matching_all_caps_messages )
	{
		AllCapsCheckRule rule ;
		wstring source = L"I have ALLCAPS." ;
		wstring target = L"I have allcaps." ;
		rule.check(source, target) ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(1u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"ALL CAPS Check: Word ALLCAPS in source but not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
