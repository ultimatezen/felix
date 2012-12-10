#include "stdafx.h"
#include "gloss_rules.h"
#include "output_device_fake.h"
#include "input_device_fake.h"

#ifdef UNIT_TEST

using namespace placement ;

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( regex_rules_tests )

	BOOST_AUTO_TEST_CASE( create_regex_rule )
	{
		placement::regex_rule(L"aaa", L"bbb", L"ccc") :
		BOOST_CHECK_EQUAL(rule.m_name, L"aaa") ;
		BOOST_CHECK_EQUAL(rule.m_pattern, L"bbb") ;
		BOOST_CHECK_EQUAL(rule.m_repl, L"ccc") ;
	}
	BOOST_AUTO_TEST_CASE( create_regex_rules )
	{
		placement::regex_rules rules ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_regex_rules_load_and_parse )

	BOOST_AUTO_TEST_CASE( test_empty )
	{
		placement::regex_rules rules ;
		pugi::xml_document doc;
		BOOST_CHECK_NO_THROW(rules.parse(doc)) ;
	}
	BOOST_AUTO_TEST_CASE( load_empty )
	{
		placement::regex_rules rules ;
		BOOST_CHECK_EQUAL(0u, rules.m_rules.size()) ;
	}
	BOOST_AUTO_TEST_CASE( load_empty_tag )
	{
		placement::regex_rules rules ;
		output_device_ptr output(new OutputDeviceFake) ;
		InputDeviceFake *raw_input = new InputDeviceFake ;
		raw_input->set_view("<rules></rules>") ;
		input_device_ptr input(raw_input) ;

		BOOST_CHECK_NO_THROW(rules.load(input, output)) ;
		BOOST_CHECK_EQUAL(0u, rules.m_rules.size()) ;
	}
	BOOST_AUTO_TEST_CASE( load_file_disabled )
	{
		placement::regex_rules rules ;
		output_device_ptr output(new OutputDeviceFake) ;
		InputDeviceFake *raw_input = new InputDeviceFake ;
		raw_input->set_view("<?xml version='1.0' encoding='utf-8'?>"
			"<rules>"
			"<rule>"
			"<name>Numbers</name>"
			"<source>(\\d+(\\.\\d+)?)</source>"
			"<target>\1</target>"
			"<sample>My number 123.456</sample>"
			"<enabled>0</enabled>"
			"</rule>"
			"<rule>"
			"<name>ALL CAPS</name>"
			"<source>([-_A-Z][-_A-Z]+)</source>"
			"<target>\1</target>"
			"<sample>Some text ALL-CAPS_BIG-NAME.</sample>"
			"<enabled>0</enabled>"
			"</rule>"
			"</rules>") ;
		input_device_ptr input(raw_input) ;

		BOOST_CHECK_NO_THROW(rules.load(input, output)) ;
		BOOST_CHECK_EQUAL(0u, rules.m_rules.size()) ;
	}
	BOOST_AUTO_TEST_CASE(load_file_enabled)
	{
		placement::regex_rules rules ;
		output_device_ptr output(new OutputDeviceFake) ;
		InputDeviceFake *raw_input = new InputDeviceFake ;
		raw_input->set_view("<?xml version='1.0' encoding='utf-8'?>"
			"<rules>"
			"<rule>"
			"<name>Numbers</name>"
			"<source>(\\d+(\\.\\d+)?)</source>"
			"<target>\1</target>"
			"<sample>My number 123.456</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"<rule>"
			"<name>ALL CAPS</name>"
			"<source>([-_A-Z][-_A-Z]+)</source>"
			"<target>\1</target>"
			"<sample>Some text ALL-CAPS_BIG-NAME.</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"</rules>") ;
		input_device_ptr input(raw_input) ;

		BOOST_CHECK_NO_THROW(rules.load(input, output)) ;
		BOOST_CHECK_EQUAL(0u, rules.m_rules.size()) ;
	}

	
BOOST_AUTO_TEST_SUITE_END()



#endif