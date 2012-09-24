#include "StdAfx.h"
#include "GlossaryDialog.h"
#include "record_local.h"
#include "memory_local.h"
#include "felix_factory.h"
#include "input_device_fake.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestGlossaryWindow )

	using namespace mem_engine;

	//////////////////////////////////////////////////////////////////////////
	// initial setup
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE(init_view_state_search_matches)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		BOOST_CHECK(gloss_placer.m_view_state_concordance.m_search_matches) ;
		BOOST_CHECK(gloss_placer.m_view_state_match.m_search_matches) ;
	}
	BOOST_AUTO_TEST_CASE(init_view_state_gloss_properties)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		BOOST_CHECK(gloss_placer.m_view_state_concordance.m_properties_gloss) ;
		BOOST_CHECK(gloss_placer.m_view_state_match.m_properties_gloss) ;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// config_matches_for_gloss_lookup
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE(config_matches_for_gloss_lookup)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.m_search_matches.m_params.m_ignore_case = false ;
		gloss_placer.m_search_matches.m_params.m_ignore_width = false ;
		gloss_placer.m_search_matches.m_params.m_ignore_hira_kata = false ;

		gloss_placer.m_properties_gloss->m_data.m_ignore_case = TRUE ;
		gloss_placer.m_properties_gloss->m_data.m_ignore_width = TRUE ;
		gloss_placer.m_properties_gloss->m_data.m_ignore_hir_kat = TRUE ;

		wstring query = L"gloss query" ;

		gloss_placer.config_matches_for_gloss_lookup(query) ;

		BOOST_CHECK(gloss_placer.m_search_matches.m_params.m_ignore_case) ;
		BOOST_CHECK(gloss_placer.m_search_matches.m_params.m_ignore_width) ;
		BOOST_CHECK(gloss_placer.m_search_matches.m_params.m_ignore_hira_kata) ;

		BOOST_CHECK_EQUAL(L"gloss query", gloss_placer.m_search_matches.get_query_rich()) ;
	}


	// on_toggle_views
	BOOST_AUTO_TEST_CASE(toggle_views_from_match)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(WindowListener::MATCH_DISPLAY_STATE) ;
		BOOST_CHECK_NO_THROW(gloss_placer.on_toggle_views()) ;
		BOOST_CHECK_EQUAL(WindowListener::CONCORDANCE_DISPLAY_STATE, gloss_placer.get_display_state()) ;
	}
	BOOST_AUTO_TEST_CASE(toggle_views_from_concordance_nothrow)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(WindowListener::CONCORDANCE_DISPLAY_STATE) ;
		BOOST_CHECK_NO_THROW(gloss_placer.on_toggle_views()) ;
		BOOST_CHECK_EQUAL(WindowListener::MATCH_DISPLAY_STATE, gloss_placer.get_display_state()) ;
	}
	BOOST_AUTO_TEST_CASE(toggle_views_from_init_nothrow)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(WindowListener::INIT_DISPLAY_STATE) ;
		BOOST_CHECK_NO_THROW(gloss_placer.on_toggle_views()) ;
		BOOST_CHECK_EQUAL(WindowListener::MATCH_DISPLAY_STATE, gloss_placer.get_display_state()) ;
	}
	BOOST_AUTO_TEST_CASE(toggle_views_from_new_nothrow)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(WindowListener::NEW_RECORD_DISPLAY_STATE) ;
		BOOST_CHECK_NO_THROW(gloss_placer.on_toggle_views()) ;
		BOOST_CHECK_EQUAL(WindowListener::MATCH_DISPLAY_STATE, gloss_placer.get_display_state()) ;
	}

	// get_record_translation
	BOOST_AUTO_TEST_CASE( get_record_translation_standard)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.m_properties_gloss->m_data.m_to_lower = FALSE ;
		gloss_placer.m_properties_gloss->m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"spam") ;

		wstring normalized = gloss_placer.get_record_translation(record) ;

		string expected = "spam" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(1u, record->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_lower)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.m_properties_gloss->m_data.m_to_lower = TRUE ;
		gloss_placer.m_properties_gloss->m_data.m_plaintext = FALSE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"SpaM") ;

		wstring normalized = gloss_placer.get_record_translation(record) ;

		string expected = "spam" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_record_translation_plain)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.m_properties_gloss->m_data.m_to_lower = FALSE ;
		gloss_placer.m_properties_gloss->m_data.m_plaintext = TRUE ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<bold>spam &lt; &amp; &gt; eggs</bold>") ;

		BOOST_CHECK_EQUAL(record->get_trans_plain(), L"spam < & > eggs") ;

		wstring normalized = gloss_placer.get_record_translation(record) ;

		string expected = "spam &lt; &amp; &gt; eggs" ;
		string actual = (LPCSTR)CStringA(normalized.c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	/*********************************************************************/
	/* tabbed text stuff                                                 */
	/*********************************************************************/
	BOOST_AUTO_TEST_CASE( load_tabbed_text )
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		InputDeviceFake *device = new InputDeviceFake ;
		device->set_view(string2string(wstring(L"日本語\tJapanese\n英語\tEnglish\n"), CP_UTF8)) ;
		gloss_placer.m_input_device = input_device_ptr(device) ;
		CString filename = _T("c:\\test\\tabbed-text.txt") ;
		gloss_placer.import_tabbed_text(filename) ;
		BOOST_CHECK_EQUAL(1u, gloss_placer.m_memories->size()) ;
		mem_engine::memory_pointer mem = gloss_placer.m_memories->get_first_memory() ;
		BOOST_CHECK_EQUAL(2u, mem->size()) ;
	}

	/*********************************************************************/
	/* multiterm stuff                                                   */
	/*********************************************************************/
	BOOST_AUTO_TEST_CASE( load_multiterm6 )
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CString filename = _T("c:\\test\\Multiterm.6.0.sample_small.txt") ;
		gloss_placer.import_multiterm(filename) ;
		BOOST_CHECK_EQUAL(1u, gloss_placer.m_memories->size()) ;
		mem_engine::memory_pointer mem = gloss_placer.m_memories->get_first_memory() ;
		BOOST_CHECK_EQUAL(5u, mem->size()) ;
	}
	BOOST_AUTO_TEST_CASE( export_gloss_mt55 )
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CString filename = _T("c:\\test\\mt.55.output.txt") ;
		::DeleteFile(filename) ;
		string source("Japanese") ;
		string target("English") ;

		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		record_pointer rec1(new record_local) ;
		record_pointer rec2(new record_local) ;
		rec1->set_source(L"りんご") ;
		rec1->set_trans(L"apple") ;

		rec2->set_source(L"日本語") ;
		rec2->set_trans(L"Japanese") ;

		mem->add_record(rec1) ;
		mem->add_record(rec2) ;

		gloss_placer.export_multiterm_55_sub(source, target, filename, mem) ;

		string fmt = "<%1%>%3%\r\n" ;
		fmt += "<%2%>%4%\r\n" ;
		fmt += "<Notes>-\r\n" ;
		fmt += "***\r\n" ;
		fmt += "<%1%>%5%\r\n" ;
		fmt += "<%2%>%6%\r\n" ;
		fmt += "<Notes>-\r\n" ;
		fmt += "***\r\n" ;
		string expected = (format(fmt)
			% "Japanese" % "English"
			% string2string(rec1->get_source_plain())
			% string2string(rec1->get_trans_plain())
			% string2string(rec2->get_source_plain())
			% string2string(rec2->get_trans_plain())
			).str() ;

		file::view fview ;
		string actual = (LPSTR)fview.create_view(filename) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( export_gloss_mt6)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CString filename = _T("c:\\test\\mt.6.output.txt") ;
		::DeleteFile(filename) ;

		wstring source(L"Japanese") ;
		wstring target(L"English") ;

		mem_engine::memory_pointer mem = FelixFactory().make_memory() ;
		record_pointer rec1(new record_local) ;
		record_pointer rec2(new record_local) ;
		rec1->set_source(L"りんご") ;
		rec1->set_trans(L"apple") ;

		rec2->set_source(L"日本語") ;
		rec2->set_trans(L"Japanese") ;

		mem->add_record(rec1) ;
		mem->add_record(rec2) ;

		gloss_placer.export_multiterm_6_sub(source, target, filename, mem) ;

		wstring fmt = L"Japanese\tEnglish\tNotes\n" ;
		fmt += L"りんご\tapple\t-\n" ;
		fmt += L"日本語\tJapanese\t-\n" ;
		string expected = string2string(fmt, CP_UTF8) ;

		file::view fview ;
		LPWSTR raw_text = (LPWSTR)fview.create_view(filename) ;
		string actual = string2string(wstring(raw_text+1), CP_UTF8) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// translation concordance
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( test_is_trans_concordance_initially_false )
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		BOOST_CHECK( !gloss_placer.m_is_trans_concordance ) ;
	}
	BOOST_AUTO_TEST_CASE( test_is_trans_concordance_set_to_true )
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.get_translation_concordances(L"foo") ;
		BOOST_CHECK(gloss_placer.m_is_trans_concordance ) ;
	}
	BOOST_AUTO_TEST_CASE( test_is_trans_concordance_set_to_false )
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.m_is_trans_concordance = true ;
		gloss_placer.get_concordances(L"foo") ;
		BOOST_CHECK( ! gloss_placer.m_is_trans_concordance ) ;
	}


	//////////////////////////////////////////////////////////////////////////
	// message processing
	//////////////////////////////////////////////////////////////////////////


	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		LRESULT lResult = 1 ;
		gloss_placer.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, gloss_placer.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_sensing_variable[0], "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		LRESULT lResult = 1 ;
		gloss_placer.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, gloss_placer.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_sensing_variable[0], "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		LRESULT lResult = 1 ;
		gloss_placer.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u,gloss_placer.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_sensing_variable[0], "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		LRESULT lResult = 1 ;
		BOOL result = gloss_placer.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u,gloss_placer.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif 
