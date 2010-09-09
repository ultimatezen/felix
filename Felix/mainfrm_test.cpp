/*!
	@brief Unit tests for CMainFrame class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "MainFrm.h"
#include "reg_msg_filter.h"
#include "record_local.h"
#include "memory_local.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCMainFrame )

	using namespace mem_engine ;

	void add_record(CMainFrame &mainframe, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mainframe.add_record(rec) ;
	}

	search_match_ptr get_first_match(CMainFrame &mainframe, search_query_params &params)
	{
		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;

		trans_match_container::iterator pos = matches.begin() ;
		return *pos ;
	}

	/************************************************************************/
	/* tests start here                                                     */
	/************************************************************************/	

	// Tests for CMainFrame

	BOOST_AUTO_TEST_CASE( get_record_translation_standard)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		mainframe.m_properties->m_mem_props.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"spam") ;

		wstring normalized = mainframe.get_record_translation(record) ;

		string expected = "spam" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(1u, record->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_plain)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		mainframe.m_properties->m_mem_props.m_data.m_plaintext = TRUE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<bold>spam &lt; &amp; &gt; eggs</bold>") ;

		BOOST_CHECK_EQUAL(record->get_trans_plain(), L"spam < & > eggs") ;

		wstring normalized = mainframe.get_record_translation(record) ;

		string expected = "spam &lt; &amp; &gt; eggs" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	/************************************************************************/
	/* message tests                                                        */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( AddMessageFilter)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		try
		{
			CMessageLoop theLoop;
			_Module.AddMessageLoop(&theLoop);
			register_message_filter(&mainframe) ;
			_Module.RemoveMessageLoop();
			BOOST_CHECK(true) ;
		}
		catch (...)
		{
			BOOST_FAIL("Exception thrown while mucking about with message filter") ;
		}

	}

	BOOST_AUTO_TEST_CASE( toggle_markup)
	{
		app_props::properties_general props ;
		props.read_from_registry() ;

		BOOL old_markup = props.m_data.m_show_markup ;

		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		WindowsMessage dummy ;

		mainframe.on_user_toggle_markup(dummy) ;

		props.read_from_registry() ;
		BOOST_CHECK(old_markup != props.m_data.m_show_markup) ;

		mainframe.on_user_toggle_markup(dummy) ;

		props.read_from_registry() ;
		BOOST_CHECK(old_markup == props.m_data.m_show_markup) ;
	}


BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// match lookup
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( TestCMainFrameMatchLookup )

	using namespace mem_engine;

	void add_record(CMainFrame &mainframe, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mainframe.add_record(rec) ;
	}

	search_match_ptr get_first_match(CMainFrame &mainframe, search_query_params &params)
	{
		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;

		trans_match_container::iterator pos = matches.begin() ;
		return *pos ;
	}

	/************************************************************************/
	/* lookup                                                               */
	/************************************************************************/

	BOOST_AUTO_TEST_CASE( lookup_does_not_change_markup)
	{
		app_props::properties_general props ;
		props.read_from_registry() ;

		BOOL old_markup = props.m_data.m_show_markup ;

		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		WindowsMessage dummy ;

		wstring query = L"spam" ;

		mainframe.lookup(query) ;
		BOOST_CHECK_EQUAL(mainframe.m_trans_matches.m_params.m_show_marking,
			!! old_markup) ;

		mainframe.on_user_toggle_markup(dummy) ;
		props.read_from_registry() ;
		BOOST_CHECK(old_markup != props.m_data.m_show_markup) ;

		mainframe.lookup(query) ;
		BOOST_CHECK_EQUAL(mainframe.m_trans_matches.m_params.m_show_marking,
			! old_markup) ;

		mainframe.on_user_toggle_markup(dummy) ;

		props.read_from_registry() ;
		BOOST_CHECK(old_markup == props.m_data.m_show_markup) ;
	}
	BOOST_AUTO_TEST_CASE(lookup_brackets)
	{
		app_props::properties_general props ;
		props.read_from_registry() ;

		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		add_record(mainframe, L"&lt;spam&gt;", L"foo") ;

		wstring query = L"&lt;spam&gt;" ;

		mainframe.lookup(query) ;
		BOOST_CHECK_EQUAL(1u, mainframe.m_trans_matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(lookup_trans_brackets)
	{
		app_props::properties_general props ;
		props.read_from_registry() ;

		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		add_record(mainframe, L"source", L"&lt;trans&gt;") ;

		wstring query = L"&lt;trans&gt;" ;

		mainframe.lookup_trans(query) ;
		BOOST_CHECK_EQUAL(1u, mainframe.m_trans_matches.size()) ;
	}
	// match lookup stuff
	BOOST_AUTO_TEST_CASE( get_matches_size_0)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;

		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_1)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_1_trans)
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
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_1_trans_bold)
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
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_2)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;
		add_record(mainframe, L"sausage beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;

		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}

	/************************************************************************/
	/* get_matches with placement                                           */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( get_matches_placement_size_1)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_size_2)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans and 2 eggs", L"beans and 2 eggs") ;

		search_query_params params ;
		params.m_rich_source = L"beans and 3 eggs" ;
		params.m_source = L"beans and 3 eggs" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"5. bar", L"5. bar") ;

		search_query_params params ;
		params.m_rich_source = L"3.foo bar" ;
		params.m_source = L"3.foo bar" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;

		search_match_ptr match = *matches.begin() ;
		string actual = (LPCSTR)CStringA(match->get_record()->get_trans_plain().c_str()) ;
		string expected = "3. bar" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		double actualscore = match->get_score() ;
		double expectedscore = 6.0 / 9.0 ;
		BOOST_CHECK_CLOSE(expectedscore, actualscore, 0.01) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_trans)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		mainframe.get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;

		search_match_ptr match = *matches.begin() ;

		wstring actualtrans = match->get_record()->get_trans_plain() ;
		wstring expectedtrans = L"ÇRÅDIPä¥Ç∂" ;
		BOOST_CHECK_EQUAL(expectedtrans, actualtrans) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_source)
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
		BOOST_CHECK_EQUAL(expectedsource, actualsource) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_marked_source)
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
		BOOST_CHECK_EQUAL(expectedsource, actualsource) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_score)
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
		BOOST_CHECK_CLOSE(expectedscore, actualscore, 0.01) ;
	}

	/************************************************************************/
	/* get_matches with markup                                              */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( get_matches_markup)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = "beans beans <span class=\"nomatch\">sp</span>am and egg" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_query)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = "beans beans <span class=\"nomatch\">h</span>am and egg" ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = CStringA(match->get_markup()->GetSource().c_str()) ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_query_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = CStringA(match->get_markup()->GetQuery().c_str()) ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = "beans beans <span class=\"nomatch\">spam</span> and egg" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo_query)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = "beans beans <span class=\"nomatch\">ham</span> and egg" ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string first_time = CStringA(match->get_markup()->GetSource().c_str()) ;
		string second_time = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(first_time, second_time) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo_query_twice)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string first_time = CStringA(match->get_markup()->GetQuery().c_str()) ;
		string second_time = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(first_time, second_time) ;
	}

	// recalculate_match
	BOOST_AUTO_TEST_CASE( recalculate_source)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"aaddcc" ;
		params.m_source = L"aaddcc" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = "aa<span class=\"nomatch\">bb</span>cc" ;

		mainframe.recalculate_match(match, params) ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_query)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"aaddcc" ;
		params.m_source = L"aaddcc" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		search_match_ptr match = get_first_match(mainframe, params) ;

		string expected = "aa<span class=\"nomatch\">dd</span>cc" ;

		mainframe.recalculate_match(match, params) ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_source_trans_lookup)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"11aa33" ;
		params.m_source = L"11aa33" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		mainframe.lookup_trans(params.m_rich_source) ;
		search_match_ptr match = mainframe.m_trans_matches.at(0) ;
		model.m_is_reverse_lookup = true ;
		mainframe.recalculate_match(match, params) ;

		string expected = "aabbcc" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_trans_trans_lookup)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"11aa33" ;
		params.m_source = L"11aa33" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		mainframe.lookup_trans(params.m_rich_source) ;
		search_match_ptr match = mainframe.m_trans_matches.at(0) ;
		model.m_is_reverse_lookup = true ;
		mainframe.recalculate_match(match, params) ;

		string expected = "11<span class=\"nomatch\">22</span>33" ;
		string actual = CStringA(match->get_markup()->GetTrans().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_query_trans_lookup)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"11aa33" ;
		params.m_source = L"11aa33" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		mainframe.lookup_trans(params.m_rich_source) ;
		search_match_ptr match = mainframe.m_trans_matches.at(0) ;
		model.m_is_reverse_lookup = true ;
		mainframe.recalculate_match(match, params) ;

		string expected = "11<span class=\"nomatch\">aa</span>33" ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// concordance
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( TestCMainFrameConcordance )

	using namespace mem_engine;

	void add_record(CMainFrame &mainframe, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mainframe.add_record(rec) ;
	}

	BOOST_AUTO_TEST_CASE(get_concordance_size_1)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		add_record(mainframe, L"aaa BBB ccc", L"get_concordance_size_1") ;

		mainframe.get_concordances(L"BBB") ;

		BOOST_CHECK_EQUAL(1u, mainframe.m_search_matches.size()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// search init
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( TestCMainFrameInitSearchStuff )

	using namespace mem_engine ;
	// init_lookup_properties
	BOOST_AUTO_TEST_CASE( test_init_trans_matches_for_lookup_on)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		search_query_params dest ;
		app_props::props_ptr source(new app_props::properties) ;

		source->m_mem_props.m_data.m_ignore_case = TRUE ;
		source->m_mem_props.m_data.m_ignore_width = TRUE ;
		source->m_mem_props.m_data.m_ignore_hir_kat = TRUE ;
		source->m_mem_props.m_data.m_assess_format_penalty = TRUE ;

		source->m_alg_props.m_data.m_match_algo = IDC_ALGO_WORD ;

		source->m_mem_props.m_data.m_place_numbers = TRUE ;
		source->m_mem_props.m_data.m_place_gloss = TRUE ;

		mainframe.init_lookup_properties(source, dest) ;
		BOOST_CHECK(dest.m_ignore_case) ;
		BOOST_CHECK(dest.m_ignore_width) ;
		BOOST_CHECK(dest.m_ignore_hira_kata) ;
		BOOST_CHECK(dest.m_assess_format_penalty) ;

		BOOST_CHECK_EQUAL(dest.m_match_algo, IDC_ALGO_WORD) ;

		BOOST_CHECK(dest.m_place_numbers) ;
		BOOST_CHECK(dest.m_place_gloss) ;
	}

	BOOST_AUTO_TEST_CASE( test_init_trans_matches_for_lookup_off)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		search_query_params dest ;
		app_props::props_ptr source(new app_props::properties) ;

		source->m_mem_props.m_data.m_ignore_case = FALSE ;
		source->m_mem_props.m_data.m_ignore_width = FALSE ;
		source->m_mem_props.m_data.m_ignore_hir_kat = FALSE ;
		source->m_mem_props.m_data.m_assess_format_penalty = FALSE ;

		source->m_alg_props.m_data.m_match_algo = IDC_ALGO_CHAR ;

		source->m_mem_props.m_data.m_place_numbers = FALSE ;
		source->m_mem_props.m_data.m_place_gloss = FALSE ;

		mainframe.init_lookup_properties(source, dest) ;

		BOOST_CHECK(!dest.m_ignore_case) ;
		BOOST_CHECK(!dest.m_ignore_width) ;
		BOOST_CHECK(!dest.m_ignore_hira_kata) ;
		BOOST_CHECK(!dest.m_assess_format_penalty) ;

		BOOST_CHECK_EQUAL(dest.m_match_algo, IDC_ALGO_CHAR) ;

		BOOST_CHECK(!dest.m_place_numbers) ;
		BOOST_CHECK(!dest.m_place_gloss) ;
	}


BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// settings
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( TestCMainFrameSettings )

	using namespace mem_engine ;

	void add_record(CMainFrame &mainframe, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		mainframe.add_record(rec) ;
	}

	BOOST_AUTO_TEST_CASE( get_window_type_string)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		CStringA typestring = mainframe.get_window_type_string() ;

		BOOST_CHECK_EQUAL("Memory", typestring) ;
	}
	BOOST_AUTO_TEST_CASE( put_show_marking)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;

		VARIANT_BOOL old_markup = mainframe.get_show_marking() ;

		mainframe.put_show_marking(VARIANT_TRUE) ;
		BOOST_CHECK(VARIANT_TRUE == mainframe.get_show_marking()) ;

		mainframe.put_show_marking(VARIANT_FALSE) ;
		BOOST_CHECK(VARIANT_FALSE == mainframe.get_show_marking()) ;

		mainframe.put_show_marking(old_markup) ;
	}
	BOOST_AUTO_TEST_CASE( get_review_content)
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
		string expected = text ;

		BOOST_CHECK_EQUAL(expected, actual_text) ;
	}
	BOOST_AUTO_TEST_CASE( clear_memory)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		add_record(mainframe, L"foo", L"bar") ;
		BOOST_CHECK_EQUAL(1u, mainframe.get_memory_model()->get_first_memory()->size()) ;
		mainframe.clear_memory() ;
		BOOST_CHECK_EQUAL(0u, mainframe.get_memory_model()->get_first_memory()->size()) ;
	}

	// get_active_mem_name
	BOOST_AUTO_TEST_CASE( get_active_mem_name_new)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		string actual = (LPCSTR)CStringA(mainframe.get_active_mem_name()) ;
		string expected = "New" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_active_mem_name_spam)
	{
		MainFrameModel model ;
		CMainFrame mainframe(&model) ;
		add_record(mainframe, L"aabbcc", L"112233") ;

		mainframe.m_model->get_memories()->get_first_memory()->set_location(_T("C:\\test\\spam.ftm"));

		string actual = (LPCSTR)CStringA(mainframe.get_active_mem_name()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	// match_count_feedback
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_1 )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		main_frame.match_count_feedback(1) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found 1 match." ) ;
	}
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_0 )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		main_frame.match_count_feedback(0) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found 0 matches." ) ;
	}
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_10 )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		main_frame.match_count_feedback(10) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found 10 matches." ) ;
	}
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_1001 )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		main_frame.match_count_feedback(1001) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found 1,001 matches." ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

/************************************************************************/
/* get_reg_gloss_record                                                 */
/************************************************************************/

BOOST_AUTO_TEST_SUITE( TestCMainFrame_get_reg_gloss_record )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( concordance_state_0 )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		search_match_ptr match = main_frame.get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"concordance_state_0") ;
		main_frame.m_search_matches.add_match(match) ;

		main_frame.set_display_state(WindowListener::CONCORDANCE_DISPLAY_STATE) ;

		record_pointer rec = main_frame.get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), L"concordance_state_0" ) ;
	}

	BOOST_AUTO_TEST_CASE( initial_state )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		search_match_ptr match = main_frame.get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"concordance_state_0") ;
		main_frame.m_search_matches.add_match(match) ;

		main_frame.set_display_state(WindowListener::INIT_DISPLAY_STATE) ;

		record_pointer rec = main_frame.get_reg_gloss_record(0u) ;
		BOOST_CHECK(rec->get_source_plain().empty()) ;
	}

	BOOST_AUTO_TEST_CASE( match_state_0 )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		search_match_ptr match = main_frame.get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"concordance_state_0") ;
		main_frame.m_search_matches.add_match(match) ;

		match = main_frame.get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"match_state_0") ;
		main_frame.m_trans_matches.add_match(match) ;

		main_frame.set_display_state(WindowListener::MATCH_DISPLAY_STATE) ;

		record_pointer rec = main_frame.get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), L"match_state_0" ) ;
	}

	BOOST_AUTO_TEST_CASE( new_state )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"new_state") ;

		main_frame.set_new_record(newrec) ;

		search_match_ptr match = main_frame.get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"match_state_0") ;
		main_frame.m_trans_matches.add_match(match) ;

		main_frame.set_display_state(WindowListener::NEW_RECORD_DISPLAY_STATE) ;

		record_pointer rec = main_frame.get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), L"new_state" ) ;
	}


	BOOST_AUTO_TEST_CASE( review_state )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"review_state") ;

		main_frame.set_review_record(newrec) ;

		search_match_ptr match = main_frame.get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"match_state_0") ;
		main_frame.m_trans_matches.add_match(match) ;

		main_frame.set_display_state(WindowListener::TRANS_REVIEW_STATE) ;

		record_pointer rec = main_frame.get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), wstring(L"review_state")) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif 


