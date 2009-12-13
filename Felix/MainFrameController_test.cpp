
#include "StdAfx.h"
#include "mainfrm.h"
#include "MainFrameModel.h"
#include "record_local.h"
#include "easyunit/testharness.h"

#ifdef UNIT_TEST


namespace easyunit
{
	TEST( MainFrameControllerTests, TestInstantiate)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		ASSERT_TRUE( true ) ;
	}
	TEST( MainFrameControllerTests, TestCallCreate)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_create" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, TestCallDestroy)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V(SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_destroy" ) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, TestCallClose)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_CLOSE, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_close" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, TestCallDrop)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_DROPFILES, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_drop" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_create)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		WindowsMessage message ;
		main_frame.on_create( message ) ;
		ASSERT_EQUALS_V(1, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "on_create" ) ;
	}
	// user messages
	TEST( MainFrameControllerTests, Teston_user_save )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_FILE_SAVE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(4, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_save" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "on_file_save" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[3].c_str()), "empty" ) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_user_retrieve_edit_record )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_RETRIEVE_EDIT_RECORD, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_retrieve_edit_record" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_user_edit_search )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_EDIT_FIND, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_edit_search" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_user_search )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_USER_SEARCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_search" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "Found 0 matches." ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_user_replace_edit_record )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_REPLACE_EDIT_RECORD, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_replace_edit_record" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_user_edit_replace )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, ID_EDIT_REPLACE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_edit_replace" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_user_view_min_end )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, UWM_USER_MESSAGE, IDC_MIN_VIEW_END, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_view_min_end" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	// WM_COMMAND
	TEST( MainFrameControllerTests, Teston_demo_check_excess_memories )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_DEMO_CHECK_EXCESS, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_demo_check_excess_memories" ) ;
	}
	TEST( MainFrameControllerTests, Teston_check_demo)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_CHECK_DEMO, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_check_demo" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_source_concordance )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SOURCE_CONCORDANCE_SEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_source_concordance" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "Found 0 concordances for string [spam]." ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_trans_concordance)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_TRANS_CONCORDANCE_SEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_trans_concordance" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "Found 0 concordances for string [spam]." ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}

	TEST( MainFrameControllerTests, Teston_tools_memory_manager)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_MEMORY_MGR, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_tools_memory_manager" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_underline)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_UNDERLINE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_underline" ) ;
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_bold)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_BOLD, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_bold" ) ;
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}

	TEST( MainFrameControllerTests, Teston_italic)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_ITALIC, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_italic" ) ;
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_format_background_color)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FORMAT_BGCOLOR, 0, lResult, 0)  ;

		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "OnFormatBackgroundColor"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "#ffffff"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
}
	TEST( MainFrameControllerTests, Teston_file_exit)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_APP_EXIT, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_file_exit" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}

	TEST( MainFrameControllerTests, Teston_new_glossary)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_GLOSSARY_NEW, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_new_glossary" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, Teston_file_new)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_NEW, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_file_new" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
}
	TEST( MainFrameControllerTests, Teston_file_open)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_OPEN, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_file_open" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}

	TEST( MainFrameControllerTests, Teston_file_save)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_file_save" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "empty" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}

	TEST( MainFrameControllerTests, on_mru_file_open)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;
		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_bacon.xml") ) )  ;

		LRESULT lResult = 1 ;

		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_mru_file_open" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "Loading file [spam_n_bacon.xml]..." ) ;

		ASSERT_EQUALS_V(0, (int)lResult) ;

		main_frame.m_sensing_variable.clear() ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST+1, 0, lResult, 0)  ;

		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_mru_file_open" ) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;

	}
	TEST( MainFrameControllerTests, on_mru_file_open_tmx)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\sample.tmx") ) )  ;

		LRESULT lResult = 1 ;

		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_MRU_FIRST, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(5, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_mru_file_open" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "Loading file [sample.tmx]..." ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[3].c_str()), "Failed to load file [sample.tmx]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[4].c_str()), "Failed to load file [sample.tmx]" ) ;
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( MainFrameControllerTests, on_mru_file_open_direct_call)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;

		WindowsMessage dummy ;
		dummy.wParam = MAKEWPARAM(ID_FILE_MRU_FIRST, 0) ;
		try
		{
			ASSERT_EQUALS_V(0, (int)main_frame.m_model->m_memories->size() ) ;
			main_frame.on_mru_file_open(dummy)  ;
			ASSERT_EQUALS_V(1, (int)main_frame.m_model->m_memories->size() ) ;
		}
		catch (CException&)
		{
			FAIL_M("Should not fail to open MRU item") ;
		}
	}
	TEST( MainFrameControllerTests, on_mru_file_open_bogus_xml)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\bogus.xml") ) )  ;
		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;

		WindowsMessage dummy ;
		dummy.wParam = MAKEWPARAM(ID_FILE_MRU_FIRST+1, 0) ;

		try
		{
			main_frame.on_mru_file_open(dummy)  ;
			FAIL_M("Should have thrown for bogus file") ;
		}
		catch (CWinException&)
		{
			ASSERT_TRUE(true) ;
		}
		catch (CException&)
		{
			FAIL_M("Threw wrong kind of exception") ;
		}
	}
	TEST( MainFrameControllerTests, on_mru_file_open_out_of_index)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_eggs.xml") ) )  ;
		main_frame.m_mru.AddStringToList(tstring(_T("c:\\test\\spam_n_bacon.xml") ) )  ;

		WindowsMessage dummy ;

		for ( WORD i=ID_FILE_MRU_FIRST +2; i <=ID_FILE_MRU_LAST+5 ; ++i )
		{
			try
			{
				dummy.wParam = MAKEWPARAM(i, 0) ;
				main_frame.on_mru_file_open(dummy)  ;
				CStringA fail_msg ;
				fail_msg.Format("Should not have entry %d", i-ID_FILE_MRU_FIRST) ;
				FAIL_M(SimpleString(fail_msg)) ;
			}
			catch (CException &e)
			{
				e ;
			}
		}
	}
	TEST( MainFrameControllerTests, Teston_register_gloss )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		LRESULT lResult = 1 ;

		std::vector< memory_engine::search_match_ptr > matches ;
		memory_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;

		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SET_GLOSS, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "[Other display state]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "on_user_register" ) ;

	}
	TEST( MainFrameControllerTests, TestOtherCommands)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE_AS, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_file_save_as" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_FILE_SAVE_ALL)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FILE_SAVE_ALL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_file_save_all" ) ;
	}
	TEST( MainFrameControllerTests, Test_IDD_EDIT_ENTRY)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDD_EDIT_ENTRY, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_edit_entry" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "on_user_edit" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_EDIT_DELETE)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_DELETE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_delete_entry" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_EDIT_ADD)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_ADD, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_add" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_EDIT_FIND)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_FIND, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
	}
	TEST( MainFrameControllerTests, Test_ID_FIND_QUICKSEARCH)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_FIND_QUICKSEARCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V(SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_find") ;
	}
	TEST( MainFrameControllerTests, Test_IDD_CONCORDANCE)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDD_CONCORDANCE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_concordance" ) ;
	}
	TEST( MainFrameControllerTests, TestID_EDIT_REPLACE)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_EDIT_REPLACE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_edit_replace" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_VIEW_TOOLBAR)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;		
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_TOOLBAR, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_toolbar" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_VIEW_STATUS_BAR)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_STATUS_BAR, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_status_bar" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_VIEW_EDIT_MODE)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_EDIT_MODE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_edit_mode" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_VIEW_MATCH)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_MATCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_match" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_VIEW_SEARCH)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND,ID_VIEW_SEARCH , 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_search" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_VIEW_SWITCH)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_VIEW_SWITCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_switch" ) ;
	}
	TEST( MainFrameControllerTests, Test_IDC_MIN_VIEW_BEGIN)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, IDC_MIN_VIEW_BEGIN, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_view_min_begin" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_APP_ABOUT)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_APP_ABOUT, 0, lResult, 0)  ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "show_about_dialog" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_HELP)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_help" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_HELP_FAQ)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP_FAQ, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_help_faq" ) ;
	}
	TEST( MainFrameControllerTests, Test_get_help_file_path)
	{
		CStringA help_file_path(get_help_file_path(get_docs_path())) ;
		ASSERT_EQUALS_V("http://felix-cat.com/media/manuals/felix/", help_file_path) ;
	}

	TEST( MainFrameControllerTests, Test_ID_HELP_REGISTER)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_HELP_REGISTER, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_help_register" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_TOOLS_PREFERENCES)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_PREFERENCES, 0, lResult, 0)  ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_tools_preferences" ) ;
	}
	TEST( MainFrameControllerTests, Test_ID_TOOLS_LANGUAGE)
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;

		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_COMMAND, ID_TOOLS_LANGUAGE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key");
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_tools_switch_language" ) ;
	}

}

#endif 


