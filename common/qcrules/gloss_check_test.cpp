/*!
	@brief Tests the Array class
 */

#include "StdAfx.h"
#include "Array.h"

#include "gloss_check.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

using namespace qc ;
using namespace std ;

BOOST_AUTO_TEST_SUITE( GlossCheckRuleTests )

	BOOST_AUTO_TEST_CASE( ensure_init )
	{
		std::vector<gloss_pair> matches ;
		GlossCheckRule rule(matches) ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL( 0u, messages.size() ) ;
	}
	BOOST_AUTO_TEST_CASE( empty_source_and_target )
	{
		std::vector<gloss_pair> matches ;
		GlossCheckRule rule(matches) ;
		wstring source = L"" ;
		wstring target = L"" ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}

	BOOST_AUTO_TEST_CASE( gloss_match )
	{
		std::vector<gloss_pair> matches ;
		matches.push_back(gloss_pair(L"foo", L"bar")) ;
		GlossCheckRule rule(matches) ;

		wstring source = L"I have foo." ;
		wstring target = L"I have bar." ;
		BOOST_CHECK( rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_gloss_match )
	{
		std::vector<gloss_pair> matches ;
		matches.push_back(gloss_pair(L"foo", L"bar")) ;
		GlossCheckRule rule(matches) ;

		wstring source = L"I have foo." ;
		wstring target = L"I have foo." ;
		BOOST_CHECK( ! rule.check(source, target) ) ;
	}
	BOOST_AUTO_TEST_CASE( non_gloss_match_messages )
	{
		std::vector<gloss_pair> matches ;
		matches.push_back(gloss_pair(L"foo", L"bar")) ;
		GlossCheckRule rule(matches) ;

		wstring source = L"I have foo." ;
		wstring target = L"I have foo." ;
		rule.check(source, target) ;
		vector<wstring> messages ;
		rule.get_error_msgs(messages) ;
		BOOST_CHECK_EQUAL(1u, messages.size()) ;
		wstring message = messages[0] ;
		wstring expected = L"Gloss Check: Word 'foo' in source but 'bar' not in target" ;
		BOOST_CHECK_EQUAL(message, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
