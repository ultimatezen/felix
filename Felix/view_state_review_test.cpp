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

		BOOST_CHECK_EQUAL(2, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_match") ;

		BOOST_CHECK_EQUAL(2, (int)vso.listener.m_feedback_int.size()) ;
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
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_match") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[3].find("<table class=") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5].c_str()), "0") ;

		BOOST_CHECK_EQUAL(2, (int)vso.listener.m_feedback_int.size()) ;
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
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<table class=") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "0") ;

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
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "get_review_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), "is_short_format") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "set_bg_color_if_needed") ;
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

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(2, (int)vso.model.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[0], "get_memories") ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[1], "get_memory_by_id") ;

		string expected("source") ;
		string actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich()).c_str()) ;
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

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(6, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), "set_review_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "redo_lookup") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3].c_str()), "user_feedback") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4].c_str()), "425") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5].c_str()), "0") ;
		BOOST_CHECK(vso.listener.review_rec->is_valid_record()) ;
	}

	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match_match_non_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"record source") ;
		rec->set_trans(L"record trans") ;
		match->set_record(rec) ;
		match->set_values_to_record() ;

		trans_match_container matches ;
		matches.insert(match) ;
		translation_match_query trans_matches; 
		trans_matches.set_matches(matches) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr current_match = state.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_concordance_match_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr match = state.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}
	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_review("record source", "record trans", vso.mem->get_id()) ;

		trans_match_container matches ;
		matches.insert(match) ;
		translation_match_query trans_matches; 
		trans_matches.set_matches(matches) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_edit_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich()).c_str()) ;

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
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "set_menu_checkmark") ;
		string id_view_match(int2string(ID_VIEW_MATCH).c_str()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), id_view_match) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "true") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3].c_str()), "set_menu_checkmark") ;
		string id_view_search(int2string(ID_VIEW_SEARCH).c_str()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4].c_str()), id_view_search) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5].c_str()), "false") ;
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
		vso.listener.review_rec = rec ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0].c_str()), "check_delete") ;
		BOOST_CHECK_EQUAL(1, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(1, (int)vso.mem->size()) ;
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
		vso.listener.review_rec = rec ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2].c_str()), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3].c_str()), "0") ;
		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1].c_str()), "get_review_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;

		BOOST_CHECK_EQUAL(0, (int)vso.mem->size()) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
