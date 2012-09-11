/*!
	@brief Unit tests for CMainFrame class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "MainFrm.h"
#include "reg_msg_filter.h"
#include "record_local.h"
#include "memory_local.h"
#include "felix_factory.h"
#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCMainFrame )

	using namespace mem_engine ;

	void add_record(frame_ptr frame, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		frame->add_record(rec) ;
	}

	search_match_ptr get_first_match(frame_ptr frame, search_query_params &params)
	{
		trans_match_container matches ;
		frame->get_matches(matches, params) ;

		trans_match_container::iterator pos = matches.begin() ;
		return *pos ;
	}

	/************************************************************************/
	/* tests start here                                                     */
	/************************************************************************/	

	// Tests for CMainFrame

	BOOST_AUTO_TEST_CASE( get_record_translation_standard)
	{
		frame_ptr frame = make_frame() ;
		frame->m_props->m_mem_props.m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"spam") ;

		wstring normalized = frame->get_record_translation(record) ;

		string expected = "spam" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(1u, record->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_plain)
	{
		frame_ptr frame = make_frame() ;
		frame->m_props->m_mem_props.m_data.m_plaintext = TRUE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<bold>spam &lt; &amp; &gt; eggs</bold>") ;

		BOOST_CHECK_EQUAL(record->get_trans_plain(), L"spam < & > eggs") ;

		wstring normalized = frame->get_record_translation(record) ;

		string expected = "spam &lt; &amp; &gt; eggs" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	/************************************************************************/
	/* message tests                                                        */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( AddMessageFilter)
	{
		frame_ptr frame = make_frame() ;

		try
		{
			CMessageLoop theLoop;
			_Module.AddMessageLoop(&theLoop);
			register_message_filter(frame.get()) ;
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

		frame_ptr frame = make_frame() ;
		BOOL old_markup = frame->m_props->m_gen_props.m_data.m_show_markup ;

		WindowsMessage dummy ;

		frame->on_user_toggle_markup(dummy) ;

		BOOST_CHECK(old_markup != frame->m_props->m_gen_props.m_data.m_show_markup) ;

		frame->on_user_toggle_markup(dummy) ;

		BOOST_CHECK(old_markup == frame->m_props->m_gen_props.m_data.m_show_markup) ;
	}

	BOOST_AUTO_TEST_CASE(test_check_shell_execute_result_ok)
	{

		frame_ptr frame = make_frame() ;
		CString path = L"c:\\foo" ;
		int result = 40 ;
		frame->check_shell_execute_result(result, path) ;
		BOOST_CHECK(true) ;
	}
	BOOST_AUTO_TEST_CASE(test_check_shell_execute_result_bad)
	{

		frame_ptr frame = make_frame() ;
		CString path = L"c:\\foo" ;
		int result = 10 ;
		try
		{
			frame->check_shell_execute_result(result, path) ;
		}
		catch (except::CWinException&)
		{
			BOOST_CHECK(true) ;
			return ;
		}
		BOOST_FAIL("Should have thrown with bad result") ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// match lookup
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( TestCMainFrameMatchLookup )

	using namespace mem_engine;

	void add_record(frame_ptr frame, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		frame->add_record(rec) ;
		frame->get_memory_model()->get_first_memory()->set_minimum_score(50) ;
	}

	search_match_ptr get_first_match(frame_ptr frame, search_query_params &params)
	{
		trans_match_container matches ;
		frame->get_matches(matches, params) ;

		trans_match_container::iterator pos = matches.begin() ;
		return *pos ;
	}

	void add_gloss(frame_ptr frame, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;

		frame->get_glossary_window()->add_record(rec) ;
	}

	/************************************************************************/
	/* lookup                                                               */
	/************************************************************************/

	BOOST_AUTO_TEST_CASE( lookup_does_not_change_markup)
	{
		frame_ptr frame = make_frame() ;
		BOOL old_markup = frame->m_props->m_gen_props.m_data.m_show_markup ;

		WindowsMessage dummy ;

		wstring query = L"spam" ;

		frame->lookup(query) ;
		BOOST_CHECK_EQUAL(frame->m_trans_matches.m_params.m_show_marking,
			!! old_markup) ;

		frame->on_user_toggle_markup(dummy) ;
		BOOST_CHECK(old_markup != frame->m_props->m_gen_props.m_data.m_show_markup) ;

		frame->lookup(query) ;
		BOOST_CHECK_EQUAL(frame->m_trans_matches.m_params.m_show_marking,
			! old_markup) ;

		frame->on_user_toggle_markup(dummy) ;

		BOOST_CHECK(old_markup == 	frame->m_props->m_gen_props.m_data.m_show_markup) ;
	}
	BOOST_AUTO_TEST_CASE(lookup_brackets)
	{
		frame_ptr frame = make_frame() ;
		add_record(frame, L"&lt;spam&gt;", L"foo") ;

		wstring query = L"&lt;spam&gt;" ;

		frame->lookup(query) ;
		BOOST_CHECK_EQUAL(1u, frame->m_trans_matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(lookup_trans_brackets)
	{
		frame_ptr frame = make_frame() ;
		add_record(frame, L"source", L"&lt;trans&gt;") ;

		wstring query = L"&lt;trans&gt;" ;

		frame->lookup_trans(query) ;
		BOOST_CHECK_EQUAL(1u, frame->m_trans_matches.size()) ;
	}
	// MON[0]Å`MON[6] of ScheduleNo1Å`7
	BOOST_AUTO_TEST_CASE(lookup_naishi)
	{
		frame_ptr frame = make_frame() ;
		add_record(frame, L"MON[0]Å`MON[6] of ScheduleNo1Å`7", L"MON[0]Å`MON[6] of ScheduleNo1Å`7") ;

		wstring query = L"MON[0]Å`MON[6] of ScheduleNo1Å`7" ;

		frame->lookup(query) ;
		BOOST_CHECK_EQUAL(1u, frame->m_trans_matches.size()) ;
	}

	// match lookup stuff
	BOOST_AUTO_TEST_CASE( get_matches_size_0)
	{
		frame_ptr frame = make_frame() ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;

		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_1)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_1_trans)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		search_match_ptr match = get_first_match(frame, params) ;
		CStringA actual = match->get_markup()->GetTrans().c_str() ;
		CStringA expected = "lovely plumage" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_1_trans_bold)
	{
		frame_ptr frame = make_frame() ;
		add_record(frame, L"beans beans spam and egg", L"lovely <b>plumage</b>") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		search_match_ptr match = get_first_match(frame, params) ;
		CStringA actual = match->get_markup()->GetTrans().c_str() ;
		CStringA expected = "lovely <b>plumage</b>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_size_2)
	{
		frame_ptr frame = make_frame() ;
		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;
		add_record(frame, L"sausage beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;

		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}

	/************************************************************************/
	/* get_matches with number placement                                    */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( get_matches_placement_size_1)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans spam and eggs" ;
		params.m_source = L"beans beans spam and eggs" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_size_2)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans and 2 eggs", L"beans and 2 eggs") ;

		search_query_params params ;
		params.m_rich_source = L"beans and 3 eggs" ;
		params.m_source = L"beans and 3 eggs" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"5. bar", L"5. bar") ;

		search_query_params params ;
		params.m_rich_source = L"3.foo bar" ;
		params.m_source = L"3.foo bar" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;
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
		frame_ptr frame = make_frame() ;

		add_record(frame, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;

		search_match_ptr match = *matches.begin() ;

		wstring actualtrans = match->get_record()->get_trans_plain() ;
		wstring expectedtrans = L"ÇRÅDIPä¥Ç∂" ;
		BOOST_CHECK_EQUAL(expectedtrans, actualtrans) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_source)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		search_match_ptr match = get_first_match(frame, params) ;

		wstring actualsource = match->get_record()->get_source_plain() ;
		wstring expectedsource = L"ÇTÅDIPä¥Ç∂" ;
		BOOST_CHECK_EQUAL(expectedsource, actualsource) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_marked_source)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		search_match_ptr match = get_first_match(frame, params) ;

		wstring actualsource = match->get_markup()->GetSource() ;
		wstring expectedsource = L"<span class=\"placement\">ÇRÅD</span><span class=\"nomatch\">IP</span>ä¥Ç∂" ;
		BOOST_CHECK_EQUAL(expectedsource, actualsource) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_placement_eating_numbers_flipped_score)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"ÇTÅDIPä¥Ç∂", L"ÇTÅDIPä¥Ç∂") ;

		search_query_params params ;
		params.m_rich_source = L"ÇRÅDä¥Ç∂" ;
		params.m_source = L"ÇRÅDä¥Ç∂" ;
		params.m_place_numbers = true ;

		search_match_ptr match = get_first_match(frame, params) ;

		double actualscore = match->get_score() ;
		double expectedscore = 2.0 / 3.0 ;
		BOOST_CHECK_CLOSE(expectedscore, actualscore, 0.01) ;
	}

	/************************************************************************/
	/* get_matches with gloss placement                                     */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( get_matches_gloss_placement_size_2)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"bbb aaa bbb", L"zzz xxx zzz") ;
		add_gloss(frame, L"aaa", L"xxx") ;
		add_gloss(frame, L"ccc", L"yyy") ;

		search_query_params params ;
		params.m_rich_source = L"bbb ccc bbb" ;
		params.m_source = L"bbb ccc bbb" ;
		params.m_place_numbers = false ;
		params.m_place_gloss = true ;
		frame->get_properties()->m_mem_props.m_data.m_place_gloss = TRUE ;

		trans_match_container matches ;
		frame->get_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}


	/************************************************************************/
	/* get_matches with markup                                              */
	/************************************************************************/
	BOOST_AUTO_TEST_CASE( get_matches_markup)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = "beans beans <span class=\"nomatch\">sp</span>am and egg" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_query)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = "beans beans <span class=\"nomatch\">h</span>am and egg" ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_twice)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = CStringA(match->get_markup()->GetSource().c_str()) ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_query_twice)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = CStringA(match->get_markup()->GetQuery().c_str()) ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = "beans beans <span class=\"nomatch\">spam</span> and egg" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo_query)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = "beans beans <span class=\"nomatch\">ham</span> and egg" ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo_twice)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(frame, params) ;

		string first_time = CStringA(match->get_markup()->GetSource().c_str()) ;
		string second_time = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(first_time, second_time) ;
	}
	BOOST_AUTO_TEST_CASE( get_matches_markup_word_algo_query_twice)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"beans beans spam and egg", L"lovely plumage") ;

		search_query_params params ;
		params.m_rich_source = L"beans beans ham and egg" ;
		params.m_source = L"beans beans ham and egg" ;
		params.m_match_algo = IDC_ALGO_WORD ;

		search_match_ptr match = get_first_match(frame, params) ;

		string first_time = CStringA(match->get_markup()->GetQuery().c_str()) ;
		string second_time = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(first_time, second_time) ;
	}

	// recalculate_match
	BOOST_AUTO_TEST_CASE( recalculate_source)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"aaddcc" ;
		params.m_source = L"aaddcc" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = "aa<span class=\"nomatch\">bb</span>cc" ;

		frame->recalculate_match(match, params) ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_query)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"aaddcc" ;
		params.m_source = L"aaddcc" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		search_match_ptr match = get_first_match(frame, params) ;

		string expected = "aa<span class=\"nomatch\">dd</span>cc" ;

		frame->recalculate_match(match, params) ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_source_trans_lookup)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"11aa33" ;
		params.m_source = L"11aa33" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		frame->lookup_trans(params.m_rich_source) ;
		search_match_ptr match = frame->m_trans_matches.at(0) ;
		frame->m_model->set_reverse_lookup(true) ;
		frame->recalculate_match(match, params) ;

		string expected = "aabbcc" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_trans_trans_lookup)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"11aa33" ;
		params.m_source = L"11aa33" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		frame->lookup_trans(params.m_rich_source) ;
		search_match_ptr match = frame->m_trans_matches.at(0) ;
		frame->m_model->set_reverse_lookup(true) ;
		frame->recalculate_match(match, params) ;

		string expected = "11<span class=\"nomatch\">22</span>33" ;
		string actual = CStringA(match->get_markup()->GetTrans().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( recalculate_query_trans_lookup)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"aabbcc", L"112233") ;

		search_query_params params ;
		params.m_rich_source = L"11aa33" ;
		params.m_source = L"11aa33" ;
		params.m_match_algo = IDC_ALGO_CHAR ;

		frame->lookup_trans(params.m_rich_source) ;
		search_match_ptr match = frame->m_trans_matches.at(0) ;
		frame->m_model->set_reverse_lookup(true) ;
		frame->recalculate_match(match, params) ;

		wstring expected = L"11<span class=\"nomatch\">aa</span>33" ;
		wstring actual = match->get_markup()->GetQuery() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// correct translations
