
#include "StdAfx.h"
#include "mainfrm.h"
#include "record_local.h"
#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "felix_model_fake.h"
#include "memory_local.h"
#include "record_local.h"
#include "felix_model_fake.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( MainFrameControllerTests )


	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( TestInstantiate)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		BOOST_CHECK( true ) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallCreate)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_create" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallDestroy)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[1].c_str()), "on_destroy" ) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallClose)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_CLOSE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_close" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallDrop)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_DROPFILES, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_drop" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_create)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		WindowsMessage message ;
		main_frame.on_create( message ) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "on_create" ) ;
	}
	// user messages
	BOOST_AUTO_TEST_CASE( Teston_user_save )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, USER_SAVE_MEMORIES, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(4, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_save" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "on_file_save" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[3].c_str()), "empty" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_lookup_source)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		main_frame.m_deferred_query = L"foo" ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, USER_LOOKUP_SOURCE, USER_LOOKUP_SOURCE, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "CMainFrame::on_user_lookup_source" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Found 0 matches." ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_lookup_trans )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		main_frame.m_deferred_query = L"foo" ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, USER_LOOKUP_TRANS, USER_LOOKUP_TRANS, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[1].c_str()), "CMainFrame::on_user_lookup_trans") ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Found 0 matches." ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_retrieve_edit_record_message )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		view_interface_fake view ;
		WindowListenerFake listener; 
		memory_pointer mem(new memory_local) ;
		model.m_model->insert_memory(mem) ;

		main_frame.m_view_state_initial.set_view(&view) ;
		main_frame.m_view_state_initial.set_window_listener(&listener) ;

		main_frame.m_editor->set_memory_id(1) ;
		LRESULT lResult = CMainFrame::INIT_DISPLAY_STATE ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_RETRIEVE_EDIT_RECORD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_retrieve_edit_record" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_retrieve_edit_record_state )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		view_interface_fake view ;
		WindowListenerFake listener; 
		memory_pointer mem(new memory_local) ;
		model.m_model->insert_memory(mem) ;

		main_frame.m_view_state_initial.set_view(&view) ;
		main_frame.m_view_state_initial.set_window_listener(&listener) ;

		main_frame.m_editor->set_memory_id(mem->get_id()) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		main_frame.m_editor->set_new_record(rec) ;
		LRESULT lResult = CMainFrame::INIT_DISPLAY_STATE ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_RETRIEVE_EDIT_RECORD, 0, lResult, 0)  ;

		BOOST_CHECK_EQUAL(1u, model.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(model.m_sensing_variable[0].c_str()), "get_memory_by_id") ;
	}

	BOOST_AUTO_TEST_CASE( Teston_user_edit_search )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_EDIT_FIND, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_edit_search" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_search )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_USER_SEARCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_search" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Found 0 matches." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_replace_edit_record )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_REPLACE_EDIT_RECORD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_replace_edit_record" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_edit_replace )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_EDIT_REPLACE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_edit_replace" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_view_min_end )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_MIN_VIEW_END, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_user_view_min_end" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	// WM_COMMAND
	BOOST_AUTO_TEST_CASE( Teston_demo_check_excess_memories )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_DEMO_CHECK_EXCESS, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_demo_check_excess_memories" ) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_check_demo)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_CHECK_DEMO, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_check_demo" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_source_concordance )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SOURCE_CONCORDANCE_SEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_source_concordance" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Found 0 concordances for string [spam]." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_trans_concordance)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_TRANS_CONCORDANCE_SEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_trans_concordance" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Found 0 concordances for string [spam]." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_tools_memory_manager)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_MEMORY_MGR, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_tools_memory_manager" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_underline)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_UNDERLINE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_underline" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_bold)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_BOLD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_bold" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_italic)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_ITALIC, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_italic" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_format_background_color)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FORMAT_BGCOLOR, 0, lResult, 0)  ;

		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "OnFormatBackgroundColor"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "#ffffff"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
}
	BOOST_AUTO_TEST_CASE( Teston_file_exit)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_APP_EXIT, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_file_exit" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_new_glossary)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_GLOSSARY_NEW, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_new_glossary" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_file_new)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_NEW, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_file_new" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
	BOOST_AUTO_TEST_CASE( Teston_file_open)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_OPEN, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_file_open" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_file_save)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_file_save" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "empty" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( on_mru_file_open)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;
		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_bacon.xml") ) )  ;

		LRESULT lResult = 1 ;

		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_mru_file_open" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Loading file [spam_n_bacon.xml]..." ) ;

		BOOST_CHECK_EQUAL(0, (int)lResult) ;

		main_frame.m_sensing_variable.clear() ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST+1, 0, lResult, 0)  ;

		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_mru_file_open" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;

	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_tmx)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\sample.tmx") ) )  ;

		LRESULT lResult = 1 ;

		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(5, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_mru_file_open" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "Loading file [sample.tmx]..." ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[3].c_str()), "Failed to load file [sample.tmx]" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[4].c_str()), "Failed to load file [sample.tmx]" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_direct_call)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;

		WindowsMessage dummy ;
		dummy.wParam = MAKEWPARAM(ID_FILE_MRU_FIRST, 0) ;
		try
		{
			BOOST_CHECK_EQUAL(0u, main_frame.m_model->get_memories()->size() ) ;
			main_frame.on_mru_file_open(dummy)  ;
			BOOST_CHECK_EQUAL(1u, main_frame.m_model->get_memories()->size() ) ;
		}
		catch (CException&)
		{
			BOOST_FAIL("Should not fail to open MRU item") ;
		}
	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_bogus_xml)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\bogus.xml") ) )  ;
		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;

		WindowsMessage dummy ;
		dummy.wParam = MAKEWPARAM(ID_FILE_MRU_FIRST+1, 0) ;

		try
		{
			main_frame.on_mru_file_open(dummy)  ;
			BOOST_FAIL("Should have thrown for bogus file") ;
		}
		catch (CWinException&)
		{
			BOOST_CHECK(true) ;
		}
		catch (CException&)
		{
			BOOST_FAIL("Threw wrong kind of exception") ;
		}
	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_out_of_index)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;
		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_bacon.xml") ) )  ;

		WindowsMessage dummy ;

		for ( WORD i=ID_FILE_MRU_FIRST +2; i <=ID_FILE_MRU_LAST+5 ; ++i )
		{
			dummy.wParam = MAKEWPARAM(i, 0) ;
			BOOST_CHECK_THROW(main_frame.on_mru_file_open(dummy), CException)  ;
		}
	}
	BOOST_AUTO_TEST_CASE( Teston_register_gloss )
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;

		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SET_GLOSS, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "[Other display state]" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "on_user_register" ) ;

	}
	BOOST_AUTO_TEST_CASE( TestOtherCommands)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE_AS, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_file_save_as" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_FILE_SAVE_ALL)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE_ALL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_file_save_all" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_IDD_EDIT_ENTRY)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDD_EDIT_ENTRY, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_edit_entry" ) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[2].c_str()), "on_user_edit" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_EDIT_DELETE)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_DELETE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_delete_entry" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_EDIT_ADD)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_ADD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_add" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_EDIT_FIND)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_FIND, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
	}
	BOOST_AUTO_TEST_CASE( Test_ID_FIND_QUICKSEARCH)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FIND_QUICKSEARCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL(string(main_frame.m_sensing_variable[1].c_str()), "on_find") ;
	}
	BOOST_AUTO_TEST_CASE( Test_IDD_CONCORDANCE)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDD_CONCORDANCE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_concordance" ) ;
	}
	BOOST_AUTO_TEST_CASE( TestID_EDIT_REPLACE)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_REPLACE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_edit_replace" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_TOOLBAR)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_TOOLBAR, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_toolbar" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_STATUS_BAR)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_STATUS_BAR, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_status_bar" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_EDIT_MODE)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_EDIT_MODE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_edit_mode" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_MATCH)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_MATCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_match" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_SEARCH)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND,ID_VIEW_SEARCH , 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_search" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_SWITCH)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_SWITCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_switch" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_IDC_MIN_VIEW_BEGIN)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_MIN_VIEW_BEGIN, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_view_min_begin" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_APP_ABOUT)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_APP_ABOUT, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "show_about_dialog" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_HELP)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_help" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_HELP_FAQ)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP_FAQ, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_help_faq" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_get_help_file_path)
	{
		CStringA help_file_path(get_help_file_path(get_docs_path())) ;
		BOOST_CHECK_EQUAL("http://felix-cat.com/media/manuals/felix/", help_file_path) ;
	}

	BOOST_AUTO_TEST_CASE( Test_ID_HELP_REGISTER)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP_REGISTER, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_help_register" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_TOOLS_PREFERENCES)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_PREFERENCES, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_tools_preferences" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_TOOLS_LANGUAGE)
	{
		FelixModelInterfaceFake model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_LANGUAGE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		BOOST_CHECK_EQUAL( string(main_frame.m_sensing_variable[1].c_str()), "on_tools_switch_language" ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif 


