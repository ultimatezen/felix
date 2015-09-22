#include "stdafx.h"
#include "view_state_review.h"
#include "record_local.h"
#include "query.h"
#include "view_state_test_utils.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( view_state_review_test )

	using namespace mem_engine ;

	search_match_ptr make_match_review(string source, string trans, int id=0)
	{
		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		match->set_memory_id(id) ;

		return match ;
	}

	//////////////////////////////////////////////////////////////////////////
	// frame
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2u, vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_enter_edit_mode_match") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(6, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_leave_edit_mode_match") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[3].find("<table class=") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5]), "0") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( show_content_view )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;
	
		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<table class=") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3]), "0") ;

		BOOST_CHECK(vso.view.m_text.find(L"Review Translation") != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( show_content_window )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "get_review_match") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "is_short_format") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "set_bg_color_if_needed") ;
	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_model )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;
		vso.mem->add_record(rec) ;

		record_pointer new_rec = record_pointer(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec, false) ;


		string expected("source") ;
		string actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich())) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;
		vso.mem->add_record(rec) ;

		record_pointer new_rec = record_pointer(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec, false) ;

		BOOST_CHECK_EQUAL(6, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "get_item_under_edit") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "set_review_match") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "redo_lookup") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), "user_feedback") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), "425") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5]), "0") ;
		BOOST_CHECK(vso.listener.review_match->get_record()->is_valid_record()) ;
	}

	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match_match_non_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"review_state") ;

		vso.listener.set_review_match(newrec, 0) ;

		search_match_ptr current_match = state.get_current_match() ;

		string expected("review_state") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_concordance_match_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = state.get_current_match() ;

		string expected ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}


	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;
		BOOST_CHECK_EQUAL(0u, state.get_current()) ;
	}

	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"on_user_edit") ;

		vso.listener.set_review_match(newrec, 0) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;

		string expected("on_user_edit") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_edit_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query(query) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string expected("") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}

	// activate
	BOOST_AUTO_TEST_CASE( activate)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		state.activate() ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "set_menu_checkmark") ;
		string id_view_match(int2string(ID_VIEW_MATCH)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), id_view_match) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "true") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), "set_menu_checkmark") ;
		string id_view_search(int2string(ID_VIEW_SEARCH)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), id_view_search) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5]), "false") ;
		BOOST_CHECK_EQUAL(6, (int)vso.listener.m_sensing_variable.size()) ;
	}


	// delete
	BOOST_AUTO_TEST_CASE( delete_match_check_delete)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = false ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;

		vso.mem->add_record(rec) ;
		vso.listener.review_match->set_record(rec) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "check_delete") ;
		BOOST_CHECK_EQUAL(1u, vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0u, vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(1u, vso.mem->size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		record_pointer rec = record_pointer(new record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;

		vso.mem->add_record(rec) ;
		vso.listener.review_match->set_record(rec) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "<center><h1>Deleted entry.</h1></center>") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3]), "0") ;
		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "get_review_match") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "user_feedback") ;

		BOOST_CHECK_EQUAL(0u, vso.mem->size()) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
