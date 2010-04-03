#include "stdafx.h"
#include "view_state_concordance.h"
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

	TEST( view_state_concordance_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateConcordanceMain state ;
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

	TEST( view_state_concordance_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.m_is_edit_mode = true ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateConcordanceMain state ;
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

	TEST( view_state_concordance_test, show_content )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateConcordanceMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.show_content() ;

	}

	TEST( view_state_concordance_test, retrieve_edit_record_model )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.m_model->insert_memory(mem) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		mem->add_record(rec) ;

		// current match
		listener.item_under_edit->set_record(rec) ;
		listener.item_under_edit->set_memory_id(mem->get_id()) ;
		mem->add_record(rec) ;

		ViewStateConcordanceMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;
		search_query_mainframe search_matches ;
		state.set_search_matches(&search_matches) ;

		record_pointer new_rec = record_pointer(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(mem->get_id(), new_rec) ;

		ASSERT_EQUALS_V(2, (int)model.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(model.m_sensing_variable[0].c_str()), "get_memory_by_id") ;
		ASSERT_EQUALS_V(SimpleString(model.m_sensing_variable[1].c_str()), "get_memories") ;

		SimpleString expected("source") ;
		SimpleString actual(string2string(listener.item_under_edit->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( view_state_concordance_test, retrieve_edit_record_listener )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		ASSERT_TRUE(! listener.new_rec->is_valid_record()) ;
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		mem->add_record(rec) ;

		// current match
		listener.item_under_edit->set_record(rec) ;
		listener.item_under_edit->set_memory_id(mem->get_id()) ;
		mem->add_record(rec) ;

		ViewStateConcordanceMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;
		search_query_mainframe search_matches ;
		state.set_search_matches(&search_matches) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(mem->get_id(), new_rec) ;

		ASSERT_EQUALS_V(5, (int)listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[2].c_str()), "user_feedback") ;
		SimpleString expected_msgid(int2string(IDS_ADDED_TRANSLATION).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[3].c_str()), expected_msgid) ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[4].c_str()), "0") ;
	}
	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////

	TEST( view_state_concordance_gloss_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateConcordanceGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;
		search_query_mainframe search_matches ;
		state.set_search_matches(&search_matches) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)view.m_sensing_variable.size()) ;
	}

	TEST( view_state_concordance_gloss_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.m_is_edit_mode = true ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem = memory_pointer(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateConcordanceGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(6, (int)view.m_sensing_variable.size()) ;
	}
	TEST( view_state_concordance_gloss_test, show_content )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;
		WindowListenerFake listener; 

		ViewStateConcordanceGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.show_content() ;

	}

}


#endif // #ifdef UNIT_TEST