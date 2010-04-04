
#include "StdAfx.h"
#include "query.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{

	using namespace mem_engine ;

	SimpleString get_dummy_text()
	{
		string etext = "	<div class=\"nomatch_query\">\r\n" ;
		etext += "		<span class=\"match_content\" id=\"query\" style=\"color: #000\"></span>\r\n" ;
		etext += "    </div>\r\n" ;
		etext += "	<div class=\"nomatch_box\">\r\n" ;
		etext += "		No Matches\r\n" ;
		etext += "	</div>" ;
		return SimpleString(etext.c_str()) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// search_query_mainframe
	//////////////////////////////////////////////////////////////////////////

	TEST(test_search_query_mainframe, get_html_short_throws)
	{
		search_query_mainframe query ;
		try
		{
			query.get_html_short() ;
			FAIL_M("Should have thrown here") ;
		}
		catch (std::logic_error&)
		{
			ASSERT_TRUE(true) ;
		}
	}

	TEST(test_search_query_mainframe, show_marking)
	{
		search_query_mainframe query ;
		ASSERT_TRUE(query.show_marking()) ;
		query.set_marking_off() ;
		ASSERT_TRUE(! query.show_marking()) ;
		query.set_marking_on() ;
		ASSERT_TRUE(query.show_marking()) ;
	}
	// query
	TEST(test_search_query_mainframe, plain_query)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_query_plain()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring text = L"spam" ;

		query.set_query_plain(text) ;
		actual = string2string(query.get_query_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_query)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_query_rich()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring text = L"<b>spam</b>" ;

		query.set_query_rich(text) ;
		actual = string2string(query.get_query_rich()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("<b>spam</b>"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_to_plain_query)
	{
		search_query_mainframe query ;

		wstring text = L"<b>spam</b>" ;

		query.set_query_rich(text) ;
		SimpleString actual = string2string(query.get_query_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	TEST(test_search_query_mainframe, source_is_query)
	{
		search_query_mainframe query ;
		wstring source = L"spam" ;

		query.set_source(source) ;
		SimpleString actual(string2string(query.get_query_rich()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	// source
	TEST(test_search_query_mainframe, plain_source)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_source_plain()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring source = L"spam" ;

		query.set_source(source) ;
		actual = string2string(query.get_source_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_source)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring source = L"<b>spam</b>" ;

		query.set_source(source) ;
		actual = string2string(query.get_source_rich()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("<b>spam</b>"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_to_plain_source)
	{
		search_query_mainframe query ;

		wstring source = L"<b>spam</b>" ;

		query.set_source(source) ;
		SimpleString actual = string2string(query.get_source_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	// trans
	TEST(test_search_query_mainframe, plain_trans)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_trans_plain()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring trans = L"spam" ;

		query.set_trans(trans) ;
		actual = string2string(query.get_trans_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_trans)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_trans_rich()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring trans = L"<b>spam</b>" ;

		query.set_trans(trans) ;
		actual = string2string(query.get_trans_rich()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("<b>spam</b>"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_to_plain_trans)
	{
		search_query_mainframe query ;

		wstring trans = L"<b>spam</b>" ;

		query.set_trans(trans) ;
		SimpleString actual = string2string(query.get_trans_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	// context
	TEST(test_search_query_mainframe, plain_context)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_context_plain()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring context = L"spam" ;

		query.set_context(context) ;
		actual = string2string(query.get_context_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_context)
	{
		search_query_mainframe query ;
		SimpleString actual(string2string(query.get_context_rich()).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(), actual) ;

		wstring context = L"<b>spam</b>" ;

		query.set_context(context) ;
		actual = string2string(query.get_context_rich()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("<b>spam</b>"), actual) ;
	}
	TEST(test_search_query_mainframe, rich_to_plain_context)
	{
		search_query_mainframe query ;

		wstring context = L"<b>spam</b>" ;

		query.set_context(context) ;
		SimpleString actual = string2string(query.get_context_plain()).c_str() ;
		ASSERT_EQUALS_V(SimpleString("spam"), actual) ;
	}

	// others
	TEST(test_search_query_mainframe, initial_match_conditions)
	{
		search_query_mainframe query ;
		ASSERT_EQUALS_V(0, (int)query.size()) ;
		ASSERT_TRUE(query.empty()) ;
	}
	TEST(test_search_query_mainframe, set_matches_smm_empty)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		query.set_matches(matches) ;
		ASSERT_TRUE(query.empty()) ;
	}
	TEST(test_search_query_mainframe, set_matches_smm_one)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match) ;
		matches.insert(m1) ;
		query.set_matches(matches) ;
		ASSERT_TRUE(! query.empty()) ;
		ASSERT_EQUALS_V(1, (int)query.size()) ;
	}
	TEST(test_search_query_mainframe, forward_empty)
	{
		search_query_mainframe query ;
		query.forward() ;
		ASSERT_EQUALS_V(0, (int)query.current_pos()) ;
	}
	TEST(test_search_query_mainframe, forward_two_matches)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		ASSERT_EQUALS_V(0, (int)query.current_pos()) ;
		query.forward() ;
		ASSERT_EQUALS_V(1, (int)query.current_pos()) ;
		query.forward() ;
		ASSERT_EQUALS_V(0, (int)query.current_pos()) ;
	}
	TEST(test_search_query_mainframe, back_empty)
	{
		search_query_mainframe query ;
		query.back() ;
		ASSERT_EQUALS_V(0, (int)query.current_pos()) ;
	}
	TEST(test_search_query_mainframe, back_three_matches)
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
		ASSERT_EQUALS_V(0, (int)query.current_pos()) ;
		query.back() ;
		ASSERT_EQUALS_V(2, (int)query.current_pos()) ;
		query.back() ;
		ASSERT_EQUALS_V(1, (int)query.current_pos()) ;
		query.back() ;
		ASSERT_EQUALS_V(0, (int)query.current_pos()) ;
	}

	TEST(test_search_query_mainframe, erase_current)
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
		ASSERT_EQUALS_DELTA_V(1.0, match->get_base_score(), 0.001) ;
	}
	TEST(test_search_query_mainframe, set_current)
	{
		search_query_mainframe query ;
		search_match_container matches ;
		search_match_ptr m1(new search_match), m2(new search_match) ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		query.set_matches(matches) ;
		query.set_current(1) ;
		ASSERT_EQUALS_V(1, (int)query.current_pos()) ;
	}

	TEST(test_search_query_mainframe, make_id_cell)
	{
		search_query_mainframe query ;
		wstring val = L"spam" ;
		wstring text = query.make_id_cell(IDS_CONTEXT_ID, val) ;
		SimpleString actual = string2string(text).c_str() ;
		SimpleString expected = "<td class=\"match_content\" id=\"context\">spam</td>\n" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_search_query_mainframe, make_table_header)
	{
		search_query_mainframe query ;
		SimpleString actual = string2string(query.make_table_header()).c_str() ;
		SimpleString expected = "<table class=\"base\">\n" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// search_query_glossary
	//////////////////////////////////////////////////////////////////////////
	TEST( test_search_query_glossary, get_html_short )
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
			SimpleString actual = string2string(items[i]).c_str() ;
			SimpleString expected = outitems[i].c_str() ;

			ASSERT_EQUALS_V(expected, actual) ;
		}
		ASSERT_EQUALS_V((int)items.size(), (int)outitems.size()) ;
	}

	TEST( test_search_query_glossary, get_fname_from_loc_with_root )
	{
		wstring loc = L"c:\\text.txt" ;

		CStringA memory_name = get_fname_from_loc(loc).c_str() ;
		ASSERT_EQUALS_V( "text.txt", SimpleString(memory_name) ) ;

	}
	TEST( test_search_query_glossary, get_fname_from_loc )
	{
		wstring loc = L"text.xml" ;

		CStringA memory_name = get_fname_from_loc(loc).c_str() ;
		ASSERT_EQUALS_V( "text.xml", SimpleString(memory_name) ) ;

	}
	TEST( test_search_query_glossary, get_mem_name_empty )
	{
		search_match_ptr match(new search_match) ;

		search_query_glossary query ;
		wstring name = query.get_mem_name(match) ;
		SimpleString actual = string2string(name).c_str() ;
		SimpleString expected = "New" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// translation_match_query
	//////////////////////////////////////////////////////////////////////////

	TEST( test_translation_match_query, get_mem_name_spam )
	{
		translation_match_query query ;
		wstring mem_name = query.get_mem_name(L"spam") ;
		ASSERT_EQUALS(L"spam", mem_name) ;
	}
	TEST( test_translation_match_query, get_mem_name_c_spam )
	{
		translation_match_query query ;
		wstring mem_name = query.get_mem_name(L"c:\\spam") ;
		ASSERT_EQUALS(L"spam", mem_name) ;
	}

	TEST(test_translation_match_query, create_dummy_match)
	{
		translation_match_query query ;
		wstring text = query.create_dummy_match() ;
		SimpleString actual = string2string(text).c_str() ;
		SimpleString expected = get_dummy_text() ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST(test_translation_match_query, get_html_short_dummy)
	{
		translation_match_query query ;
		wstring text = query.get_html_short() ;
		SimpleString actual = string2string(text).c_str() ;
		SimpleString expected = get_dummy_text() ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_translation_match_query, get_html_long_dummy)
	{
		translation_match_query query ;
		wstring text = query.get_html_long() ;
		SimpleString actual = string2string(text).c_str() ;
		SimpleString expected = get_dummy_text() ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_translation_match_query, get_html_long_doesnt_blow_up)
	{
		translation_match_query query ;
		search_match_ptr m1(new search_match) ;
		search_match_container matches ;
		matches.insert(m1) ;
 		query.set_matches(matches) ;
		wstring text = query.get_html_long() ;
		ASSERT_TRUE(text.find(L"<table") != wstring::npos) ;
	}
	TEST(test_translation_match_query, get_html_all_dummy)
	{
		translation_match_query query ;
		wstring text = query.get_html_all() ;
		SimpleString actual = string2string(text).c_str() ;
		SimpleString expected = get_dummy_text() ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_translation_match_query, get_html_all_doesnt_blow_up)
	{
		translation_match_query query ;
		search_match_ptr m1(new search_match) ;
		search_match_container matches ;
		matches.insert(m1) ;
 		query.set_matches(matches) ;
		wstring text = query.get_html_all() ;
		ASSERT_TRUE(text.find(L"<table") != wstring::npos) ;
	}

	// prev/next scores
	TEST(test_translation_match_query, prev_no_formatting_penalty)
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

		SimpleString actual = string2string(query.prev_score()).c_str() ;
		SimpleString expected = "(100%)" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_translation_match_query, next_no_formatting_penalty)
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

		SimpleString actual = string2string(query.next_score()).c_str() ;
		SimpleString expected = "(75%)" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_translation_match_query, prev_formatting_penalty)
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

		SimpleString actual = string2string(query.prev_score()).c_str() ;
		SimpleString expected = "(40% <font color=\"#888888\">[F]</font>)" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_translation_match_query, next_formatting_penalty)
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

		SimpleString actual = string2string(query.next_score()).c_str() ;
		SimpleString expected = "(90% <font color=\"#888888\">[F]</font>)" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

}


#endif // #ifdef _DEBUG
