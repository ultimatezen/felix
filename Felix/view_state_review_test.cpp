#include "stdafx.h"
#include "view_state_review.h"
#include "record_local.h"
#include "query.h"
#include "easyunit/testharness.h"
#include "view_state_test_utils.h"

#ifdef UNIT_TEST

namespace easyunit
{
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

	TEST( view_state_review_test, handle_toggle_edit_mode_false )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_match") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_review_test, handle_toggle_edit_mode_true )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(6, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_match") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		ASSERT_TRUE(vso.view.m_sensing_variable[3].find("<table class=") != string::npos) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[5].c_str()), "0") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_review_test, show_content_view )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;
	
		ASSERT_EQUALS_V(4, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		ASSERT_TRUE(vso.view.m_sensing_variable[1].find("<table class=") != string::npos) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "0") ;

		ASSERT_TRUE(vso.view.m_text.find(L"Review Translation") != wstring::npos) ;
	}
	TEST( view_state_review_test, show_content_window )
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;

		ASSERT_EQUALS_V(3, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "get_review_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "is_short_format") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "set_bg_color_if_needed") ;
	}
	TEST( view_state_review_test, retrieve_edit_record_model )
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

		ASSERT_EQUALS_V(2, (int)vso.model.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[0].c_str()), "get_memories") ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[1].c_str()), "get_memory_by_id") ;

		SimpleString expected("source") ;
		SimpleString actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

	}
	TEST( view_state_review_test, retrieve_edit_record_listener )
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

		ASSERT_EQUALS_V(6, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "set_review_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "redo_lookup") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), "user_feedback") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), "425") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[5].c_str()), "0") ;
		ASSERT_TRUE(vso.listener.review_rec->is_valid_record()) ;
	}

	// get_current_match
	TEST( view_state_review_test, get_current_match_match_non_empty)
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

		SimpleString expected("record source") ;
		SimpleString actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( view_state_review_test, get_current_match_concordance_match_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr match = state.get_current_match() ;

		SimpleString expected("query") ;
		SimpleString actual(string2string(match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_TRUE(match->get_record()->get_trans_rich().empty()) ;
	}
	// on_user_edit
	TEST( view_state_review_test, on_user_edit)
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

		SimpleString expected("record source") ;
		SimpleString actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	TEST( view_state_review_test, on_user_edit_empty)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		SimpleString expected("query") ;
		SimpleString actual(string2string(match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_TRUE(match->get_record()->get_trans_rich().empty()) ;
		ASSERT_EQUALS_V(vso.mem->get_id(), match->get_memory_id()) ;
	}

	// activate
	TEST( view_state_review_test, activate)
	{
		ViewStateReview state ;
		view_state_obj vso(&state) ;

		state.activate() ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "set_menu_checkmark") ;
		SimpleString id_view_match(int2string(ID_VIEW_MATCH).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), id_view_match) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "true") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), "set_menu_checkmark") ;
		SimpleString id_view_search(int2string(ID_VIEW_SEARCH).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), id_view_search) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[5].c_str()), "false") ;
		ASSERT_EQUALS_V(6, (int)vso.listener.m_sensing_variable.size()) ;
	}


	// delete
	TEST( view_state_review_test, delete_match_check_delete)
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

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "check_delete") ;
		ASSERT_EQUALS_V(1, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(1, (int)vso.mem->size()) ;
	}

	TEST( view_state_review_test, delete_match_to_empty)
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

		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "0") ;
		ASSERT_EQUALS_V(4, (int)vso.view.m_sensing_variable.size()) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "get_review_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;

		ASSERT_EQUALS_V(0, (int)vso.mem->size()) ;
	}
}


#endif // #ifdef UNIT_TEST