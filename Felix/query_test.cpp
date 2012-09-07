
#include "StdAfx.h"
#include "query.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

wstring get_dummy_text()
{
	wstring etext = L"	<div class=\"nomatch_query\">\r\n"
	L"		<span class=\"match_content\" id=\"query\" style=\"color: #000\"></span>\r\n" 
	L"    </div>\r\n" 
	L"	<div class=\"nomatch_box\">\r\n" 
	L"		No Matches\r\n" 
	L"	</div>" ;
	return etext ;
}

bool is_critical( std::logic_error const& ) { return true; }

BOOST_AUTO_TEST_SUITE( test_search_query_mainframe )

	using namespace mem_engine ;

	//////////////////////////////////////////////////////////////////////////
	// search_query_mainframe
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( get_html_short_throws)
	{
		search_query_mainframe query ;
		BOOST_CHECK_EXCEPTION(query.get_html_short(), std::logic_error, is_critical) ;
	}

	BOOST_AUTO_TEST_CASE( show_marking)
	{
		search_query_mainframe query ;
		BOOST_CHECK(query.show_marking()) ;
		query.set_marking_off() ;
		BOOST_CHECK(! query.show_marking()) ;
		query.set_marking_on() ;
		BOOST_CHECK(query.show_marking()) ;
	}
	// query
	BOOST_AUTO_TEST_CASE( plain_query)
	{
		search_query_mainframe query ;
		wstring actual(query.get_query_plain()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring text = L"spam" ;

		query.set_query_plain(text) ;
		actual = query.get_query_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_query)
	{
		search_query_mainframe query ;
		wstring actual(query.get_query_rich()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring text = L"<b>spam</b>" ;

		query.set_query_rich(text) ;
		actual = query.get_query_rich() ;
		BOOST_CHECK_EQUAL(wstring(L"<b>spam</b>"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_to_plain_query)
	{
		search_query_mainframe query ;

		wstring text = L"<b>spam</b>" ;

		query.set_query_rich(text) ;
		wstring actual = query.get_query_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( source_is_query)
	{
		search_query_mainframe query ;
		wstring source = L"spam" ;

		query.set_source(source) ;
		wstring actual(query.get_query_rich()) ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	// source
	BOOST_AUTO_TEST_CASE( plain_source)
	{
		search_query_mainframe query ;
		wstring actual(query.get_source_plain()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring source = L"spam" ;

		query.set_source(source) ;
		actual = query.get_source_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_source)
	{
		search_query_mainframe query ;
		wstring actual(query.get_source_rich()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring source = L"<b>spam</b>" ;

		query.set_source(source) ;
		actual = query.get_source_rich() ;
		BOOST_CHECK_EQUAL(wstring(L"<b>spam</b>"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_to_plain_source)
	{
		search_query_mainframe query ;

		wstring source = L"<b>spam</b>" ;

		query.set_source(source) ;
		wstring actual = query.get_source_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	// trans
	BOOST_AUTO_TEST_CASE( plain_trans)
	{
		search_query_mainframe query ;
		wstring actual(query.get_trans_plain()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring trans = L"spam" ;

		query.set_trans(trans) ;
		actual = query.get_trans_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_trans)
	{
		search_query_mainframe query ;
		wstring actual(query.get_trans_rich()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring trans = L"<b>spam</b>" ;

		query.set_trans(trans) ;
		actual = query.get_trans_rich() ;
		BOOST_CHECK_EQUAL(wstring(L"<b>spam</b>"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_to_plain_trans)
	{
		search_query_mainframe query ;

		wstring trans = L"<b>spam</b>" ;

		query.set_trans(trans) ;
		wstring actual = query.get_trans_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	// context
	BOOST_AUTO_TEST_CASE( plain_context)
	{
		search_query_mainframe query ;
		wstring actual(query.get_context_plain()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring context = L"spam" ;

		query.set_context(context) ;
		actual = query.get_context_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_context)
	{
		search_query_mainframe query ;
		wstring actual(query.get_context_rich()) ;
		BOOST_CHECK_EQUAL(wstring(), actual) ;

		wstring context = L"<b>spam</b>" ;

		query.set_context(context) ;
		actual = query.get_context_rich() ;
		BOOST_CHECK_EQUAL(wstring(L"<b>spam</b>"), actual) ;
	}
	BOOST_AUTO_TEST_CASE( rich_to_plain_context)
	{
		search_query_mainframe query ;

		wstring context = L"<b>spam</b>" ;

		query.set_context(context) ;
		wstring actual = query.get_context_plain() ;
		BOOST_CHECK_EQUAL(wstring(L"spam"), actual) ;
	}

	// others
	BOOST_AUTO_TEST_CASE( initial_match_conditions)
	{
		search_query_mainframe query ;
		BOOST_CHECK_EQUAL(0u, query.size()) ;
		BOOST_CHECK(query.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( set_matches_smm_empty)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		query.set_matches(matches) ;
		BOOST_CHECK(query.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( set_matches_smm_one)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match) ;
		matches.insert(m1) ;
		query.set_matches(matches) ;
		BOOST_CHECK(! query.empty()) ;
		BOOST_CHECK_EQUAL(1u, query.size()) ;
	}

	BOOST_AUTO_TEST_CASE(add_match)
	{
		search_query_mainframe matches ;
		search_match_ptr m1(new search_match) ;
		matches.add_match(m1) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( forward_empty)
	{
		search_query_mainframe query ;
		query.forward() ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
	}
	BOOST_AUTO_TEST_CASE( forward_two_matches)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
		query.forward() ;
		BOOST_CHECK_EQUAL(1u, query.current_pos()) ;
		query.forward() ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
	}
	BOOST_AUTO_TEST_CASE( back_empty)
	{
		search_query_mainframe query ;
		query.back() ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
	}
	BOOST_AUTO_TEST_CASE( back_three_matches)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), 
			m2(new search_match), 
			m3(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
		query.back() ;
		BOOST_CHECK_EQUAL(2u, query.current_pos()) ;
		query.back() ;
		BOOST_CHECK_EQUAL(1u, query.current_pos()) ;
		query.back() ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
	}

	BOOST_AUTO_TEST_CASE( erase_current)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		m1->set_base_score(1.0) ;
		m2->set_base_score(0.0) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		query.forward() ;
		query.erase_current() ;
		search_match_ptr match = query.current() ;
		BOOST_CHECK_CLOSE(1.0, match->get_base_score(), 0.001) ;
	}
	BOOST_AUTO_TEST_CASE( set_current)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		query.set_current(1) ;
		BOOST_CHECK_EQUAL(1u, query.current_pos()) ;
	}
	BOOST_AUTO_TEST_CASE( set_current_for_emtpy)
	{
		search_query_mainframe query ;
		query.set_current(1) ;
		BOOST_CHECK_EQUAL(0u, query.current_pos()) ;
	}

	BOOST_AUTO_TEST_CASE( erase_at)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		BOOST_CHECK_EQUAL(2u, query.size()) ;
		query.erase_at(1) ;
		BOOST_CHECK_EQUAL(1u, query.size()) ;
	}
	BOOST_AUTO_TEST_CASE( erase_at_for_empty)
	{
		search_query_mainframe query ;
		query.erase_at(1) ;
		BOOST_CHECK_EQUAL(0u, query.size()) ;
	}
	BOOST_AUTO_TEST_CASE( erase_at_beyond_bounds)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		query.erase_at(5) ;
		BOOST_CHECK_EQUAL(2u, query.size()) ;
	}

	BOOST_AUTO_TEST_CASE( make_id_cell)
	{
		search_query_mainframe query ;
		wstring val = L"spam" ;
		wstring text = query.make_id_cell(IDS_CONTEXT_ID, val) ;
		wstring actual = text ;
		wstring expected = L"<td class=\"match_content\" id=\"context\">spam</td>\n" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( make_table_header)
	{
		search_query_mainframe query ;
		wstring actual = query.make_table_header() ;
		wstring expected = L"<table class=\"base\">\n" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	// prev_match_pos
	BOOST_AUTO_TEST_CASE( get_prev_match_pos_0)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		query.set_current(0) ;

		BOOST_CHECK_EQUAL(1u, query.prev_match_pos()) ;
	}

	BOOST_AUTO_TEST_CASE( get_prev_match_pos_mid)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match), m3(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;
		query.set_current(1) ;

		BOOST_CHECK_EQUAL(0u, query.prev_match_pos()) ;
	}
	BOOST_AUTO_TEST_CASE( get_next_match_pos_0)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		query.set_current(0) ;

		BOOST_CHECK_EQUAL(1u, query.next_match_pos()) ;
	}

	BOOST_AUTO_TEST_CASE( get_next_match_pos_end)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match), m3(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;
		query.set_current(2) ;

		BOOST_CHECK_EQUAL(0u, query.next_match_pos()) ;
	}
	BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE( test_search_query_glossary )

	using namespace mem_engine ;

	//////////////////////////////////////////////////////////////////////////
	// search_query_glossary
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( get_html_short )
	{
		search_query_glossary query ;
		search_match_ptr m1(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		m1->set_record(rec) ;
		m1->set_values_to_record() ;
		m1->set_memory_location(L"c:\\foo.xml") ;
		search_match_container matches ;
		matches.insert(m1) ;
		query.set_matches(matches) ;

		wstring html = query.get_html_short() ;
		BOOST_CHECK(html.find(L"<strong>1.</strong>") != wstring::npos) ;
		BOOST_CHECK(html.find(L"<td id=\"source\">spam</td></tr>") != wstring::npos) ;
		BOOST_CHECK(html.find(L"<td id=\"trans\">egg</td>") != wstring::npos) ;
		BOOST_CHECK(html.find(L"<a title=\"Edit entry\" href=\"142:0\">[E]</a>") != wstring::npos) ;
	}

	BOOST_AUTO_TEST_CASE( get_fname_from_loc_with_root )
	{
		wstring loc = L"c:\\text.txt" ;

		CStringA memory_name = get_fname_from_loc(loc).c_str() ;
		BOOST_CHECK_EQUAL( "text", string(memory_name) ) ;

	}
	BOOST_AUTO_TEST_CASE( test_get_fname_from_loc )
	{
		wstring loc = L"text.xml" ;

		CStringA memory_name = get_fname_from_loc(loc).c_str() ;
		BOOST_CHECK_EQUAL( "text", string(memory_name) ) ;

	}
	BOOST_AUTO_TEST_CASE( get_mem_name_empty )
	{
		search_match_ptr match(new search_match) ;

		search_query_glossary query ;
		wstring actual = query.get_mem_name(match) ;
		wstring expected = L"New" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// translation_match_query
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE( test_translation_match_query )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( get_mem_name_spam )
	{
		translation_match_query query ;
		wstring mem_name = query.get_mem_name(L"spam") ;
		BOOST_CHECK_EQUAL(L"spam", mem_name) ;
	}
	BOOST_AUTO_TEST_CASE( get_mem_name_c_spam )
	{
		translation_match_query query ;
		wstring mem_name = query.get_mem_name(L"c:\\spam") ;
		BOOST_CHECK_EQUAL(L"spam", mem_name) ;
	}

	BOOST_AUTO_TEST_CASE( create_dummy_match)
	{
		translation_match_query query ;
		wstring text = query.create_dummy_match() ;
		wstring actual = text ;
		wstring expected = get_dummy_text() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_html_short_dummy)
	{
		translation_match_query query ;
		wstring text = query.get_html_short() ;
		wstring actual = text ;
		wstring expected = get_dummy_text() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_html_long_dummy)
	{
		translation_match_query query ;
		wstring text = query.get_html_long() ;
		wstring actual = text ;
		wstring expected = get_dummy_text() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_html_long_doesnt_blow_up)
	{
		translation_match_query query ;
		search_match_ptr m1(new search_match) ;
		search_match_container matches ;
		matches.insert(m1) ;
		query.set_matches(matches) ;
		wstring text = query.get_html_long() ;
		BOOST_CHECK(text.find(L"<table") != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( get_html_all_dummy)
	{
		translation_match_query query ;
		wstring text = query.get_html_all() ;
		wstring actual = text ;
		wstring expected = get_dummy_text() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_html_all_doesnt_blow_up)
	{
		translation_match_query query ;
		search_match_ptr m1(new search_match) ;
		search_match_container matches ;
		matches.insert(m1) ;
		query.set_matches(matches) ;
		wstring text = query.get_html_all() ;
		BOOST_CHECK(text.find(L"<table") != wstring::npos) ;
	}

	// prev/next scores
	BOOST_AUTO_TEST_CASE( prev_no_formatting_penalty)
	{
		translation_match_query query ;
		query.m_params.m_assess_format_penalty = true ;
		search_match_ptr m1(new search_match), 
			m2(new search_match),
			m3(new search_match) ;
		m1->set_base_score(0.5) ;
		m2->set_base_score(0.75) ;
		m3->set_base_score(1.0) ;
		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;

		wstring actual = query.prev_score() ;
		wstring expected = L"(100%)" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( prev_empty)
	{
		translation_match_query query ;
		wstring actual = query.prev_score() ;
		wstring expected = L"(0%)" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE( next_no_formatting_penalty)
	{
		translation_match_query query ;
		query.m_params.m_assess_format_penalty = true ;
		search_match_ptr m1(new search_match), 
			m2(new search_match),
			m3(new search_match) ;
		m1->set_base_score(0.5) ;
		m2->set_base_score(0.75) ;
		m3->set_base_score(1.0) ;
		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;

		wstring actual = query.next_score() ;
		wstring expected = L"(75%)" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( prev_formatting_penalty)
	{
		translation_match_query query ;
		query.m_params.m_assess_format_penalty = true ;
		search_match_ptr m1(new search_match), 
			m2(new search_match),
			m3(new search_match) ;
		m1->set_base_score(0.5) ;
		m1->set_formatting_penalty(0.1) ;
		m2->set_base_score(0.75) ;
		m3->set_base_score(1.0) ;
		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;
		query.set_current(1) ;

		wstring actual = query.prev_score() ;
		wstring expected = L"(40% <span class=\"format_penalty\">[F]</span>)" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( next_formatting_penalty)
	{
		translation_match_query query ;
		query.m_params.m_assess_format_penalty = true ;
		search_match_ptr m1(new search_match), 
			m2(new search_match),
			m3(new search_match) ;
		m1->set_base_score(0.5) ;
		m2->set_base_score(0.75) ;
		m3->set_base_score(1.0) ;
		m3->set_formatting_penalty(0.1) ;
		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		matches.insert(m3) ;
		query.set_matches(matches) ;
		query.set_current(1) ;

		wstring actual = query.next_score() ;
		wstring expected = L"(90% <span class=\"format_penalty\">[F]</span>)" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( next_empty)
	{
		translation_match_query query ;
		wstring actual = query.next_score() ;
		wstring expected = L"(0%)" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}	

	BOOST_AUTO_TEST_CASE( score_text_plain)
	{
		translation_match_query query ;
		search_match_ptr match(new search_match) ;
		match->set_base_score(.75) ;

		wstring actual = query.get_score_text(match) ;
		wstring expected = L"75%" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE( score_text_formatting)
	{
		translation_match_query query ;
		query.m_params.m_assess_format_penalty = true; 
		search_match_ptr match(new search_match) ;
		match->set_base_score(1.0) ;
		match->set_formatting_penalty(.1) ;

		wstring actual = query.get_score_text(match) ;
		wstring expected = L"90% <span class=\"format_penalty\">[F]</span>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE( score_text_placement)
	{
		translation_match_query query ;
		query.m_params.m_assess_format_penalty = false; 
		search_match_ptr match(new search_match) ;
		match->set_base_score(.75) ;
		match->set_placement_on() ;

		wstring actual = query.get_score_text(match) ;
		wstring expected = L"75% <span class=\"format_penalty\">[P]</span>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}	

BOOST_AUTO_TEST_SUITE_END()
