
#include "StdAfx.h"
#include "query.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

string get_dummy_text()
{
	string etext = "	<div class=\"nomatch_query\">\r\n" ;
	etext += "		<span class=\"match_content\" id=\"query\" style=\"color: #000\"></span>\r\n" ;
	etext += "    </div>\r\n" ;
	etext += "	<div class=\"nomatch_box\">\r\n" ;
	etext += "		No Matches\r\n" ;
	etext += "	</div>" ;
	return string(etext.c_str()) ;
}

BOOST_AUTO_TEST_SUITE( test_search_query_mainframe )

					  using namespace mem_engine ;

//////////////////////////////////////////////////////////////////////////
// search_query_mainframe
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( get_html_short_throws)
{
	search_query_mainframe query ;
	try
	{
		query.get_html_short() ;
		BOOST_FAIL("Should have thrown here") ;
	}
	catch (std::logic_error&)
	{
		BOOST_CHECK(true) ;
	}
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
	string actual(string2string(query.get_query_plain()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring text = L"spam" ;

	query.set_query_plain(text) ;
	actual = string2string(query.get_query_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_query)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_query_rich()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring text = L"<b>spam</b>" ;

	query.set_query_rich(text) ;
	actual = string2string(query.get_query_rich()).c_str() ;
	BOOST_CHECK_EQUAL(string("<b>spam</b>"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_to_plain_query)
{
	search_query_mainframe query ;

	wstring text = L"<b>spam</b>" ;

	query.set_query_rich(text) ;
	string actual = string2string(query.get_query_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
BOOST_AUTO_TEST_CASE( source_is_query)
{
	search_query_mainframe query ;
	wstring source = L"spam" ;

	query.set_source(source) ;
	string actual(string2string(query.get_query_rich()).c_str()) ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
// source
BOOST_AUTO_TEST_CASE( plain_source)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_source_plain()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring source = L"spam" ;

	query.set_source(source) ;
	actual = string2string(query.get_source_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_source)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_source_rich()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring source = L"<b>spam</b>" ;

	query.set_source(source) ;
	actual = string2string(query.get_source_rich()).c_str() ;
	BOOST_CHECK_EQUAL(string("<b>spam</b>"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_to_plain_source)
{
	search_query_mainframe query ;

	wstring source = L"<b>spam</b>" ;

	query.set_source(source) ;
	string actual = string2string(query.get_source_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
// trans
BOOST_AUTO_TEST_CASE( plain_trans)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_trans_plain()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring trans = L"spam" ;

	query.set_trans(trans) ;
	actual = string2string(query.get_trans_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_trans)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_trans_rich()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring trans = L"<b>spam</b>" ;

	query.set_trans(trans) ;
	actual = string2string(query.get_trans_rich()).c_str() ;
	BOOST_CHECK_EQUAL(string("<b>spam</b>"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_to_plain_trans)
{
	search_query_mainframe query ;

	wstring trans = L"<b>spam</b>" ;

	query.set_trans(trans) ;
	string actual = string2string(query.get_trans_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
// context
BOOST_AUTO_TEST_CASE( plain_context)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_context_plain()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring context = L"spam" ;

	query.set_context(context) ;
	actual = string2string(query.get_context_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_context)
{
	search_query_mainframe query ;
	string actual(string2string(query.get_context_rich()).c_str()) ;
	BOOST_CHECK_EQUAL(string(), actual) ;

	wstring context = L"<b>spam</b>" ;

	query.set_context(context) ;
	actual = string2string(query.get_context_rich()).c_str() ;
	BOOST_CHECK_EQUAL(string("<b>spam</b>"), actual) ;
}
BOOST_AUTO_TEST_CASE( rich_to_plain_context)
{
	search_query_mainframe query ;

	wstring context = L"<b>spam</b>" ;

	query.set_context(context) ;
	string actual = string2string(query.get_context_plain()).c_str() ;
	BOOST_CHECK_EQUAL(string("spam"), actual) ;
}

// others
BOOST_AUTO_TEST_CASE( initial_match_conditions)
{
	search_query_mainframe query ;
	BOOST_CHECK_EQUAL(0, (int)query.size()) ;
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
	BOOST_CHECK_EQUAL(1, (int)query.size()) ;
}
BOOST_AUTO_TEST_CASE( forward_empty)
{
	search_query_mainframe query ;
	query.forward() ;
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
}
BOOST_AUTO_TEST_CASE( forward_two_matches)
{
	search_query_mainframe query ;
	search_match_container matches ;
	search_match_ptr m1(new search_match), m2(new search_match) ;
	matches.insert(m1) ;
	matches.insert(m2) ;
	query.set_matches(matches) ;
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
	query.forward() ;
	BOOST_CHECK_EQUAL(1, (int)query.current_pos()) ;
	query.forward() ;
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
}
BOOST_AUTO_TEST_CASE( back_empty)
{
	search_query_mainframe query ;
	query.back() ;
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
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
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
	query.back() ;
	BOOST_CHECK_EQUAL(2, (int)query.current_pos()) ;
	query.back() ;
	BOOST_CHECK_EQUAL(1, (int)query.current_pos()) ;
	query.back() ;
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
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
	BOOST_CHECK_EQUAL(1, (int)query.current_pos()) ;
}
BOOST_AUTO_TEST_CASE( set_current_for_emtpy)
{
	search_query_mainframe query ;
	query.set_current(1) ;
	BOOST_CHECK_EQUAL(0, (int)query.current_pos()) ;
}

BOOST_AUTO_TEST_CASE( erase_at)
{
	search_query_mainframe query ;
	search_match_container matches ;
	search_match_ptr m1(new search_match), m2(new search_match) ;
	matches.insert(m1) ;
	matches.insert(m2) ;
	query.set_matches(matches) ;
	BOOST_CHECK_EQUAL(2, (int)query.size()) ;
	query.erase_at(1) ;
	BOOST_CHECK_EQUAL(1, (int)query.size()) ;
}
BOOST_AUTO_TEST_CASE( erase_at_for_empty)
{
	search_query_mainframe query ;
	query.erase_at(1) ;
	BOOST_CHECK_EQUAL(0, (int)query.size()) ;
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
	BOOST_CHECK_EQUAL(2, (int)query.size()) ;
}

BOOST_AUTO_TEST_CASE( make_id_cell)
{
	search_query_mainframe query ;
	wstring val = L"spam" ;
	wstring text = query.make_id_cell(IDS_CONTEXT_ID, val) ;
	string actual = string2string(text).c_str() ;
	string expected = "<td class=\"match_content\" id=\"context\">spam</td>\n" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( make_table_header)
{
	search_query_mainframe query ;
	string actual = string2string(query.make_table_header()).c_str() ;
	string expected = "<table class=\"base\">\n" ;
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

	BOOST_CHECK_EQUAL(1, (int)query.prev_match_pos()) ;
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

	BOOST_CHECK_EQUAL(0, (int)query.prev_match_pos()) ;
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

	BOOST_CHECK_EQUAL(1, (int)query.next_match_pos()) ;
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

	BOOST_CHECK_EQUAL(0, (int)query.next_match_pos()) ;
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
	textstream_reader<wchar_t> reader(html.c_str()) ;
	std::vector<wstring> items ;
	reader.split(items, L" \t\n\r") ;
	string text ;
	text += "<table class=\"base\"> <tr id=\"0\"> " ;
	text += "<td rowspan=\"2\">" ;
	text += "<strong>1.</strong><br /> " ;
	text += "<a title=\"Edit entry\" href=\"142:0\">[E]</a>-<a title=\"Delete entry\" href=\"122:0\">[D]</a>-<a title=\"Add entry to memory\" href=\"158:0\">[M]</a><br /> " ;
	text += "[foo.xml]</td> " ;
	text += "<td><strong>Source</strong></td> " ;
	text += "<td id=\"source\">spam</td></tr> " ;
	text += "<tr> " ;
	text += "<td><strong>Trans</strong></td> " ;
	text += "<td id=\"trans\">egg</td>" ;
	text += "</tr> " ;
	text += "</table> " ;
	textstream_reader<char> creader(text.c_str()) ;
	std::vector<string> outitems ;
	creader.split(outitems, " \t\n\r") ;
	for (size_t i=0 ; i < items.size() && i < outitems.size(); ++i)
	{
		string actual = string2string(items[i]).c_str() ;
		string expected = outitems[i].c_str() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_CHECK_EQUAL((int)items.size(), (int)outitems.size()) ;
}

BOOST_AUTO_TEST_CASE( get_fname_from_loc_with_root )
{
	wstring loc = L"c:\\text.txt" ;

	CStringA memory_name = get_fname_from_loc(loc).c_str() ;
	BOOST_CHECK_EQUAL( "text.txt", string(memory_name) ) ;

}
BOOST_AUTO_TEST_CASE( test_get_fname_from_loc )
{
	wstring loc = L"text.xml" ;

	CStringA memory_name = get_fname_from_loc(loc).c_str() ;
	BOOST_CHECK_EQUAL( "text.xml", string(memory_name) ) ;

}
BOOST_AUTO_TEST_CASE( get_mem_name_empty )
{
	search_match_ptr match(new search_match) ;

	search_query_glossary query ;
	wstring name = query.get_mem_name(match) ;
	string actual = string2string(name).c_str() ;
	string expected = "New" ;
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
	string actual = string2string(text).c_str() ;
	string expected = get_dummy_text() ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}

BOOST_AUTO_TEST_CASE( get_html_short_dummy)
{
	translation_match_query query ;
	wstring text = query.get_html_short() ;
	string actual = string2string(text).c_str() ;
	string expected = get_dummy_text() ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( get_html_long_dummy)
{
	translation_match_query query ;
	wstring text = query.get_html_long() ;
	string actual = string2string(text).c_str() ;
	string expected = get_dummy_text() ;
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
	string actual = string2string(text).c_str() ;
	string expected = get_dummy_text() ;
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

	string actual = string2string(query.prev_score()).c_str() ;
	string expected = "(100%)" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( prev_empty)
{
	translation_match_query query ;
	string actual = string2string(query.prev_score()).c_str() ;
	string expected = "(0%)" ;
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

	string actual = string2string(query.next_score()).c_str() ;
	string expected = "(75%)" ;
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

	string actual = string2string(query.prev_score()).c_str() ;
	string expected = "(40% <span class=\"format_penalty\">[F]</span>)" ;
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

	string actual = string2string(query.next_score()).c_str() ;
	string expected = "(90% <span class=\"format_penalty\">[F]</span>)" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}
BOOST_AUTO_TEST_CASE( next_empty)
{
	translation_match_query query ;
	string actual = string2string(query.next_score()).c_str() ;
	string expected = "(0%)" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}	

BOOST_AUTO_TEST_CASE( score_text_plain)
{
	translation_match_query query ;
	search_match_ptr match(new search_match) ;
	match->set_base_score(.75) ;

	string actual = string2string(query.get_score_text(match)).c_str() ;
	string expected = "75%" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}	
BOOST_AUTO_TEST_CASE( score_text_formatting)
{
	translation_match_query query ;
	query.m_params.m_assess_format_penalty = true; 
	search_match_ptr match(new search_match) ;
	match->set_base_score(1.0) ;
	match->set_formatting_penalty(.1) ;

	string actual = string2string(query.get_score_text(match)).c_str() ;
	string expected = "90% <span class=\"format_penalty\">[F]</span>" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}	
BOOST_AUTO_TEST_CASE( score_text_placement)
{
	translation_match_query query ;
	query.m_params.m_assess_format_penalty = false; 
	search_match_ptr match(new search_match) ;
	match->set_base_score(.75) ;
	match->Placement() ;

	string actual = string2string(query.get_score_text(match)).c_str() ;
	string expected = "75% <span class=\"format_penalty\">[P]</span>" ;
	BOOST_CHECK_EQUAL(expected, actual) ;
}	

BOOST_AUTO_TEST_SUITE_END()
