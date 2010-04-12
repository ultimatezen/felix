#include "stdafx.h"
#include "view_state_new.h"
#include "view_state_test_utils.h"
#include "memory_local.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	//////////////////////////////////////////////////////////////////////////
	// frame
	//////////////////////////////////////////////////////////////////////////

	TEST( view_state_new_test, handle_toggle_edit_mode_false )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_new_record") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_new_test, handle_toggle_edit_mode_true )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		state.handle_toggle_edit_mode() ;
		
		ASSERT_EQUALS_V(6, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_new") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[5].c_str()), "0") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_new_test, show_content )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		state.show_content() ;

		ASSERT_EQUALS_V(4, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "ensure_document_complete") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "set_text") ;
		ASSERT_TRUE(str::starts_with(string2wstring(vso.view.m_sensing_variable[2]), L"<table class=\"added\" ID=\"table1\">")) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "ensure_document_complete") ;
	}

	TEST( view_state_new_test, retrieve_edit_record_model )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), rec) ;

		ASSERT_EQUALS_V(2, (int)vso.model.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[0].c_str()), "get_memories") ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[1].c_str()), "get_memory_by_id") ;
	}
	TEST( view_state_new_test, retrieve_edit_record_listener )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(vso.mem->get_id(), rec) ;

		ASSERT_EQUALS_V(5, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), "345") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), "0") ;
		ASSERT_TRUE(vso.listener.new_rec->is_valid_record()) ;
	}
	TEST( view_state_new_test, get_current_match )
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		vso.listener.set_new_record(rec) ;

		search_match_ptr match = state.get_current_match() ;
		SimpleString expected("source") ;
		SimpleString actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_DELTA_V(1.0, match->get_score(), 0.001) ;
	}
	TEST( view_state_new_test, activate)
	{
		ViewStateNewMain state ;
		view_state_obj vso(&state) ;

		state.activate() ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "set_menu_checkmark") ;
		SimpleString id_view_match(int2string(ID_VIEW_MATCH).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), id_view_match) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "false") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), "set_menu_checkmark") ;
		SimpleString id_view_search(int2string(ID_VIEW_SEARCH).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), id_view_search) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[5].c_str()), "false") ;
		ASSERT_EQUALS_V(6, (int)vso.listener.m_sensing_variable.size()) ;
	}

	// delete
	TEST( view_state_new_test, delete_match_check_delete)
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

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "check_delete") ;
		ASSERT_EQUALS_V(1, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(1, (int)vso.mem->size()) ;
	}

	TEST( view_state_new_test, delete_match_to_empty)
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

		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "0") ;
		ASSERT_EQUALS_V(4, (int)vso.view.m_sensing_variable.size()) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "get_new_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;

		ASSERT_EQUALS_V(0, (int)vso.mem->size()) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////

	TEST( view_state_new_gloss_test, handle_toggle_edit_mode_false )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_new_record") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_new_gloss_test, handle_toggle_edit_mode_true )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(6, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_new") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[5].c_str()), "0") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}
	TEST( view_state_new_gloss_test, show_content )
	{
		ViewStateNewGloss state ;
		view_state_obj vso(&state) ;

		state.show_content() ;

		ASSERT_EQUALS_V(6, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "ensure_document_complete") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "set_text") ;
		ASSERT_TRUE(str::starts_with(string2wstring(vso.view.m_sensing_variable[2]), L"<b>Added Glossary Entry</b>")) ;
		ASSERT_TRUE(vso.view.m_sensing_variable[2].find("No context") != string::npos) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "ensure_document_complete") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[5].c_str()), "0") ;
	}

}


#endif // #ifdef UNIT_TEST