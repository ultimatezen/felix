/*!
	@brief Unit tests for CMainFrame class
	@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "MainFrm.h"
#include "record_local.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST


namespace easyunit
{
	using namespace mem_engine;

	LRESULT FakeFreeFunction( WindowsMessage &message )
	{
		message ;
		return 0L ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Windows message handling
	//////////////////////////////////////////////////////////////////////////
	TEST( CMainFrameMessageTestCase, TestMessageMap )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		ASSERT_EQUALS_V( 4, (int)main_frame.m_message_map.size()) ;
	}
	TEST( CMainFrameMessageTestCase, TestMessageMapAddFunction )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		ASSERT_EQUALS_V( 4, (int)main_frame.m_message_map.size()) ;
		main_frame.register_event_listener( WM_CREATE, FakeFreeFunction ) ;
		ASSERT_EQUALS_V( 4, (int)main_frame.m_message_map.size()) ;
	}

	TEST( CMainFrameMessageTestCase, Teston_create )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		WindowsMessage message ;
		main_frame.on_create(message) ;
		ASSERT_EQUALS_V(1, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "on_create" ) ;
	}

	TEST( CMainFrameMessageTestCase, Teston_close )
	{
		MainFrameModel model ;
		CMainFrame main_frame(&model) ;
		WindowsMessage message ;
		main_frame.on_close(message) ;
		ASSERT_EQUALS_V(1, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "on_close" ) ;
		ASSERT_EQUALS_M( message.isHandled(), false, "We should not handle the closed message here" ) ;
	}

	TEST( CMainFrameMessageTestCase, Teston_destroy )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;
		main_frame.on_destroy(message) ;
		ASSERT_EQUALS_V(1, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "on_destroy" ) ;
		ASSERT_EQUALS_M( message.isHandled(), false, "We should not handle the closed message here" ) ;
	}

	TEST( CMainFrameMessageTestCase, Test_get_message_map )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		CMainFrame::messageMapType *mmap = main_frame.get_message_map( WM_CREATE ) ;
		ASSERT_EQUALS_M( mmap, &main_frame.m_message_map, "Should get the standard message map" ) ;

		mmap = main_frame.get_message_map( UWM_USER_MESSAGE ) ;
		ASSERT_EQUALS_M( mmap, &main_frame.m_user_message_map, "Should get the user message map" ) ;

		mmap = main_frame.get_message_map( WM_COMMAND ) ;
		ASSERT_EQUALS_M( mmap, &main_frame.m_command_message_map, "Should get the user message map" ) ;
	}

	TEST( CMainFrameMessageTestCase, Test_get_message_key )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		UINT key = main_frame.get_message_key( WM_CREATE, 7 ) ;
		ASSERT_EQUALS_M( key, WM_CREATE, "Should get the message as key" ) ;

		key = main_frame.get_message_key( UWM_USER_MESSAGE, 10 ) ;
		ASSERT_EQUALS_M( key, 10, "Should get the wParam as key" ) ;

		key = main_frame.get_message_key( WM_COMMAND, MAKEWPARAM( 15, 20 ) ) ;
		ASSERT_EQUALS_M( key, 15, "Should get the low word of wParam as key" ) ;
	}

	TEST( CMainFrameMessageTestCase, Test_register_glossary_entries_no_display_state )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;
		main_frame.m_trans_matches.set_matches( matches ) ;

		main_frame.on_register_gloss(message) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "[Other display state]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_register" ) ;

	}
	TEST( CMainFrameMessageTestCase, Test_register_glossary_entries_NEW_RECORD_DISPLAY_STATE )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;
		main_frame.m_trans_matches.set_matches( matches ) ;

		main_frame.set_display_state( CMainFrame::NEW_RECORD_DISPLAY_STATE ) ;
		main_frame.on_register_gloss(message) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "[NEW_RECORD_DISPLAY_STATE]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_register" ) ;


	}
	TEST( CMainFrameMessageTestCase, Test_register_glossary_entries_MATCH_DISPLAY_STATE )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;
		main_frame.m_trans_matches.set_matches( matches ) ;

		main_frame.set_display_state( CMainFrame::MATCH_DISPLAY_STATE ) ;
		main_frame.on_register_gloss(message) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "[MATCH_DISPLAY_STATE]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_register" ) ;


	}
	TEST( CMainFrameMessageTestCase, Test_register_glossary_entries_CONCORDANCE_DISPLAY_STATE )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;
		main_frame.m_trans_matches.set_matches( matches ) ;

		main_frame.set_display_state( CMainFrame::CONCORDANCE_DISPLAY_STATE ) ;
		main_frame.on_register_gloss(message) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "[CONCORDANCE_DISPLAY_STATE]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_register" ) ;


	}
	TEST( CMainFrameMessageTestCase, Test_register_glossary_entries_INIT_DISPLAY_STATE )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;
		main_frame.m_trans_matches.set_matches( matches ) ;

		main_frame.set_display_state( CMainFrame::INIT_DISPLAY_STATE ) ;
		main_frame.on_register_gloss(message) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "[Other display state]" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_register" ) ;


	}


	//////////////////////////////////////////////////////////////////////////
	// messages
	//////////////////////////////////////////////////////////////////////////
	TEST( CMainFrameMessages, on_create )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "Found message key"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_create" ) ;

		ASSERT_EQUALS_M( 0, lResult, "lResult should equal 0") ;
	}

	TEST( CMainFrameMessages, OnNavEdit )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnNavEdit(0) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "OnNavEdit"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_edit" ) ;
	}
	TEST( CMainFrameMessages, OnBeforeNavigateNavEdit )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnBeforeNavigate2( _bstr_t(L"100:edit:0") ) ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "OnBeforeNavigate2"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "OnNavEdit"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "on_user_edit" ) ;
	}

	TEST( CMainFrameMessages, OnNavDelete)
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnNavDelete(0) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "OnNavDelete"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_delete" ) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[2].c_str()), "No Matches" ) ;
		ASSERT_EQUALS_V(3, (int)main_frame.m_sensing_variable.size()) ;
	}

	TEST( CMainFrameMessages, TestOnNavAddToGloss )
	{
		MainFrameModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnNavAddToGloss(0) ;
		ASSERT_EQUALS_V(2, (int)main_frame.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[0].c_str()), "OnNavAddToGloss"); 
		ASSERT_EQUALS_V( SimpleString(main_frame.m_sensing_variable[1].c_str()), "on_user_add_to_glossary" ) ;
	}

}

#endif 


