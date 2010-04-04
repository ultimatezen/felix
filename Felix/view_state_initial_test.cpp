#include "stdafx.h"
#include "view_state_initial.h"
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

	TEST( view_state_initial_test, handle_toggle_edit_mode_false )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;

		ViewStateInitialMain state ;
		state.set_view(&view) ;
		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(view.is_edit_mode()) ;
	}

	TEST( view_state_initial_test, handle_toggle_edit_mode_true )
	{
		view_interface_fake view ;
		view.put_edit_mode(true) ;
		ASSERT_TRUE(view.is_edit_mode()) ;

		ViewStateInitialMain state ;
		state.set_view(&view) ;
		state.handle_toggle_edit_mode() ;

		ASSERT_TRUE(! view.is_edit_mode()) ;
	}

	TEST( view_state_initial_test, show_content )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;

		ViewStateInitialMain state ;
		state.set_view(&view) ;
		state.show_content() ;

		ASSERT_EQUALS_V(5, (int)view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[1].c_str()), "ensure_document_complete") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[2].c_str()), "navigate") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[3].c_str()), "C:/Users/Ryan/AppData/Local/Felix/html/en/start.html") ;
		ASSERT_EQUALS_V(SimpleString(view.m_sensing_variable[4].c_str()), "ensure_document_complete") ;
	}

	TEST( view_state_initial_test, retrieve_edit_record_model )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		model.m_model->insert_memory(mem) ;

		ViewStateInitialMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(mem->get_id(), rec) ;

		ASSERT_EQUALS_V(1, (int)model.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(model.m_sensing_variable[0].c_str()), "get_memory_by_id") ;
	}
	TEST( view_state_initial_test, retrieve_edit_record_listener )
	{
		view_interface_fake view ;
		WindowListenerFake listener; 
		ASSERT_TRUE(! listener.new_rec->is_valid_record()) ;
		FelixModelInterfaceFake model ;
		memory_pointer mem(new memory_local) ;
		model.get_memories()->insert_memory(mem) ;

		ViewStateInitialMain state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.set_model(&model) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		state.retrieve_edit_record(mem->get_id(), rec) ;

		ASSERT_EQUALS_V(2, (int)listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[0].c_str()), "get_new_record") ;
		ASSERT_EQUALS_V(SimpleString(listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_TRUE(listener.new_rec->is_valid_record()) ;
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

}


#endif // #ifdef UNIT_TEST