#include "stdafx.h"
#include "rule_placement.h"
#include "output_device_fake.h"
#include "input_device_fake.h"
#include "gloss_placement.h"

#ifdef UNIT_TEST

#include "unit_testing.h"

using namespace mem_engine ;

#define MAKE_A_HOLE(X) 	rp::hole_pair_t X ; \
	rp::hole_finder finder ;\
	BOOST_CHECK(finder.find_hole(pairings, holes)) ;


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
	BOOST_AUTO_TEST_CASE( test_get_replacements_numbers )
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
			BOOST_CHECK_EQUAL(replacements[i].first, matches[i]) ;
			BOOST_CHECK_EQUAL(replacements[i].second, matches[i]) ;
		}
	}

	BOOST_AUTO_TEST_CASE( test_get_replacements_x_to_y )
	{
		placement::regex_rule rule(L"Numbers", L"x", L"y") ;
		wstring haystack = L"a x a" ;
		std::vector<wstring> matches ;
		rule.get_matches(haystack, matches) ;
		std::vector<std::pair<wstring, wstring> > replacements;
		rule.get_replacements(matches, replacements) ;
		BOOST_CHECK_EQUAL(1u, replacements.size()) ;
		BOOST_CHECK_EQUAL(replacements[0].first, placement::repl_t(L"x", L"y").first) ;
		BOOST_CHECK_EQUAL(replacements[0].second, placement::repl_t(L"x", L"y").second) ;
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
		BOOST_CHECK_EQUAL(replacements[3].first, matches[0]) ;
		BOOST_CHECK_EQUAL(replacements[3].second, matches[0]) ;
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


//////////////////////////////////////////////////////////////////////////
// rule_placer
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_rule_placer )

	using namespace mem_engine ;
	namespace rp = placement ;

	BOOST_AUTO_TEST_CASE(init_empty)
	{
		rp::regex_rules rules ;
		rp::rule_placer placer(rules) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		rules.get_matches(matches, text) ;
		BOOST_CHECK(matches.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(get_matches_none)
	{
		rp::regex_rules rules ;
		rules.m_rules.push_back(rp::regex_ptr(new rp::regex_rule(L"Numbers", L"(\\d+)", L"\\1"))) ;
		rp::rule_placer placer(rules) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		rules.get_matches(matches, text) ;
		BOOST_CHECK(matches.empty()) ;
	}



	// num_hits
	BOOST_AUTO_TEST_CASE(test_num_hits_empty_string)
	{
		rp::regex_rules rules ;
		rp::rule_placer placer(rules) ;
		BOOST_CHECK_EQUAL(0u, placer.num_hits(L"", L"bar baz bar")) ;
	}
	BOOST_AUTO_TEST_CASE(test_num_hits_0)
	{
		rp::regex_rules rules ;
		rp::rule_placer placer(rules) ;
		BOOST_CHECK_EQUAL(0u, placer.num_hits(L"foo", L"bar baz bar")) ;
	}
	BOOST_AUTO_TEST_CASE(test_num_hits_1)
	{
		rp::regex_rules rules ;
		rp::rule_placer placer(rules) ;
		BOOST_CHECK_EQUAL(1u, placer.num_hits(L"foo", L"bar foo bar")) ;
	}
	BOOST_AUTO_TEST_CASE(test_num_hits_3)
	{
		rp::regex_rules rules ;
		rp::rule_placer placer(rules) ;
		BOOST_CHECK_EQUAL(3u, placer.num_hits(L"foo", L"bar foofoo bar foo")) ;
	}
	// get_rule_replacement_source
	BOOST_AUTO_TEST_CASE(test_get_rule_replacement_source_num)
	{
		rp::regex_rules rules ;
		rules.m_rules.push_back(rp::regex_ptr(new rp::regex_rule(L"Numbers", L"(\\d\\d)", L"\\1"))) ;
		rp::rule_placer placer(rules) ;

		rp::pairings_t pairings ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'1', rp::NOMATCH, L'2')) ;
		pairings.push_back(rp::pairing_t(L'1', rp::NOMATCH, L'2')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;

		const wstring trans = L"End up with 11" ;

		MAKE_A_HOLE(holes) ;
		rp::repl_t replacement ;
		rp::rule_placement_param param(holes, rules.m_rules[0], L"aa11bb", L"aa22bb") ;
		BOOST_CHECK(placer.get_rule_replacement_source(param, trans)) ;

		BOOST_CHECK_EQUAL(param.s_replacement.first, L"11") ;
		BOOST_CHECK_EQUAL(param.s_replacement.second, L"11") ;

	}
	// get_rule_replacement_query
	BOOST_AUTO_TEST_CASE(test_get_rule_replacement_query_num)
	{
		rp::regex_rules rules ;
		rules.m_rules.push_back(rp::regex_ptr(new rp::regex_rule(L"Numbers", L"(\\d\\d)", L"\\1"))) ;
		rp::rule_placer placer(rules) ;

		rp::pairings_t pairings ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'1', rp::NOMATCH, L'2')) ;
		pairings.push_back(rp::pairing_t(L'1', rp::NOMATCH, L'2')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;

		const wstring trans = L"End up with 22" ;

		MAKE_A_HOLE(holes) ;
		rp::rule_placement_param param(holes, rules.m_rules[0], L"aa11bb", L"aa22bb") ;
		param.s_replacement = rp::repl_t(L"11", L"11") ;
		BOOST_CHECK(placer.get_rule_replacement_query(param)) ;

		BOOST_CHECK_EQUAL(param.q_replacement.first, L"22") ;
		BOOST_CHECK_EQUAL(param.q_replacement.second, L"22") ;

	}
	// create_new_pairings
	BOOST_AUTO_TEST_CASE(create_new_pairings_xx_yy)
	{
		rp::hole_finder finder ;
		rp::pairings_t pairings ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'y', rp::NOMATCH, L'x')) ;
		pairings.push_back(rp::pairing_t(L'y', rp::NOMATCH, L'x')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;

		rp::create_new_pairings(pairings, holes.lhs) ;

		std::vector<rp::pairing_t> pairvec ;
		pairvec.assign(pairings.begin(), pairings.end()) ;

		BOOST_CHECK_EQUAL(pairvec[0].source(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[1].source(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[2].source(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[3].source(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[4].source(), L'b') ;
		BOOST_CHECK_EQUAL(pairvec[5].source(), L'b') ;

		BOOST_CHECK_EQUAL(pairvec[0].query(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[1].query(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[2].query(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[3].query(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[4].query(), L'b') ;
		BOOST_CHECK_EQUAL(pairvec[5].query(), L'b') ;

		BOOST_CHECK_EQUAL(pairvec[2].match_type(), rp::PLACEMENT) ;
	}

	// place
	BOOST_AUTO_TEST_CASE(place_true)
	{
		rp::pairings_t pairings ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'a', rp::MATCH, L'a')) ;
		pairings.push_back(rp::pairing_t(L'1', rp::NOMATCH, L'2')) ;
		pairings.push_back(rp::pairing_t(L'1', rp::NOMATCH, L'2')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;
		pairings.push_back(rp::pairing_t(L'b', rp::MATCH, L'b')) ;

		rp::regex_rules rules ;
		rules.m_rules.push_back(rp::regex_ptr(new rp::regex_rule(L"Numbers", L"\\d\\d", L"foo"))) ;
		rp::rule_placer placer(rules) ;

		const wstring trans = L"End up with foo" ;
		std::pair<wstring, wstring> segs(trans, trans) ;

		MAKE_A_HOLE(holes) ;
		BOOST_CHECK(placer.place(pairings, segs, holes)) ;
		const wstring expected_trans = L"End up with <span class=\"placement\">foo</span>" ;
		BOOST_CHECK_EQUAL(segs.second, expected_trans) ;

		const wstring placed_query = rp::mark_up(pairings, rp::QUERY) ;
		const wstring placed_source = rp::mark_up(pairings, rp::SOURCE) ;
		const wstring expected_query = L"aa<span class=\"placement\">22</span>bb" ;
		const wstring expected_source = L"aa<span class=\"placement\">22</span>bb" ;

		BOOST_CHECK_EQUAL(placed_query, expected_query) ;
		BOOST_CHECK_EQUAL(placed_source, expected_source) ;
	}
	
BOOST_AUTO_TEST_SUITE_END()

#endif