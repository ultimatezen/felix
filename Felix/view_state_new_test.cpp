#include "stdafx.h"
#include "view_state_new.h"
#include "view_state_test_utils.h"
#include "memory_local.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( view_state_new_test )

	using namespace mem_engine ;
	//////////////////////////////////////////////////////////////////////////
	// frame
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2u, vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_new_record") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		state.handle_toggle_edit_mode() ;
		
		BOOST_CHECK_EQUAL(6, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_new") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5].c_str()), "0") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( show_content )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "set_text") ;
		BOOST_CHECK(boost::contains(string2wstring(vso.view.m_sensing_variable[2]), L"<table class=\"added\" ID=\"table1\">")) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "ensure_document_complete") ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_edit_record_model )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), rec) ;

		BOOST_CHECK_EQUAL(2u, vso.model.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[0], "get_memories") ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[1], "get_memory_by_id") ;
	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(vso.mem->get_id(), rec) ;

		BOOST_CHECK_EQUAL(5, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3].c_str()), "345") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4].c_str()), "0") ;
		BOOST_CHECK(vso.listener.new_rec->is_valid_record()) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		vso.listener.set_new_record(rec) ;

		search_match_ptr match = state.get_current_match() ;
		string expected("source") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.001) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}
	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;
		BOOST_CHECK_EQUAL(0u, state.get_current()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_edit )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		vso.listener.set_new_record(rec) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string expected("source") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.001) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}

	BOOST_AUTO_TEST_CASE( activate)
	{
		ViewStateNewMain state ;
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

	// delete
	BOOST_AUTO_TEST_CASE( delete_match_check_delete)
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = false ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;

		vso.mem->add_record(rec) ;
		vso.listener.new_rec = rec ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "check_delete") ;
		BOOST_CHECK_EQUAL(1u, vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0u, vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(1u, vso.mem->size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_empty)
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;

		vso.mem->add_record(rec) ;
		vso.listener.new_rec = rec ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "0") ;
		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), "get_new_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;

		BOOST_CHECK_EQUAL(0u, vso.mem->size()) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( view_state_new_gloss_test )
using namespace mem_engine ;

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2u, vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_new_record") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;
		vso.listener.new_rec->set_source(L"source") ;
		vso.listener.new_rec->set_trans(L"trans") ;
		vso.view.m_is_edit_mode = true ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_new_record_glossary") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[4].find("<td class=\"match_content\" id=\"source\">source</td>") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5].c_str()), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[6].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[7].c_str()), "0") ;
		BOOST_CHECK_EQUAL(8, (int)vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}
	BOOST_AUTO_TEST_CASE( show_content )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(6, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "set_text") ;
		BOOST_CHECK(boost::starts_with(string2wstring(vso.view.m_sensing_variable[2]), L"<b>Added Glossary Entry</b>")) ;
		BOOST_CHECK(vso.view.m_sensing_variable[2].find("No context") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5].c_str()), "0") ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		vso.listener.set_new_record(rec) ;

		search_match_ptr match = state.get_current_match() ;
		string expected("source") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.001) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}
	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;
		BOOST_CHECK_EQUAL(0u, state.get_current()) ;
	}
	BOOST_AUTO_TEST_CASE( on_user_edit )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		vso.listener.set_new_record(rec) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string expected("source") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.001) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
