#include "stdafx.h"
#include "rule_placement.h"
#include "output_device_fake.h"
#include "input_device_fake.h"

#ifdef UNIT_TEST

using namespace mem_engine ;

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_regex_rule )

	BOOST_AUTO_TEST_CASE( test_create_regex_rule )
	{
		placement::regex_rule rule(L"aaa", L"bbb", L"ccc") ;
		BOOST_CHECK_EQUAL(rule.m_name, L"aaa") ;
		BOOST_CHECK_EQUAL(rule.m_expr.str(), L"bbb") ;
		BOOST_CHECK_EQUAL(rule.m_repl, L"ccc") ;

	}

	BOOST_AUTO_TEST_CASE( test_get_matches )
	{
		placement::regex_rule rule(L"Numbers", L"(\\d+)", L"\\1") ;
		wstring haystack = L"a 1 bb 22 ccc 333" ;
		std::vector<wstring> matches ;
		rule.get_matches(haystack, matches) ;
		BOOST_CHECK_EQUAL(3u, matches.size()) ;
		BOOST_CHECK_EQUAL(matches[0], L"1") ;
		BOOST_CHECK_EQUAL(matches[1], L"22") ;
		BOOST_CHECK_EQUAL(matches[2], L"333") ;

	}
	BOOST_AUTO_TEST_CASE( test_get_replacements )
	{
		placement::regex_rule rule(L"Numbers", L"(\\d+)", L"\\1") ;
		wstring haystack = L"a 1 bb 22 ccc 333" ;
		std::vector<wstring> matches ;
		rule.get_matches(haystack, matches) ;
		std::vector<std::pair<wstring, wstring> > replacements;
		rule.get_replacements(matches, replacements) ;
		BOOST_CHECK_EQUAL(3u, replacements.size()) ;
		for (size_t i=replacements.size(); i<3; ++i)
		{
			BOOST_CHECK_EQUAL(replacements[i], std::make_pair(matches[i], matches[i])) ;
		}
	}
	BOOST_AUTO_TEST_CASE( test_get_replacements_twice )
	{
		placement::regex_rule rule(L"Numbers", L"(\\d+)", L"\\1") ;
		wstring haystack = L"a 1 bb 22 ccc 333" ;
		std::vector<wstring> matches ;
		rule.get_matches(haystack, matches) ;
		std::vector<std::pair<wstring, wstring> > replacements;
		rule.get_replacements(matches, replacements) ;

		haystack = L"4 5" ;
		matches.clear() ;
		rule.get_matches(haystack, matches) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
		rule.get_replacements(matches, replacements) ;

		BOOST_CHECK_EQUAL(5u, replacements.size()) ;
		BOOST_CHECK_EQUAL(replacements[3], std::make_pair(matches[0], matches[0])) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( test_regex_rules )

	using namespace mem_engine ;
	namespace rp = placement ;

	BOOST_AUTO_TEST_CASE( test_create_regex_rules )
	{
		rp::regex_rules rules ;
		BOOST_CHECK_EQUAL(0u, rules.m_rules.size()) ;
	}
	BOOST_AUTO_TEST_CASE( test_make_match)
	{
		rp::regex_rules rules ;
		search_match_ptr match = rules.make_match(L"source", L"trans") ;

		BOOST_CHECK_EQUAL(match->get_memory_id(), RULE_MEMORY_ID) ;
		record_pointer rec = match->get_record() ;
		BOOST_CHECK_EQUAL(rec->get_source_rich(), L"source") ;
		BOOST_CHECK_EQUAL(rec->get_trans_plain(), L"trans") ;
	}
	BOOST_AUTO_TEST_CASE(test_get_matches_empty)
	{
		rp::regex_rules rules ;
		search_match_container matches ;
		wstring haystack = L"a 1 bb" ;
		rules.get_matches(matches, haystack) ;
		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_matches)
	{
		rp::regex_ptr rule(new rp::regex_rule(L"Numbers", L"(\\d+)", L"\\1")) ;
		rp::regex_rules rules ;
		rules.m_rules.push_back(rule) ;
		BOOST_CHECK_EQUAL(1u, rules.m_rules.size()) ;
		search_match_container matches ;
		wstring haystack = L"a 1 bb" ;
		rules.get_matches(matches, haystack) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
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
		output_device_ptr output(new OutputDeviceFake) ;
		InputDeviceFake *raw_input = new InputDeviceFake ;
		raw_input->set_view("") ;
		input_device_ptr input(raw_input) ;

		BOOST_CHECK_NO_THROW(rules.load(input, output)) ;
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
			"<target>\\1</target>"
			"<sample>My number 123.456</sample>"
			"<enabled>0</enabled>"
			"</rule>"
			"<rule>"
			"<name>ALL CAPS</name>"
			"<source>([-_A-Z][-_A-Z]+)</source>"
			"<target>\\1</target>"
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
			"<target>\\1</target>"
			"<sample>My number 123.456</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"<rule>"
			"<name>ALL CAPS</name>"
			"<source>([-_A-Z][-_A-Z]+)</source>"
			"<target>\\1</target>"
			"<sample>Some text ALL-CAPS_BIG-NAME.</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"</rules>") ;
		input_device_ptr input(raw_input) ;

		BOOST_CHECK_NO_THROW(rules.load(input, output)) ;
		BOOST_CHECK_EQUAL(2u, rules.m_rules.size()) ;

		placement::regex_ptr rule = rules.m_rules[1] ;

		BOOST_CHECK_EQUAL(rule->m_name, L"ALL CAPS") ;
		BOOST_CHECK_EQUAL(rule->m_expr.str(), L"([-_A-Z][-_A-Z]+)") ;
		BOOST_CHECK_EQUAL(rule->m_repl, L"\\1") ;

	}
	BOOST_AUTO_TEST_CASE( load_file_missing_tags)
	{
		placement::regex_rules rules ;
		output_device_ptr output(new OutputDeviceFake) ;
		InputDeviceFake *raw_input = new InputDeviceFake ;
		raw_input->set_view("<?xml version='1.0' encoding='utf-8'?>"
			"<rules>"
			"<rule>"
			"<name>Numbers</name>"
			"<sample>My number 123.456</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"</rules>") ;
		input_device_ptr input(raw_input) ;

		BOOST_CHECK_NO_THROW(rules.load(input, output)) ;
		BOOST_CHECK_EQUAL(1u, rules.m_rules.size()) ;
		placement::regex_ptr rule = rules.m_rules[0] ;

		BOOST_CHECK_EQUAL(rule->m_name, L"Numbers") ;
		BOOST_CHECK_EQUAL(rule->m_expr.str(), L"") ;
		BOOST_CHECK_EQUAL(rule->m_repl, L"") ;

	}
	BOOST_AUTO_TEST_CASE(test_reload_clears)
	{
		placement::regex_rules rules ;
		output_device_ptr output(new OutputDeviceFake) ;
		InputDeviceFake *raw_input1 = new InputDeviceFake ;
		raw_input1->set_view("<?xml version='1.0' encoding='utf-8'?>"
			"<rules>"
			"<rule>"
			"<name>Numbers</name>"
			"<source>(\\d+(\\.\\d+)?)</source>"
			"<target>\\1</target>"
			"<sample>My number 123.456</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"</rules>") ;
		input_device_ptr input1(raw_input1) ;

		BOOST_CHECK_NO_THROW(rules.load(input1, output)) ;
		BOOST_CHECK_EQUAL(1u, rules.m_rules.size()) ;

		InputDeviceFake *raw_input2 = new InputDeviceFake ;
		raw_input2->set_view("<?xml version='1.0' encoding='utf-8'?>"
			"<rules>"
			"<rule>"
			"<name>ALL CAPS</name>"
			"<source>([-_A-Z][-_A-Z]+)</source>"
			"<target>\\1</target>"
			"<sample>Some text ALL-CAPS_BIG-NAME.</sample>"
			"<enabled>1</enabled>"
			"</rule>"
			"</rules>") ;
		input_device_ptr input2(raw_input2) ;

		BOOST_CHECK_NO_THROW(rules.load(input2, output)) ;
		BOOST_CHECK_EQUAL(1u, rules.m_rules.size()) ;

		placement::regex_ptr rule = rules.m_rules[0] ;

		BOOST_CHECK_EQUAL(rule->m_name, L"ALL CAPS") ;
		BOOST_CHECK_EQUAL(rule->m_expr.str(), L"([-_A-Z][-_A-Z]+)") ;
		BOOST_CHECK_EQUAL(rule->m_repl, L"\\1") ;

	}


BOOST_AUTO_TEST_SUITE_END()



#endif