#include "stdafx.h"
#include "view_state_initial.h"
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

	TEST( view_state_initial_test, handle_toggle_edit_mode_false )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(vso.view.is_edit_mode()) ;
	}

	TEST( view_state_initial_test, handle_toggle_edit_mode_true )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		vso.view.put_edit_mode(true) ;
		ASSERT_TRUE(vso.view.is_edit_mode()) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(! vso.view.is_edit_mode()) ;
	}

	TEST( view_state_initial_test, show_content )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.show_content() ;

		ASSERT_EQUALS_V(4, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "ensure_document_complete") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "navigate") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "C:/Users/Ryan/AppData/Local/Felix/html/en/start.html") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "ensure_document_complete") ;
	}

	TEST( view_state_initial_test, retrieve_edit_record_model )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(vso.mem->get_id(), rec) ;

		ASSERT_EQUALS_V(2, (int)vso.model.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[0].c_str()), "get_memories") ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[1].c_str()), "get_memory_by_id") ;
	}
	TEST( view_state_initial_test, retrieve_edit_record_listener )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(vso.mem->get_id(), rec) ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "get_new_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_TRUE(vso.listener.new_rec->is_valid_record()) ;
	}
	TEST( view_state_initial_test, get_current_match )
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = state.get_current_match() ;
		SimpleString expected("") ;
		SimpleString actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(vso.mem->get_id(), match->get_memory_id()) ;
	}
	TEST( view_state_initial_test, activate)
	{
		ViewStateInitialMain state ;
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

	TEST( view_state_initial_test, delete_match_empty)
	{
		ViewStateInitialMain state ;
		view_state_obj vso(&state) ;

		state.delete_match(0) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "user_feedback") ;
		SimpleString ids_no_matches(int2string(IDS_NO_MATCHES).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), ids_no_matches) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "0") ;
		ASSERT_EQUALS_V(3, (int)vso.listener.m_sensing_variable.size()) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////

	TEST( view_state_initial_gloss_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;

		ViewStateInitialGloss state ;
		state.set_view(&view) ;
		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(view.is_edit_mode()) ;
	}

	TEST( view_state_initial_gloss_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.put_edit_mode(true) ;
		ASSERT_TRUE(view.is_edit_mode()) ;

		ViewStateInitialGloss state ;
		state.set_view(&view) ;
		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(! view.is_edit_mode()) ;
	}
	TEST( view_state_initial_gloss_test, show_content )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;
		WindowListenerFake listener; 

		ViewStateInitialGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.show_content() ;

		ASSERT_EQUALS_V(5, (int)view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[1].c_str()), "ensure_document_complete") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[2].c_str()), "navigate") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[3].c_str()), "C:/Users/Ryan/AppData/Local/Felix/html/en/start_gloss.html") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[4].c_str()), "ensure_document_complete") ;
	}
	TEST( view_state_initial_gloss_test, get_current_match )
	{
		ViewStateInitialGloss state ;
		search_match_ptr match = state.get_current_match() ;
		SimpleString expected("") ;
		SimpleString actual(string2string(match->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( view_state_initial_gloss_test, delete_match_empty)
	{
		ViewStateInitialGloss state ;
		view_state_obj vso(&state) ;

		state.delete_match(0) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "user_feedback") ;
		SimpleString ids_no_matches(int2string(IDS_NO_MATCHES).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), ids_no_matches) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "0") ;
		ASSERT_EQUALS_V(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

}


#endif // #ifdef UNIT_TEST