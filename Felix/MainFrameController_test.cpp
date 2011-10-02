
#include "StdAfx.h"
#include "mainfrm.h"
#include "record_local.h"
#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "memory_local.h"
#include "record_local.h"
#include "felix_model_fake.h"
#include "felix_factory.h"
#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( MainFrameControllerTests )


	using namespace mem_engine ;
	using namespace except ;

	frame_ptr make_fake_frame()
	{
		app_props::props_ptr props(new app_props::properties); 
		model_iface_ptr the_model(new FelixModelInterfaceFake);
		return frame_ptr(new CMainFrame( the_model, props )) ;
	}

	BOOST_AUTO_TEST_CASE( TestCallCreate)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[1], "on_create" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallDestroy)
	{
		frame_ptr frame = make_fake_frame() ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[1], "on_destroy" ) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallClose)
	{
		frame_ptr frame = make_fake_frame() ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_CLOSE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_close" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( TestCallDrop)
	{
		frame_ptr frame = make_fake_frame() ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_DROPFILES, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_drop" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	// user messages
	BOOST_AUTO_TEST_CASE( Teston_user_save )
	{
		frame_ptr frame = make_fake_frame() ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, USER_SAVE_MEMORIES, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(4, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_save" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "on_file_save" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[3], "empty" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_lookup_source)
	{
		frame_ptr frame = make_fake_frame() ;
		frame->m_deferred_query = L"foo" ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, USER_LOOKUP_SOURCE, USER_LOOKUP_SOURCE, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "CMainFrame::on_user_lookup_source" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Found 0 matches." ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_lookup_trans )
	{
		frame_ptr frame = make_fake_frame() ;
		frame->m_deferred_query = L"foo" ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, USER_LOOKUP_TRANS, USER_LOOKUP_TRANS, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[1], "CMainFrame::on_user_lookup_trans") ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Found 0 matches." ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}



	BOOST_AUTO_TEST_CASE( Teston_user_edit_search )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_EDIT_FIND, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_edit_search" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_search )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_USER_SEARCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_search" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Found 0 matches." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_replace_edit_record )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_REPLACE_EDIT_RECORD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_replace_edit_record" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_edit_replace )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_EDIT_REPLACE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_edit_replace" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_user_view_min_end )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_MIN_VIEW_END, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_view_min_end" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	// WM_COMMAND
	BOOST_AUTO_TEST_CASE( Teston_demo_check_excess_memories )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_DEMO_CHECK_EXCESS, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_demo_check_excess_memories" ) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_check_demo)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_CHECK_DEMO, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_check_demo" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_source_concordance )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_SOURCE_CONCORDANCE_SEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_source_concordance" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Found 0 concordances for string [spam]." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_trans_concordance)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_TRANS_CONCORDANCE_SEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_trans_concordance" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Found 0 concordances for string [spam]." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_tools_memory_manager)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_MEMORY_MGR, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_tools_memory_manager" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_underline)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_UNDERLINE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_underline" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_bold)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_BOLD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_bold" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_italic)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_ITALIC, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_italic" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_format_background_color)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FORMAT_BGCOLOR, 0, lResult, 0)  ;

		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "OnFormatBackgroundColor"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "#ffffff"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
}
	BOOST_AUTO_TEST_CASE( Teston_file_exit)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_APP_EXIT, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_file_exit" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_new_glossary)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_GLOSSARY_NEW, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_new_glossary" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( Teston_file_new)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_NEW, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_file_new" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
	BOOST_AUTO_TEST_CASE( Teston_file_open)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_OPEN, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_file_open" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_file_save)
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_file_save" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "empty" ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}

	BOOST_AUTO_TEST_CASE( on_mru_file_open)
	{
		frame_ptr frame = make_fake_frame() ;


		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;
		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_bacon.xml") ) )  ;

		LRESULT lResult = 1 ;

		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_mru_file_open" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Loading file [spam_n_bacon.xml]..." ) ;

		BOOST_CHECK_EQUAL(0, (int)lResult) ;

		frame->m_sensing_variable.clear() ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST+1, 0, lResult, 0)  ;

		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_mru_file_open" ) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;

	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_tmx)
	{
		frame_ptr frame = make_fake_frame() ;


		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\sample.tmx") ) )  ;

		LRESULT lResult = 1 ;

		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(4, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_mru_file_open" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "Loading file [sample.tmx]..." ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[3], "...Done! Loaded 2 records from file [sample.tmx]." ) ;
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_direct_call)
	{
		frame_ptr frame = make_fake_frame() ;


		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;

		WindowsMessage dummy ;
		dummy.wParam = MAKEWPARAM(ID_FILE_MRU_FIRST, 0) ;
		try
		{
			BOOST_CHECK_EQUAL(0u, frame->m_model->get_memories()->size() ) ;
			frame->on_mru_file_open(dummy)  ;
			BOOST_CHECK_EQUAL(1u, frame->m_model->get_memories()->size() ) ;
		}
		catch (CException&)
		{
			BOOST_FAIL("Should not fail to open MRU item") ;
		}
	}
	BOOST_AUTO_TEST_CASE( on_mru_file_open_bogus_xml)
	{
		frame_ptr frame = make_fake_frame() ;


		frame->m_mru.AddStringToList(tstring(_T("c:\\bogus.xml") ) )  ;
		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;

		WindowsMessage dummy ;
		dummy.wParam = MAKEWPARAM(ID_FILE_MRU_FIRST+1, 0) ;

		try
		{
			frame->on_mru_file_open(dummy)  ;
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
		frame_ptr frame = make_fake_frame() ;


		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;
		frame->m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_bacon.xml") ) )  ;

		WindowsMessage dummy ;

		for ( WORD i=ID_FILE_MRU_FIRST +2; i <=ID_FILE_MRU_LAST+5 ; ++i )
		{
			dummy.wParam = MAKEWPARAM(i, 0) ;
			BOOST_CHECK_THROW(frame->on_mru_file_open(dummy), CException)  ;
		}
	}
	BOOST_AUTO_TEST_CASE( Teston_register_gloss )
	{
		frame_ptr frame = make_fake_frame() ;

		LRESULT lResult = 1 ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		frame->m_search_matches.set_matches( matches ) ;

		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_SET_GLOSS, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "[Other display state]" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "on_user_register" ) ;

	}
	BOOST_AUTO_TEST_CASE( TestOtherCommands)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE_AS, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_file_save_as" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_FILE_SAVE_ALL)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE_ALL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_file_save_all" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_IDD_EDIT_ENTRY)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDD_EDIT_ENTRY, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_edit_entry" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "on_user_edit" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_EDIT_DELETE)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_DELETE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_delete_entry" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_EDIT_ADD)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_ADD, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_add" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_EDIT_FIND)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_FIND, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
	}
	BOOST_AUTO_TEST_CASE( Test_ID_FIND_QUICKSEARCH)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_FIND_QUICKSEARCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[1], "on_find") ;
	}
	BOOST_AUTO_TEST_CASE( Test_IDD_CONCORDANCE)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDD_CONCORDANCE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_concordance" ) ;
	}
	BOOST_AUTO_TEST_CASE( TestID_EDIT_REPLACE)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_REPLACE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_edit_replace" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_TOOLBAR)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;		
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_TOOLBAR, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_toolbar" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_STATUS_BAR)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_STATUS_BAR, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_status_bar" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_EDIT_MODE)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_EDIT_MODE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_edit_mode" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_MATCH)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_MATCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_match" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_SEARCH)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND,ID_VIEW_SEARCH , 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_search" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_VIEW_SWITCH)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_SWITCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_switch" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_IDC_MIN_VIEW_BEGIN)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, IDC_MIN_VIEW_BEGIN, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_view_min_begin" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_APP_ABOUT)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_APP_ABOUT, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "show_about_dialog" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_HELP)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_help" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_HELP_FAQ)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP_FAQ, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_help_faq" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_get_help_file_path)
	{
		CStringA help_file_path(get_help_file_path(get_docs_path())) ;
		BOOST_CHECK_EQUAL("http://felix-cat.com/media/manuals/felix/", help_file_path) ;
	}

	BOOST_AUTO_TEST_CASE( Test_ID_HELP_REGISTER)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP_REGISTER, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_help_register" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_TOOLS_PREFERENCES)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_PREFERENCES, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_tools_preferences" ) ;
	}
	BOOST_AUTO_TEST_CASE( Test_ID_TOOLS_LANGUAGE)
	{
		frame_ptr frame = make_fake_frame() ;


		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_LANGUAGE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key");
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_tools_switch_language" ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif 


