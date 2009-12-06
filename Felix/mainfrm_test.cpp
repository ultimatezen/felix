/*!
	@brief Unit tests for CMainFrame class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "MainFrm.h"
#include "reg_msg_filter.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	void add_record(CMainFrame &mainframe, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mainframe.add_record(rec) ;
	}

	search_match_ptr get_first_match(CMainFrame &mainframe, search_query_params &params)
	{
		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		return *pos ;
	}

	/************************************************************************/
	/* tests start here                                                     */
	/************************************************************************/	
	// Tests for CMainFrame
	TEST( TestCMainFrame, get_record_translation_standard)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		mainframe.m_properties.m_mem_props.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"spam") ;

		wstring normalized = mainframe.get_record_translation(record) ;

		SimpleString expected = "spam" ;
		SimpleString actual = (LPCSTR)CStringA(normalized.c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(1, (int)record->get_refcount()) ;
	}
	TEST( TestCMainFrame, get_record_translation_plain)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		mainframe.m_properties.m_mem_props.m_data.m_plaintext = TRUE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<bold>spam &lt; &amp; &gt; eggs</bold>") ;

		ASSERT_EQUALS(record->get_trans_plain(), L"spam < & > eggs") ;

		wstring normalized = mainframe.get_record_translation(record) ;

		SimpleString expected = "spam &lt; &amp; &gt; eggs" ;
		SimpleString actual = (LPCSTR)CStringA(normalized.c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	/************************************************************************/
	/* message tests                                                        */
	/************************************************************************/
	TEST(TestCMainFrame, AddMessageFilter)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		try
		{
			CMessageLoop theLoop;
			_Module.AddMessageLoop(&theLoop);
			register_message_filter(&mainframe) ;
			_Module.RemoveMessageLoop();
			ASSERT_TRUE(true) ;
		}
		catch (...)
		{
			FAIL_M("Exception thrown while mucking about with message filter") ;
		}

	}

	TEST(TestCMainFrame, toggle_markup)
	{
		app_props::properties_general props ;
		props.read_from_registry() ;

		BOOL old_markup = props.m_data.m_show_markup ;

		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		WindowsMessage dummy ;

		mainframe.on_user_toggle_markup(dummy) ;

		props.read_from_registry() ;
		ASSERT_EQUALS(old_markup, ! props.m_data.m_show_markup) ;

		mainframe.on_user_toggle_markup(dummy) ;

		props.read_from_registry() ;
		ASSERT_EQUALS(old_markup, props.m_data.m_show_markup) ;
	}

	TEST(TestCMainFrame, lookup_does_not_change_markup)
	{
		app_props::properties_general props ;
		props.read_from_registry() ;

		BOOL old_markup = props.m_data.m_show_markup ;

		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		WindowsMessage dummy ;

		wstring query = L"spam" ;

		mainframe.lookup(query) ;
		ASSERT_EQUALS(mainframe.m_trans_matches.m_params.m_show_marking,
			!! old_markup) ;

		mainframe.on_user_toggle_markup(dummy) ;
		props.read_from_registry() ;
		ASSERT_EQUALS(old_markup, ! props.m_data.m_show_markup) ;

		mainframe.lookup(query) ;
		ASSERT_EQUALS(mainframe.m_trans_matches.m_params.m_show_marking,
			! old_markup) ;

		mainframe.on_user_toggle_markup(dummy) ;

		props.read_from_registry() ;
		ASSERT_EQUALS(old_markup, props.m_data.m_show_markup) ;
	}

	// match lookup stuff
	TEST(TestCMainFrame, get_matches_size_0)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have zero matches: ") + 
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(0, matches.size(), err_msg) ;
	}
	TEST(TestCMainFrame, get_matches_size_1)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have exactly one match: ") + 
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(1, matches.size(), err_msg) ;
	}
	TEST(TestCMainFrame, get_matches_size_1_trans)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		search_match_ptr match = get_first_match(mainframe, params) ;
		CStringA actual = match->get_markup()->GetTrans().c_str() ;
		CStringA expected = "lovely plumage" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_size_1_trans_bold)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely <b>plumage</b>") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		search_match_ptr match = get_first_match(mainframe, params) ;
		CStringA actual = match->get_markup()->GetTrans().c_str() ;
		CStringA expected = "lovely <b>plumage</b>" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_size_2)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;
		add_record(mainframe, L"sausage beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have two matches: ") + 
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(2, matches.size(), err_msg) ;
	}

	/************************************************************************/
	/* get_matches with placement                                           */
	/************************************************************************/
	TEST(TestCMainFrame, get_matches_placement_size_1)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;
		params.m_place_numbers = true ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have exactly one match: ") + 
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(1, matches.size(), err_msg) ;
	}
	TEST(TestCMainFrame, get_matches_placement_size_2)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans and 2 eggs", L"beans and 2 eggs") ;

		search_query_params params ;
		params.m_rich_source = L"beans and 3 eggs" ;
		params.m_source = L"beans and 3 eggs" ;
		params.m_place_numbers = true ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have exactly two matches: ") +
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(2, matches.size(), err_msg) ;
	}

	TEST(TestCMainFrame, get_matches_placement_eating_numbers)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"5. bar", L"5. bar") ;

		search_query_params params ;
		params.m_rich_source = L"3.foo bar" ;
		params.m_source = L"3.foo bar" ;
		params.m_place_numbers = true ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have exactly two matches: ") +
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(2, matches.size(), err_msg) ;
		search_match_ptr match = *matches.begin() ;
		SimpleString actual = (LPCSTR)CStringA(match->get_record()->get_trans_plain().c_str()) ;
		SimpleString expected = "3. bar" ;
		ASSERT_EQUALS_V(expected, actual) ;
		double actualscore = match->get_score() ;
		double expectedscore = 6.0 / 9.0 ;
		ASSERT_EQUALS_DELTA_V(expectedscore, actualscore, 0.0001) ;
	}
	TEST(TestCMainFrame, get_matches_placement_eating_numbers_flipped_trans)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		TransMatchContainer matches ;
		mainframe.get_matches(matches, params) ;
		SimpleString err_msg = SimpleString("Should have exactly two matches: ") +
			StringFrom(static_cast< int >(matches.size())) ;
		ASSERT_EQUALS_M(2, matches.size(), err_msg) ;
		search_match_ptr match = *matches.begin() ;

		wstring actualtrans = match->get_record()->get_trans_plain() ;
		wstring expectedtrans = L"ÇRÅDIPä¥Ç∂" ;
		ASSERT_EQUALS(expectedtrans, actualtrans) ;
	}
	TEST(TestCMainFrame, get_matches_placement_eating_numbers_flipped_source)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		wstring actualsource = match->get_record()->get_source_plain() ;
		wstring expectedsource = L"ÇTÅDIPä¥Ç∂" ;
		ASSERT_EQUALS(expectedsource, actualsource) ;
	}
	TEST(TestCMainFrame, get_matches_placement_eating_numbers_flipped_marked_source)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		wstring actualsource = match->get_markup()->GetSource() ;
		wstring expectedsource = L"<span class=\"placement\">ÇRÅD</span><span class=\"nomatch\">IP</span>ä¥Ç∂" ;
		ASSERT_EQUALS(expectedsource, actualsource) ;
	}
	TEST(TestCMainFrame, get_matches_placement_eating_numbers_flipped_score)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		double actualscore = match->get_score() ;
		double expectedscore = 2.0 / 3.0 ;
		ASSERT_EQUALS_DELTA_V(expectedscore, actualscore, 0.0001) ;
	}

	/************************************************************************/
	/* get_matches with markup                                              */
	/************************************************************************/
	TEST(TestCMainFrame, get_matches_markup)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = "beans beans <span class=\"nomatch\">sp</span>am and egg" ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_markup_query)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = "beans beans <span class=\"nomatch\">h</span>am and egg" ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_markup_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = CStringA(match->get_markup()->GetSource().c_str()) ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_markup_query_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = CStringA(match->get_markup()->GetQuery().c_str()) ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_markup_word_algo)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = "beans beans <span class=\"nomatch\">spam</span> and egg" ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_markup_word_algo_query)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = "beans beans <span class=\"nomatch\">ham</span> and egg" ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, get_matches_markup_word_algo_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString first_time = CStringA(match->get_markup()->GetSource().c_str()) ;
		SimpleString second_time = CStringA(match->get_markup()->GetSource().c_str()) ;

		SimpleString err_msg = SimpleString("\nexpected: ") + first_time + 
			SimpleString("\nactual: ") + second_time ;
		ASSERT_EQUALS_M(first_time, second_time, err_msg) ;
	}
	TEST(TestCMainFrame, get_matches_markup_word_algo_query_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString first_time = CStringA(match->get_markup()->GetQuery().c_str()) ;
		SimpleString second_time = CStringA(match->get_markup()->GetQuery().c_str()) ;

		SimpleString err_msg = SimpleString("\nexpected: ") + first_time + 
			SimpleString("\nactual: ") + second_time ;
		ASSERT_EQUALS_M(first_time, second_time, err_msg) ;
	}

	TEST(TestCMainFrame, recalculate_source)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"aaddcc" ;
		params.m_source = L"aaddcc" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = CStringA(match->get_markup()->GetSource().c_str()) ;

		mainframe.recalculate_match(match, params) ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(TestCMainFrame, recalculate_query)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"aaddcc" ;
		params.m_source = L"aaddcc" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		SimpleString expected = CStringA(match->get_markup()->GetQuery().c_str()) ;

		mainframe.recalculate_match(match, params) ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST(TestCMainFrame, test_init_trans_matches_for_lookup_on)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		search_query_params dest ;
		app_props::properties source ;

		source.m_mem_props.m_data.m_ignore_case = TRUE ;
		source.m_mem_props.m_data.m_ignore_width = TRUE ;
		source.m_mem_props.m_data.m_ignore_hir_kat = TRUE ;
		source.m_mem_props.m_data.m_assess_format_penalty = TRUE ;

		source.m_alg_props.m_data.m_match_algo = IDC_ALGO_WORD ;

		source.m_mem_props.m_data.m_place_numbers = TRUE ;
		source.m_mem_props.m_data.m_place_gloss = TRUE ;

		mainframe.init_lookup_properties(source, dest) ;
		ASSERT_TRUE(dest.m_ignore_case) ;
		ASSERT_TRUE(dest.m_ignore_width) ;
		ASSERT_TRUE(dest.m_ignore_hira_kata) ;
		ASSERT_TRUE(dest.m_assess_format_penalty) ;

		ASSERT_EQUALS_V(dest.m_match_algo, IDC_ALGO_WORD) ;

		ASSERT_TRUE(dest.m_place_numbers) ;
		ASSERT_TRUE(dest.m_place_gloss) ;
	}

	TEST(TestCMainFrame, test_init_trans_matches_for_lookup_off)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		search_query_params dest ;
		app_props::properties source ;

		source.m_mem_props.m_data.m_ignore_case = FALSE ;
		source.m_mem_props.m_data.m_ignore_width = FALSE ;
		source.m_mem_props.m_data.m_ignore_hir_kat = FALSE ;
		source.m_mem_props.m_data.m_assess_format_penalty = FALSE ;

		source.m_alg_props.m_data.m_match_algo = IDC_ALGO_CHAR ;

		source.m_mem_props.m_data.m_place_numbers = FALSE ;
		source.m_mem_props.m_data.m_place_gloss = FALSE ;

		mainframe.init_lookup_properties(source, dest) ;

		ASSERT_TRUE(!dest.m_ignore_case) ;
		ASSERT_TRUE(!dest.m_ignore_width) ;
		ASSERT_TRUE(!dest.m_ignore_hira_kata) ;
		ASSERT_TRUE(!dest.m_assess_format_penalty) ;

		ASSERT_EQUALS_V(dest.m_match_algo, IDC_ALGO_CHAR) ;

		ASSERT_TRUE(!dest.m_place_numbers) ;
		ASSERT_TRUE(!dest.m_place_gloss) ;
	}

	TEST(TestCMainFrame, get_window_type_string)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		CStringA typestring = mainframe.get_window_type_string() ;

		ASSERT_EQUALS_V("Memory", typestring) ;
	}
	TEST(TestCMainFrame, put_show_marking)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		VARIANT_BOOL old_markup = mainframe.get_show_marking() ;

		mainframe.put_show_marking(VARIANT_TRUE) ;
		ASSERT_EQUALS_M(VARIANT_TRUE, mainframe.get_show_marking(), 
			"Show marking should be VARIANT_TRUE, but it's VARIANT_FALSE") ;

		mainframe.put_show_marking(VARIANT_FALSE) ;
		ASSERT_EQUALS_M(VARIANT_FALSE, mainframe.get_show_marking(), 
			"Show marking should be VARIANT_FALSE, but it's VARIANT_TRUE") ;

		mainframe.put_show_marking(old_markup) ;
	}
	TEST(TestCMainFrame, get_review_content)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		mainframe.m_review_record = record_pointer(new record_local) ;
		mainframe.m_review_record->set_source(L"foo") ;
		mainframe.m_review_record->set_trans(L"bar") ;

		memory_pointer mem(new memory_local) ;

		CStringA actual = mainframe.get_review_content(mem).c_str() ;

		string text ;
		text += "<table class=\"base\">\r\n" ;
		text += "	<tr>\r\n" ;
		text += "		<th class=\"header\" colspan=\"2\">\r\n" ;
		text += "			Review Translation\r\n" ;
		text += "		</th>\r\n" ;
		text += "	</tr>\r\n" ;
		text += "  <tr id=\"0\">\r\n" ;
		text += "    <th class=\"perfect\">Source</th>\r\n" ;
		text += "	<td class=\"match_content\" id=\"source\" style=\"color: #000000\">foo</td>\r\n" ;
		text += "  </tr>\r\n" ;
		text += "  <tr>\r\n" ;
		text += "    <th class=\"perfect\">Trans</td>\r\n" ;
		text += "	<td class=\"match_content\" id=\"trans\" style=\"color: #000000\">bar</td>\r\n" ;
		text += "  </tr>\r\n" ;
		text += "</table>\r\n" ;
		text += "\r\n" ;
		text += "<p>\r\n" ;
		text += "  <a href=\"142:0\" title=\"Edit entry\">Edit</a> | \r\n" ;
		text += "  <a href=\"122:0\" title=\"Delete entry\">Delete</a>\r\n" ;
		text += "</p>\r\n" ;
		text += "\r\n" ;
		text += "  <hr style=\"margin-top:3px;\" />\r\n" ;
		text += "\r\n" ;
		text += "<div style=\"float:left;\"> \r\n" ;
		text += "  <a href=\"127:0\" title=\"Register glossary entries\">Register Glossary</a> | \r\n" ;
		text += "  <a href=\"129:0\" title=\"Add entry to glossary\">Add to Glossary</a>\r\n" ;
		text += "</div>\r\n" ;
		text += "<div style=\"float:right;\"> \r\n" ;
		text += "	<A title=\"View detailed information about this entry\" href=\"152:159\">More &gt;&gt;</A>\r\n" ;
		text += "</div>\r\n" ;
		text += "\r\n" ;
		text += "<br clear=\"all\" />\r\n" ;
		text += "\r\n" ;
		text += "<table class=\"actions\">\r\n" ;
		text += "  <tr>\r\n" ;
		text += "    <td><b>Memory</b>: New</td>\r\n" ;
		text += "	<td><b>Reference Count</b>: 0</td>\r\n" ;
		text += "   </tr>\r\n" ;
		text += "</table>" ;

		string actual_text(actual) ;
		SimpleString expected = text.c_str() ;

		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestCMainFrame, clear_memory)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		add_record(mainframe, L"foo", L"bar") ;
		ASSERT_EQUALS_V(1, (int)mainframe.get_memory_model()->get_first_memory()->size()) ;
		mainframe.clear_memory() ;
		ASSERT_EQUALS_V(0, (int)mainframe.get_memory_model()->get_first_memory()->size()) ;
	}

	// concordance
	TEST(TestCMainFrame, create_concordance_list_0)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		SimpleString actual = string2string(mainframe.create_concordance_list()).c_str() ;
		SimpleString expected = "<b>Search Results:</b><br>Found 0 matches." ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
}

#endif 


