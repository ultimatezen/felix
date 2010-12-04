#include "stdafx.h"
#include "view_state_match.h"
#include "view_state_test_utils.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( view_state_match_test )

	using namespace mem_engine ;

	search_match_ptr make_match(string source, string trans, int id=0, string name="new")
	{
		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		match->set_memory_id(id) ;
		match->set_memory_location(string2wstring(name)) ;

		return match ;
	}

	//////////////////////////////////////////////////////////////////////////
	// frame
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateMatchMain state ;
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
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(7, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_leave_edit_mode_match") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[3].find("<table class=") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4]), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[6]), "0") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( show_content_view )
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(5, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<div class=\"nomatch_query\">") != string::npos) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "ensure_document_complete") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4]), "0") ;

		BOOST_CHECK(vso.view.m_text.find(L"No Matches") != wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( show_content_window )
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		mem_engine::translation_match_query matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(5, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "is_single_page") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "is_short_format") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "check_mousewheel") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), "is_single_page") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), "set_bg_color_if_needed") ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_edit_record_model )
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;


		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(2u, vso.model.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[0], "get_memories") ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[1], "get_memory_by_id") ;
	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_replace)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(1u, vso.mem->size()) ;
		record_pointer first = *vso.mem->get_records().begin() ;
		string actual = string2string(first->get_source_rich()) ;
		BOOST_CHECK_EQUAL(string("source"), string(actual)) ;

		actual = string2string(vso.listener.item_under_edit->get_record()->get_source_rich()) ;
		BOOST_CHECK_EQUAL(string("source"), string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		BOOST_CHECK(! vso.listener.new_rec->is_valid_record()) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;
		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(6, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "get_item_under_edit") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "set_new_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "redo_lookup") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), "user_feedback") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), "425") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5]), "0") ;
		BOOST_CHECK(vso.listener.new_rec->is_valid_record()) ;
	}

	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match_non_empty)
	{
		ViewStateMatchMain state ;
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
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_empty)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr match = state.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}


	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = vso.mem->make_match() ;

		search_query_glossary trans_matches; 
		trans_matches.add_match(match) ;
		trans_matches.add_match(match) ;
		trans_matches.add_match(match) ;
		trans_matches.set_current(1u) ;
		state.set_search_matches(&trans_matches) ;

		BOOST_CHECK_EQUAL(1u, state.get_current()) ;
	}

	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match("record source", "record trans", vso.mem->get_id()) ;

		trans_match_container matches ;
		matches.insert(match) ;
		translation_match_query trans_matches; 
		trans_matches.set_matches(matches) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_edit_empty)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}
	// activate
	BOOST_AUTO_TEST_CASE( activate)
	{
		ViewStateMatchMain state ;
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

	// delete_match
	BOOST_AUTO_TEST_CASE( delete_match_empty)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		state.set_search_matches(&trans_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "user_feedback") ;
		string ids_no_matches(int2string(IDS_NO_MATCHES)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), ids_no_matches) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "0") ;
		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_out_of_range)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match("record source", "record trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		translation_match_query search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(1) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "user_feedback") ;
		string ids_out_of_range(int2string(IDS_OUT_OF_RANGE)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), ids_out_of_range) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "0") ;
		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_check_delete)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = false ;

		search_match_ptr match = make_match("source", "trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		translation_match_query search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "check_delete") ;
		BOOST_CHECK_EQUAL(1u, vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_empty)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		search_match_ptr match = make_match("source", "trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;
		translation_match_query search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "<center><h1>No Matches</h1></center>") ;
		BOOST_CHECK_EQUAL(2u, vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "user_feedback") ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_1)
	{
		ViewStateMatchMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		search_match_ptr m1 = make_match("source", "trans", vso.mem->get_id()) ;
		search_match_ptr m2 = make_match("source", "trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		translation_match_query search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
			
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<td class=\"match_content\" id=\"source\" style=\"color: #000000\">source</td>") != string::npos) ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<td class=\"match_content\" id=\"trans\" style=\"color: #000000\">trans</td>") != string::npos) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "user_feedback") ;
	}
BOOST_AUTO_TEST_SUITE_END()


	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( view_state_match_gloss_test )

	using namespace mem_engine ;

	search_match_ptr make_match(string source, string trans, int id=0)
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

	// handle_toggle_edit_mode
	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2u, vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_enter_edit_mode_new_record") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		search_query_glossary search_matches ;
		state.set_search_matches(&search_matches) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(6, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_leave_edit_mode_concordance_glossary") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3]), "<p>Found 0 matches.</p>") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5]), "0") ;

		BOOST_CHECK_EQUAL(2u, vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	// retrieve_edit_record
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_model )
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;
		vso.mem->add_record(rec) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[0], "get_memories") ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[1], "get_memory_by_id") ;
		BOOST_CHECK_EQUAL(2u, vso.model.m_sensing_variable.size()) ;

		string expected("source") ;
		string actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich())) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;
		vso.mem->add_record(rec) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(5, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "get_item_under_edit") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "set_new_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "user_feedback") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), "345") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), "0") ;
		BOOST_CHECK(vso.listener.new_rec->is_valid_record()) ;
	}

	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match_non_empty)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"record source") ;
		rec->set_trans(L"record trans") ;
		match->set_record(rec) ;
		match->set_values_to_record() ;

		translation_match_query trans_matches; 
		trans_matches.add_match(match) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr current_match = state.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_empty)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr match = state.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}


	BOOST_AUTO_TEST_CASE( get_current )
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = vso.mem->make_match() ;

		search_query_glossary trans_matches; 
		trans_matches.add_match(match) ;
		trans_matches.add_match(match) ;
		trans_matches.add_match(match) ;
		trans_matches.set_current(1u) ;
		state.set_search_matches(&trans_matches) ;

		BOOST_CHECK_EQUAL(1u, state.get_current()) ;
	}

	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match("record source", "record trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_glossary search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_edit_empty)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		wstring query(L"query") ;
		trans_matches.set_query_rich(query) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}

	// show_content
	BOOST_AUTO_TEST_CASE( show_content )
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		search_query_glossary search_matches; 
		state.set_search_matches(&search_matches) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "<p>Found 0 matches.</p>") ;
	}

	// delete_match
	BOOST_AUTO_TEST_CASE( delete_match_empty)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		search_query_glossary search_matches; 
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "user_feedback") ;
		string ids_no_matches(int2string(IDS_NO_MATCHES)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), ids_no_matches) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "0") ;
		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_out_of_range)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match("record source", "record trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_glossary search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(1) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "user_feedback") ;
		string ids_out_of_range(int2string(IDS_OUT_OF_RANGE)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), ids_out_of_range) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "0") ;
		BOOST_CHECK_EQUAL(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_check_delete)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = false ;

		search_match_ptr match = make_match("source", "trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_glossary search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "check_delete") ;
		BOOST_CHECK_EQUAL(1u, vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_empty)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		search_match_ptr match = make_match("source", "trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_glossary search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "<center><h1>No Matches</h1></center>") ;
		BOOST_CHECK_EQUAL(2u, vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "user_feedback") ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_1)
	{
		ViewStateMatchGloss state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;
		app_props::properties_glossary props ;
		state.set_app_props(&props) ;

		search_match_ptr m1 = make_match("source 1", "trans 1", vso.mem->get_id()) ;
		search_match_ptr m2 = make_match("source 2", "trans 2", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		search_query_glossary search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;

		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<td id=\"source\">source 2</td></tr>") != string::npos) ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<td id=\"trans\">trans 2</td></tr>") != string::npos) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "user_feedback") ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST
