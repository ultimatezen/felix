#include "stdafx.h"
#include "view_state_match.h"
#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "felix_model_fake.h"
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

	TEST( view_state_match_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateMatchMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_new_record") ;

		ASSERT_EQUALS_V(2, (int)listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_ENTERING_EDIT_MODE, listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_IN_EDIT_MODE, listener.m_feedback_int[1]) ;
	}

	TEST( view_state_match_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.m_is_edit_mode = true ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateMatchMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(6, (int)view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_new") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[2].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[3].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[5].c_str()), "0") ;

		ASSERT_EQUALS_V(2, (int)listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_LEAVING_EDIT_MODE, listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_LEFT_EDIT_MODE, listener.m_feedback_int[1]) ;
	}

	TEST( view_state_match_test, show_content )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateMatchMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.show_content() ;

	}

	TEST( view_state_match_test, retrieve_edit_record_model )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		model.m_model->insert_memory(mem) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		mem->add_record(rec) ;

		// current match
		listener.item_under_edit->set_record(rec) ;
		listener.item_under_edit->set_memory_id(mem->get_id()) ;

		ViewStateMatchMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(mem->get_id(), new_rec) ;

		ASSERT_EQUALS_V(1, (int)model.m_sensing_variable.size()) ;
	}
	TEST( view_state_match_test, retrieve_edit_record_replace)
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		mem->add_record(rec) ;

		// current match
		listener.item_under_edit->set_record(rec) ;
		listener.item_under_edit->set_memory_id(mem->get_id()) ;
		mem->add_record(rec) ;
		model.m_model->insert_memory(mem) ;

		ViewStateMatchMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(mem->get_id(), new_rec) ;

		ASSERT_EQUALS_V(1, (int)mem->size()) ;
		record_pointer first = *mem->get_records().begin() ;
		string actual = string2string(first->get_source_rich()) ;
		ASSERT_EQUALS_V(SimpleString("source"), SimpleString(actual.c_str())) ;
	}
	TEST( view_state_match_test, retrieve_edit_record_listener )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		ASSERT_TRUE(! listener.new_rec->is_valid_record()) ;
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		mem->add_record(rec) ;

		// current match
		listener.item_under_edit->set_record(rec) ;
		listener.item_under_edit->set_memory_id(mem->get_id()) ;
		mem->add_record(rec) ;

		ViewStateMatchMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;
		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;
		state.retrieve_edit_record(mem->get_id(), new_rec) ;

		ASSERT_EQUALS_V(6, (int)listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[2].c_str()), "redo_lookup") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[3].c_str()), "user_feedback") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[4].c_str()), "425") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[5].c_str()), "0") ;
		ASSERT_TRUE(listener.new_rec->is_valid_record()) ;
	}
	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////

	TEST( view_state_match_gloss_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;
		listener.current_match->set_memory_id(mem->get_id()) ;

		ViewStateMatchGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)view.m_sensing_variable.size()) ;
	}

	TEST( view_state_match_gloss_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.m_is_edit_mode = true ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;
		listener.current_match->set_memory_id(mem->get_id()) ;

		ViewStateMatchGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(6, (int)view.m_sensing_variable.size()) ;
	}
	TEST( view_state_match_gloss_test, show_content )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;
		WindowListenerFake listener; 

		ViewStateMatchGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.show_content() ;

	}

}


#endif // #ifdef UNIT_TEST