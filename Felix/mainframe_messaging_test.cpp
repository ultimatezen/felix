/*!
	@brief Unit tests for CMainFrame class
	@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "MainFrm.h"
#include "record_local.h"
#include "felix_model_fake.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CMainFrameMessageTestCase )


	using namespace mem_engine;

	LRESULT FakeFreeFunction( WindowsMessage &message )
	{
		message ;
		return 0L ;
	}
	frame_ptr make_fake_frame()
	{
		app_props::props_ptr props(new app_props::properties); 
		model_iface_ptr the_model(new FelixModelInterfaceFake);
		return frame_ptr(new CMainFrame( the_model, props )) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Windows message handling
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( TestMessageMap )
	{
		frame_ptr frame = make_fake_frame() ;
		BOOST_CHECK_EQUAL( 4, (int)frame->m_message_map.size()) ;
	}
	BOOST_AUTO_TEST_CASE( TestMessageMapAddFunction )
	{
		frame_ptr frame = make_fake_frame() ;
		BOOST_CHECK_EQUAL( 4, (int)frame->m_message_map.size()) ;
		frame->register_event_listener( WM_CREATE, FakeFreeFunction ) ;
		BOOST_CHECK_EQUAL( 4, (int)frame->m_message_map.size()) ;
	}

	// on_create
	BOOST_AUTO_TEST_CASE( Teston_create )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;
		frame->on_create(message) ;

		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(frame->m_sensing_variable[0], "on_create" ) ;
	}
	BOOST_AUTO_TEST_CASE(starts_with_one_gloss_window)
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;
		frame->on_create(message) ;

		BOOST_CHECK_EQUAL(1u, frame->m_glossary_windows.size()) ;
	}

	// on_close
	BOOST_AUTO_TEST_CASE( Teston_close )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;
		frame->on_close(message) ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "on_close" ) ;
		BOOST_CHECK( !message.isHandled()) ;
	}

	BOOST_AUTO_TEST_CASE( Teston_destroy )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;
		frame->on_destroy(message) ;
		BOOST_CHECK_EQUAL(1u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "on_destroy" ) ;
		BOOST_CHECK( !message.isHandled()) ;
	}

	BOOST_AUTO_TEST_CASE( Test_get_message_map )
	{
		frame_ptr frame = make_fake_frame() ;
		CMainFrame::messageMapType *mmap = frame->get_message_map( WM_CREATE ) ;
		BOOST_CHECK( mmap == &frame->m_message_map) ;

		mmap = frame->get_message_map( UWM_USER_MESSAGE ) ;
		BOOST_CHECK( mmap == &frame->m_user_message_map) ;

		mmap = frame->get_message_map( WM_COMMAND ) ;
		BOOST_CHECK( mmap == &frame->m_command_message_map) ;
	}

	BOOST_AUTO_TEST_CASE( Test_get_message_key )
	{
		frame_ptr frame = make_fake_frame() ;
		UINT key = frame->get_message_key( WM_CREATE, 7 ) ;
		BOOST_CHECK( key == WM_CREATE ) ;

		key = frame->get_message_key( UWM_USER_MESSAGE, 10 ) ;
		BOOST_CHECK( key == 10 ) ;

		key = frame->get_message_key( WM_COMMAND, MAKEWPARAM( 15, 20 ) ) ;
		BOOST_CHECK( key == 15 ) ;
	}

	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_no_display_state )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		frame->m_search_matches.set_matches( matches ) ;
		frame->m_trans_matches.set_matches( matches ) ;

		frame->on_register_gloss(message) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "[Other display state]" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_register" ) ;

	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_NEW_RECORD_DISPLAY_STATE )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		frame->m_search_matches.set_matches( matches ) ;
		frame->m_trans_matches.set_matches( matches ) ;

		frame->set_display_state( CMainFrame::NEW_RECORD_DISPLAY_STATE ) ;
		frame->on_register_gloss(message) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "[NEW_RECORD_DISPLAY_STATE]" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_register" ) ;


	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_MATCH_DISPLAY_STATE )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		frame->m_search_matches.set_matches( matches ) ;
		frame->m_trans_matches.set_matches( matches ) ;

		frame->set_display_state( CMainFrame::MATCH_DISPLAY_STATE ) ;
		frame->on_register_gloss(message) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "[MATCH_DISPLAY_STATE]" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_register" ) ;


	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_CONCORDANCE_DISPLAY_STATE )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		frame->m_search_matches.set_matches( matches ) ;
		frame->m_trans_matches.set_matches( matches ) ;

		frame->set_display_state( CMainFrame::CONCORDANCE_DISPLAY_STATE ) ;
		frame->on_register_gloss(message) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "[CONCORDANCE_DISPLAY_STATE]" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_register" ) ;


	}
	BOOST_AUTO_TEST_CASE( Test_register_glossary_entries_INIT_DISPLAY_STATE )
	{
		frame_ptr frame = make_fake_frame() ;
		WindowsMessage message ;

		std::vector< mem_engine::search_match_ptr > matches ;
		mem_engine::search_match_ptr match(new search_match) ;
		match->set_record( record_pointer(new record_local()) ) ;
		matches += match ;
		frame->m_search_matches.set_matches( matches ) ;
		frame->m_trans_matches.set_matches( matches ) ;

		frame->set_display_state( CMainFrame::INIT_DISPLAY_STATE ) ;
		frame->on_register_gloss(message) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "[Other display state]" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_register" ) ;


	}
BOOST_AUTO_TEST_SUITE_END()

	//////////////////////////////////////////////////////////////////////////
	// messages
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( TestCMainFrameMessages )
	frame_ptr make_fake_frame()
	{
		app_props::props_ptr props(new app_props::properties); 
		model_iface_ptr the_model(new FelixModelInterfaceFake);
		return frame_ptr(new CMainFrame( the_model, props )) ;
	}
	BOOST_AUTO_TEST_CASE( on_create )
	{
		frame_ptr frame = make_fake_frame() ;
		LRESULT lResult = 1 ;
		frame->ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "Found message key"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_create" ) ;

		BOOST_CHECK( 0 == lResult ) ;
	}

	BOOST_AUTO_TEST_CASE( OnNavEdit )
	{
		frame_ptr frame = make_fake_frame() ;

		frame->OnNavEdit(0) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "OnNavEdit"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_edit" ) ;
	}
	BOOST_AUTO_TEST_CASE( OnBeforeNavigateNavEdit )
	{
		frame_ptr frame = make_fake_frame() ;

		frame->OnBeforeNavigate2( _bstr_t(L"100:edit:0") ) ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "OnNavEdit"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "on_user_edit" ) ;
	}

	BOOST_AUTO_TEST_CASE( OnNavDelete)
	{
		frame_ptr frame = make_fake_frame() ;

		frame->OnNavDelete(0) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "OnNavDelete"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_delete" ) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[2], "No Matches" ) ;
		BOOST_CHECK_EQUAL(3, (int)frame->m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( TestOnNavAddToGloss )
	{
		frame_ptr frame = make_fake_frame() ;

		// Will throw because record is invalid.
		BOOST_CHECK_THROW(frame->OnNavAddToGloss(0), except::CException) ;
		BOOST_CHECK_EQUAL(2u, frame->m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[0], "OnNavAddToGloss"); 
		BOOST_CHECK_EQUAL( frame->m_sensing_variable[1], "on_user_add_to_glossary" ) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif 