//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( TestConnect )

	using namespace mem_engine;

	// review match
	BOOST_AUTO_TEST_CASE(add_null_mem)
	{
		frame_ptr frame = make_frame() ;

		LRESULT res = frame->add_remote_memory(frame->m_model->get_memories(),
											   memory_pointer()) ;

		BOOST_CHECK_EQUAL(-1L, res) ;
		BOOST_CHECK_EQUAL(0u, frame->m_model->size()) ;
	}

	// review match
	BOOST_AUTO_TEST_CASE(add_ok_mem)
	{
		frame_ptr frame = make_frame() ;

		LRESULT res = frame->add_remote_memory(frame->m_model->get_memories(),
												memory_pointer(new memory_local(frame->m_props))) ;

		BOOST_CHECK_EQUAL(0L, res) ;
		BOOST_CHECK_EQUAL(1u, frame->m_model->size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()


//////////////////////////////////////////////////////////////////////////
// correct translations
//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( TestCorrectTrans )

	using namespace mem_engine;

	// review match
	BOOST_AUTO_TEST_CASE(from_top_memory)
	{
		frame_ptr frame = make_frame() ;

		memory_pointer mem2 = frame->m_model->add_memory() ;
		memory_pointer mem1 = frame->m_model->add_memory() ;

		record_pointer record(new record_local) ;
		search_match_ptr match = search_match_ptr(new search_match(record)) ;
		record->set_source(L"foo") ;
		record->set_trans(L"bar") ;
		mem1->add_record(record) ;
		match->set_memory_id(mem1->get_id()) ;
		frame->m_review_match = match ;
		frame->m_display_state = WindowListener::TRANS_REVIEW_STATE ;

		frame->correct_trans(L"corrected") ;

		wstring expected_trans = L"corrected" ;

		BOOST_CHECK_EQUAL(expected_trans, record->get_trans_rich()) ;
		BOOST_CHECK_EQUAL(0u, mem2->size()) ;
		BOOST_CHECK_EQUAL(1u, mem1->size()) ;

	}

	// review match
	BOOST_AUTO_TEST_CASE(from_bottom_memory)
	{
		frame_ptr frame = make_frame() ;

		memory_pointer mem2 = frame->m_model->add_memory() ;
		memory_pointer mem1 = frame->m_model->add_memory() ;

		record_pointer record(new record_local) ;
		search_match_ptr match = search_match_ptr(new search_match(record)) ;
		record->set_source(L"foo") ;
		record->set_trans(L"bar") ;
		mem2->add_record(record) ;
		match->set_memory_id(mem2->get_id()) ;
		frame->m_review_match = match ;
		frame->m_display_state = WindowListener::TRANS_REVIEW_STATE ;

		frame->correct_trans(L"corrected") ;

		wstring expected_trans = L"corrected" ;

		BOOST_CHECK_EQUAL(expected_trans, record->get_trans_rich()) ;
		BOOST_CHECK_EQUAL(0u, mem1->size()) ;
		BOOST_CHECK_EQUAL(1u, mem2->size()) ;

	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// concordance
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( TestCMainFrameConcordance )

	using namespace mem_engine;

	void add_record(frame_ptr frame, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		frame->add_record(rec) ;
	}

	BOOST_AUTO_TEST_CASE(get_concordance_size_1)
	{
		frame_ptr frame = make_frame() ;

		add_record(frame, L"aaa BBB ccc", L"get_concordance_size_1") ;

		frame->get_concordances(L"BBB") ;

		BOOST_CHECK_EQUAL(1u, frame->m_search_matches.size()) ;
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
		frame_ptr frame = make_frame() ;

		search_query_params dest ;
		app_props::props_ptr source(new app_props::properties) ;

		source->m_mem_props.m_data.m_ignore_case = TRUE ;
		source->m_mem_props.m_data.m_ignore_width = TRUE ;
		source->m_mem_props.m_data.m_ignore_hir_kat = TRUE ;
		source->m_mem_props.m_data.m_assess_format_penalty = TRUE ;

		source->m_alg_props.m_data.m_match_algo = IDC_ALGO_WORD ;

		source->m_mem_props.m_data.m_place_numbers = TRUE ;
		source->m_mem_props.m_data.m_place_gloss = TRUE ;

		frame->init_lookup_properties(source, dest) ;
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
		frame_ptr frame = make_frame() ;

		search_query_params dest ;
		app_props::props_ptr source(new app_props::properties) ;

		source->m_mem_props.m_data.m_ignore_case = FALSE ;
		source->m_mem_props.m_data.m_ignore_width = FALSE ;
		source->m_mem_props.m_data.m_ignore_hir_kat = FALSE ;
		source->m_mem_props.m_data.m_assess_format_penalty = FALSE ;

		source->m_alg_props.m_data.m_match_algo = IDC_ALGO_CHAR ;

		source->m_mem_props.m_data.m_place_numbers = FALSE ;
		source->m_mem_props.m_data.m_place_gloss = FALSE ;

		frame->init_lookup_properties(source, dest) ;

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

	void add_record(frame_ptr frame, LPCWSTR source, LPCWSTR trans)
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(wstring(source)) ;
		rec->set_trans(wstring(trans)) ;
		frame->add_record(rec) ;
	}

	BOOST_AUTO_TEST_CASE( get_window_type_string)
	{
		frame_ptr frame = make_frame() ;

		CStringA typestring = frame->get_window_type_string() ;

		BOOST_CHECK_EQUAL("Memory", typestring) ;
	}
	BOOST_AUTO_TEST_CASE( put_show_marking)
	{
		frame_ptr frame = make_frame() ;

		VARIANT_BOOL old_markup = frame->get_show_marking() ;

		frame->put_show_marking(VARIANT_TRUE) ;
		BOOST_CHECK(VARIANT_TRUE == frame->get_show_marking()) ;

		frame->put_show_marking(VARIANT_FALSE) ;
		BOOST_CHECK(VARIANT_FALSE == frame->get_show_marking()) ;

		frame->put_show_marking(old_markup) ;
	}
	BOOST_AUTO_TEST_CASE( get_review_content)
	{
		frame_ptr frame = make_frame() ;

		record_pointer record(new record_local) ;
		frame->m_review_match = search_match_ptr(new search_match(record)) ;
		record->set_source(L"foo") ;
		record->set_trans(L"bar") ;

		memory_pointer mem = frame->m_model->add_memory() ;

		CStringA actual = frame->get_review_content(mem).c_str() ;

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
		frame_ptr frame = make_frame() ;
		add_record(frame, L"foo", L"bar") ;
		BOOST_CHECK_EQUAL(1u, frame->get_memory_model()->get_first_memory()->size()) ;
		frame->clear_memory() ;
		BOOST_CHECK_EQUAL(0u, frame->get_memory_model()->get_first_memory()->size()) ;
	}

	// get_active_mem_name
	BOOST_AUTO_TEST_CASE( get_active_mem_name_new)
	{
		frame_ptr frame = make_frame() ;
		string actual = (LPCSTR)CStringA(frame->get_active_mem_name()) ;
		string expected = "New" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_active_mem_name_spam)
	{
		frame_ptr frame = make_frame() ;
		add_record(frame, L"aabbcc", L"112233") ;

		frame->m_model->get_memories()->get_first_memory()->set_location(_T("C:\\test\\spam.ftm"));

		string actual = (LPCSTR)CStringA(frame->get_active_mem_name()) ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	// match_count_feedback
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_1 )
	{
		frame_ptr frame = make_frame() ;
		frame->match_count_feedback(1) ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found 1 match." ) ;
	}
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_0 )
	{
		frame_ptr frame = make_frame() ;
		frame->match_count_feedback(0) ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found 0 matches." ) ;
	}
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_10 )
	{
		frame_ptr frame = make_frame() ;
		frame->match_count_feedback(10) ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found 10 matches." ) ;
	}
	BOOST_AUTO_TEST_CASE( test_msg_match_count_feedback_1001 )
	{
		frame_ptr frame = make_frame() ;
		frame->match_count_feedback(1001) ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found 1,001 matches." ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

/************************************************************************/
/* get_reg_gloss_record                                                 */
/************************************************************************/

BOOST_AUTO_TEST_SUITE( TestCMainFrame_get_reg_gloss_record )

	using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( concordance_state_0 )
	{
		frame_ptr frame = make_frame() ;

		search_match_ptr match = frame->get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"concordance_state_0") ;
		frame->m_search_matches.add_match(match) ;

		frame->set_display_state(WindowListener::CONCORDANCE_DISPLAY_STATE) ;

		record_pointer rec = frame->get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), L"concordance_state_0" ) ;
	}

	BOOST_AUTO_TEST_CASE( initial_state )
	{
		frame_ptr frame = make_frame() ;

		search_match_ptr match = frame->get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"concordance_state_0") ;
		frame->m_search_matches.add_match(match) ;

		frame->set_display_state(WindowListener::INIT_DISPLAY_STATE) ;

		record_pointer rec = frame->get_reg_gloss_record(0u) ;
		BOOST_CHECK(rec->get_source_plain().empty()) ;
	}

	BOOST_AUTO_TEST_CASE( match_state_0 )
	{
		frame_ptr frame = make_frame() ;

		search_match_ptr match = frame->get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"concordance_state_0") ;
		frame->m_search_matches.add_match(match) ;

		match = frame->get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"match_state_0") ;
		frame->m_trans_matches.add_match(match) ;

		frame->set_display_state(WindowListener::MATCH_DISPLAY_STATE) ;

		record_pointer rec = frame->get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), L"match_state_0" ) ;
	}

	BOOST_AUTO_TEST_CASE( new_state )
	{
		frame_ptr frame = make_frame() ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"new_state") ;

		frame->set_new_record(newrec) ;

		search_match_ptr match = frame->get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"match_state_0") ;
		frame->m_trans_matches.add_match(match) ;

		frame->set_display_state(WindowListener::NEW_RECORD_DISPLAY_STATE) ;

		record_pointer rec = frame->get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), L"new_state" ) ;
	}


	BOOST_AUTO_TEST_CASE( review_state )
	{
		frame_ptr frame = make_frame() ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"review_state") ;

		frame->set_review_match(newrec, 0) ;

		search_match_ptr match = frame->get_model()->get_first_memory()->make_match() ;
		match->get_record()->set_source(L"match_state_0") ;
		frame->m_trans_matches.add_match(match) ;

		frame->set_display_state(WindowListener::TRANS_REVIEW_STATE) ;

		record_pointer rec = frame->get_reg_gloss_record(0u) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), wstring(L"review_state")) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif 


