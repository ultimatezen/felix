#include "stdafx.h"
#include "view_state_concordance.h"
#include "view_state_test_utils.h"
#include "record_local.h"
#include "query.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

	//////////////////////////////////////////////////////////////////////////
	// frame
	//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( view_state_concordance_test )

	using namespace mem_engine ;
	search_match_ptr make_match_con(string source, string trans, int id=0, string name="new")
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

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_enter_edit_mode_concordance") ;

		BOOST_CHECK_EQUAL(2, (int)vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true_view )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(4, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_leave_edit_mode_concordance") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3]), "<b>Search Results:</b><br />Found 0 matches.") ;
	}
	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true_listener )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2, (int)vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( show_content_0_matches_view )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(2, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("Found 0 matches.") != string::npos) ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_edit_record_model )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;
		vso.mem->add_record(rec) ;

		search_query_mainframe search_matches ;
		state.set_search_matches(&search_matches) ;

		record_pointer new_rec = record_pointer(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(3, (int)vso.model.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[0], "get_memories") ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[1], "get_memory_by_id") ;
		BOOST_CHECK_EQUAL(vso.model.m_sensing_variable[2], "get_memories") ;

		string expected("source") ;
		string actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich())) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"before") ;
		rec->set_trans(L"before") ;
		vso.mem->add_record(rec) ;

		// current match
		vso.listener.item_under_edit->set_record(rec) ;
		vso.listener.item_under_edit->set_memory_id(vso.mem->get_id()) ;
		vso.mem->add_record(rec) ;

		search_query_mainframe search_matches ;
		state.set_search_matches(&search_matches) ;

		record_pointer new_rec(new record_local) ;
		new_rec->set_source(L"source") ;
		new_rec->set_trans(L"trans") ;

		state.retrieve_edit_record(vso.mem->get_id(), new_rec) ;

		BOOST_CHECK_EQUAL(5, (int)vso.listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "get_item_under_edit") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "set_new_record") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "user_feedback") ;
		string expected_msgid(int2string(IDS_ADDED_TRANSLATION)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), expected_msgid) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), "0") ;
	}

	// get_current_match
	BOOST_AUTO_TEST_CASE( get_current_match_non_empty)
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe trans_matches; 
		trans_matches.set_matches(matches) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr current_match = state.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_empty)
	{
		ViewStateConcordanceMain state ;
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

	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit_non_empty)
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe trans_matches; 
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
		ViewStateConcordanceMain state ;
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
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		state.activate() ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "set_menu_checkmark") ;
		string id_view_match(int2string(ID_VIEW_MATCH)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), id_view_match) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[2]), "false") ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[3]), "set_menu_checkmark") ;
		string id_view_search(int2string(ID_VIEW_SEARCH)) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[4]), id_view_search) ;
		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[5]), "true") ;
		BOOST_CHECK_EQUAL(6, (int)vso.listener.m_sensing_variable.size()) ;
	}

	// delete_match
	BOOST_AUTO_TEST_CASE( delete_match_empty)
	{
		ViewStateConcordanceMain state ;
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
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe search_matches; 
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
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = false ;

		search_match_ptr match = make_match_con("source", "trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[0]), "check_delete") ;
		BOOST_CHECK_EQUAL(1, (int)vso.listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_empty)
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		search_match_ptr match = make_match_con("source", "trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "<center><h1>No Matches</h1></center>") ;
		BOOST_CHECK_EQUAL(2, (int)vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "user_feedback") ;
	}

	BOOST_AUTO_TEST_CASE( delete_match_to_1)
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.listener.m_should_delete = true ;

		search_match_ptr m1 = make_match_con("source", "trans", vso.mem->get_id()) ;
		search_match_ptr m2 = make_match_con("source", "trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(m1) ;
		matches.insert(m2) ;
		search_query_mainframe search_matches; 
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		state.delete_match(0) ;

		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "set_text") ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<td class=\"match_content\" id=\"source\">source</td>") != string::npos) ;
		BOOST_CHECK(vso.view.m_sensing_variable[1].find("<td class=\"match_content\" id=\"trans\">trans</td>") != string::npos) ;
		BOOST_CHECK_EQUAL(2, (int)vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(string(vso.listener.m_sensing_variable[1]), "user_feedback") ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// glossary
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( view_state_concordance_gloss_test )

	using namespace mem_engine ;

	search_match_ptr make_match_con(string source, string trans, int id=0, string name="new")
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

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_false )
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(2, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_enter_edit_mode_new_record") ;

		BOOST_CHECK_EQUAL(2, (int)vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( handle_toggle_edit_mode_true )
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		state.handle_toggle_edit_mode() ;

		BOOST_CHECK_EQUAL(6, (int)vso.view.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[0]), "is_edit_mode") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[1]), "handle_leave_edit_mode_new") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[2]), "set_text") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[3]), "<center><h1>Deleted entry.</h1></center>") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[4]), "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(string(vso.view.m_sensing_variable[5]), "0") ;

		BOOST_CHECK_EQUAL(2, (int)vso.listener.m_feedback_int.size()) ;
		BOOST_CHECK_EQUAL(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		BOOST_CHECK_EQUAL(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	BOOST_AUTO_TEST_CASE( retrieve_edit_record_model )
	{
		ViewStateConcordanceGloss state ;
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
		BOOST_CHECK_EQUAL(2, (int)vso.model.m_sensing_variable.size()) ;

		string expected("source") ;
		string actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich())) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

	}
	BOOST_AUTO_TEST_CASE( retrieve_edit_record_listener )
	{
		ViewStateConcordanceGloss state ;
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
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_glossary		search_matches ;
		search_matches.set_matches(matches) ;
		state.set_search_matches(&search_matches) ;

		search_match_ptr current_match = state.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_empty)
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		wstring query(L"query") ;
		search_query_glossary search_matches ;
		search_matches.set_query_rich(query) ;
		state.set_search_matches(&search_matches) ;

		search_match_ptr match = state.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}

	// on_user_edit
	BOOST_AUTO_TEST_CASE( on_user_edit_non_empty)
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;

		search_query_glossary search_matches ;
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
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		wstring query(L"query") ;
		search_query_glossary search_matches ;
		search_matches.set_query_rich(query) ;
		state.set_search_matches(&search_matches) ;

		state.on_user_edit() ;
		search_match_ptr match = vso.listener.item_under_edit ;

		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL("query", actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
		BOOST_CHECK_EQUAL(vso.mem->get_id(), match->get_memory_id()) ;
	}

	// show_content
	BOOST_AUTO_TEST_CASE( show_content_empty )
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		search_query_glossary search_matches ;
		state.set_search_matches(&search_matches) ;

		state.show_content() ;

		BOOST_CHECK_EQUAL(vso.view.m_sensing_variable[0], "set_text") ;
		BOOST_CHECK(boost::contains(vso.view.m_sensing_variable[1], "Found 0 matches.")) ;
		BOOST_CHECK_EQUAL(vso.view.m_sensing_variable[2], "set_scroll_pos") ;
		BOOST_CHECK_EQUAL(4u, vso.view.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL(vso.listener.m_sensing_variable[0], "user_feedback") ;
		BOOST_CHECK(boost::contains(vso.listener.m_sensing_variable[1], "Found 0 concordances for")) ;
		BOOST_CHECK_EQUAL(vso.listener.m_sensing_variable[2], "0") ;
		BOOST_CHECK_EQUAL(vso.listener.m_sensing_variable[3], "check_mousewheel") ;
		BOOST_CHECK_EQUAL(4u, vso.listener.m_sensing_variable.size()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef UNIT_TEST
