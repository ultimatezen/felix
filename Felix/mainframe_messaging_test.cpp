/*!
	@brief Unit tests for CMainFrame class
	@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "MainFrm.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CMainFrameMessageTestCase )


	using namespace mem_engine;

	LRESULT FakeFreeFunction( WindowsMessage &message )
	{
		message ;
		return 0L ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Windows message handling
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( TestMessageMap )
	{
		FelixModel model ;
		CMainFrame main_frame(&model) ;
		BOOST_CHECK_EQUAL( 4, (int)main_frame.m_message_map.size()) ;
	}
	BOOST_AUTO_TEST_CASE( TestMessageMapAddFunction )
	{
		FelixModel model ;
		CMainFrame main_frame(&model) ;
		BOOST_CHECK_EQUAL( 4, (int)main_frame.m_message_map.size()) ;
		main_frame.register_event_listener( WM_CREATE, FakeFreeFunction ) ;
		BOOST_CHECK_EQUAL( 4, (int)main_frame.m_message_map.size()) ;
	}

	// on_create
	BOOST_AUTO_TEST_CASE( Teston_create )
	{
		FelixModel model ;
		CMainFrame main_frame(&model) ;
		WindowsMessage message ;
		main_frame.on_create(message) ;

		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(main_frame.m_sensing_variable[0], "on_create" ) ;
	}
	BOOST_AUTO_TEST_CASE(starts_with_one_gloss_window)
	{
		FelixModel model ;
		CMainFrame mainframe(&model) ;
		WindowsMessage message ;
		mainframe.on_create(message) ;

		BOOST_CHECK_EQUAL(1u, mainframe.m_glossary_windows.size()) ;
	}

	// on_close
	BOOST_AUTO_TEST_CASE( Teston_close )
	{
		FelixModel model ;
		CMainFrame main_frame(&model) ;
		WindowsMessage message ;
		main_frame.on_close(message) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "on_close" ) ;
		BOOST_CHECK( !message.isHandled()) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_destroy )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;
		main_frame.on_destroy(message) ;
		BOOST_CHECK_EQUAL(1u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "on_destroy" ) ;
		BOOST_CHECK( !message.isHandled()) ;
	}

	BOOST_AUTO_TEST_CASE( Test_get_message_map )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;
		CMainFrame::messageMapType *mmap = main_frame.get_message_map( WM_CREATE ) ;
		BOOST_CHECK( mmap == &main_frame.m_message_map) ;

		mmap = main_frame.get_message_map( UWM_USER_MESSAGE ) ;
		BOOST_CHECK( mmap == &main_frame.m_user_message_map) ;

		mmap = main_frame.get_message_map( WM_COMMAND ) ;
		BOOST_CHECK( mmap == &main_frame.m_command_message_map) ;
	}

	BOOST_AUTO_TEST_CASE( Test_get_message_key )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;
		UINT key = main_frame.get_message_key( WM_CREATE, 7 ) ;
		BOOST_CHECK( key == WM_CREATE ) ;

		key = main_frame.get_message_key( UWM_USER_MESSAGE, 10 ) ;
		BOOST_CHECK( key == 10 ) ;

		key = main_frame.get_message_key( WM_COMMAND, MAKEWPARAM( 15, 20 ) ) ;
		BOOST_CHECK( key == 15 ) ;
	}

	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_no_display_state )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		main_frame.m_search_matches.set_matches( matches ) ;
		main_frame.m_trans_matches.set_matches( matches ) ;

		main_frame.on_register_gloss(message) ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "[Other display state]" ) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_register" ) ;

	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_NEW_RECORD_DISPLAY_STATE )
	{
		FelixModel model ;
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
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "[NEW_RECORD_DISPLAY_STATE]" ) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_register" ) ;


	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_MATCH_DISPLAY_STATE )
	{
		FelixModel model ;
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
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "[MATCH_DISPLAY_STATE]" ) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_register" ) ;


	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_CONCORDANCE_DISPLAY_STATE )
	{
		FelixModel model ;
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
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "[CONCORDANCE_DISPLAY_STATE]" ) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_register" ) ;


	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_INIT_DISPLAY_STATE )
	{
		FelixModel model ;
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
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "[Other display state]" ) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_register" ) ;


	}
BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// messages
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( TestCMainFrameMessages )

	BOOST_AUTO_TEST_CASE( on_create )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;
		LRESULT lResult = 1 ;
		main_frame.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_create" ) ;

		BOOST_CHECK( 0 == lResult ) ;
	}

	BOOST_AUTO_TEST_CASE( OnNavEdit )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnNavEdit(0) ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "OnNavEdit"); 
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_edit" ) ;
	}
	BOOST_AUTO_TEST_CASE( OnBeforeNavigateNavEdit )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnBeforeNavigate2( _bstr_t(L"100:edit:0") ) ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "OnNavEdit"); 
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[2], "on_user_edit" ) ;
	}

	BOOST_AUTO_TEST_CASE( OnNavDelete)
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;

		main_frame.OnNavDelete(0) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "OnNavDelete"); 
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_delete" ) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[2], "No Matches" ) ;
		BOOST_CHECK_EQUAL(3, (int)main_frame.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( TestOnNavAddToGloss )
	{
		FelixModel model ;
		CMainFrame main_frame( &model ) ;

		// Will throw because record is invalid.
		BOOST_CHECK_THROW(main_frame.OnNavAddToGloss(0), except::CException) ;
		BOOST_CHECK_EQUAL(2u, main_frame.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[0], "OnNavAddToGloss"); 
		BOOST_CHECK_EQUAL( main_frame.m_sensing_variable[1], "on_user_add_to_glossary" ) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif 


