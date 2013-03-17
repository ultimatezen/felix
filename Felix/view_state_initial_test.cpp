#include "stdafx.h"
#include "view_state_initial.h"
#include "view_state_test_utils.h"
#include "memory_local.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( view_state_base_test )

	using namespace mem_engine ;

	//////////////////////////////////////////////////////////////////////////
	// ViewState functions
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( retrieve_record_trans_plain)
	{
		ViewStateInitialMain state ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<b>foo</b>") ;

		record_string_prefs prefs(true, false) ;

		string expected = "foo" ;
		string actual = string2string(state.retrieve_record_trans(record, prefs)).c_str() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_record_trans_plain_lower)
	{
		ViewStateInitialMain state ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<b>DOLLY</b>") ;

		record_string_prefs prefs(true, true) ;

		string expected = "dolly" ;
		string actual = string2string(state.retrieve_record_trans(record, prefs)).c_str() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_record_trans_rich_lower)
	{
		ViewStateInitialMain state ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<b>DOLLY</b>") ;

		record_string_prefs prefs(false, true) ;

		string expected = "<b>dolly</b>" ;
		string actual = string2string(state.retrieve_record_trans(record, prefs)).c_str() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_record_trans_rich)
	{
		ViewStateInitialMain state ;

		record_pointer record(new record_local) ;
		record->set_trans(L"<b>DOLLY</b>") ;

		record_string_prefs prefs(false, false) ;

		string expected = "<b>DOLLY</b>" ;
		string actual = string2string(state.retrieve_record_trans(record, prefs)).c_str() ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// frame
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( view_state_initial_test )

using namespace mem_engine ;
	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK(vso.view.is_edit_mode()) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		vso.view.put_edit_mode(true) ;
		BOOST_CHECK(vso.view.is_edit_mode()) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK(! vso.view.is_edit_mode()) ;
	}

	BOOST_AUTO_TEST_CASE( show_content )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "navigate") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "D:\\Users\\Ryan\\AppData\\Local\\Felix\\html\\en\\start.html") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "ensure_document_complete") ;
	}


	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(vso.mem->get_id(), rec, true) ;

		BOOST_CHECK_EQUAL(1u, vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "set_new_record") ;
		BOOST_CHECK(vso.listener.new_rec->is_valid_record()) ;
	}
	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = state.get_current_match() ;
		string expected("") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}

	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;
		BOOST_CHECK_EQUAL(0u, state.get_current()) ;
	}


	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;
		string expected("") ;
		string actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), current_match->get_memory_id()) ;
	}
	// activate
	BOOST_AUTO_TEST_CASE( activate)
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.activate() ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "set_menu_checkmark") ;
		string id_view_match(int2string(ID_VIEW_MATCH).c_str()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), id_view_match) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "false") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3].c_str()), "set_menu_checkmark") ;
		string id_view_search(int2string(ID_VIEW_SEARCH).c_str()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4].c_str()), id_view_search) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5].c_str()), "false") ;
		BOOST_CHECK_EQUAL(6, (int)vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_empty)
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "user_feedback") ;
		string ids_no_matches(int2string(IDS_NO_MATCHES).c_str()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), ids_no_matches) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "0") ;
		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( view_state_initial_gloss_test )

using namespace mem_engine ;
	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		BOOST_CHECK(! view.is_edit_mode()) ;

		ViewStateInitialGloss state ;
		state.set_view(&view) ;
		state.handle_toggle_edit_mode() ;

		BOOST_CHECK(view.is_edit_mode()) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.put_edit_mode(true) ;
		BOOST_CHECK(view.is_edit_mode()) ;

		ViewStateInitialGloss state ;
		state.set_view(&view) ;
		state.handle_toggle_edit_mode() ;

		BOOST_CHECK(! view.is_edit_mode()) ;
	}
	BOOST_AUTO_TEST_CASE( show_content )
	{
		view_interface_fake view ;
		BOOST_CHECK(! view.is_edit_mode()) ;
		WindowListenerFake listener; 

		ViewStateInitialGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.show_content() ;

		BOOST_CHECK_EQUAL(5, (int)view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(view.m_sensing_variable[0], "is_edit_mode") ;
		BOOST_CHECK_EQUAL(view.m_sensing_variable[1], "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(view.m_sensing_variable[2], "navigate") ;
		BOOST_CHECK_EQUAL(view.m_sensing_variable[3], "D:\\Users\\Ryan\\AppData\\Local\\Felix\\html\\en\\start_gloss.html") ;
		BOOST_CHECK_EQUAL(view.m_sensing_variable[4], "ensure_document_complete") ;
	}
	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match )
	{
		ViewStateInitialGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = state.get_current_match() ;
		string expected("") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}


	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateInitialGloss state ;
		view_state_obj vso(&state) ;
		BOOST_CHECK_EQUAL(0u, state.get_current()) ;
	}

	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit )
	{
		ViewStateInitialGloss state ;
		view_state_obj vso(&state) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;
		string expected("") ;
		string actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	// delete
	BOOST_AUTO_TEST_CASE( delete_match_empty)
	{
		ViewStateInitialGloss state ;
		view_state_obj vso(&state) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "user_feedback") ;
		string ids_no_matches(int2string(IDS_NO_MATCHES).c_str()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), ids_no_matches) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "0") ;
		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
