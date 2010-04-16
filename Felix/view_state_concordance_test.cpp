#include "stdafx.h"
#include "view_state_concordance.h"
#include "view_state_test_utils.h"
#include "record_local.h"
#include "query.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	search_match_ptr make_match_con(string source, string trans, int id=0)
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

	TEST( view_state_concordance_test, handle_toggle_edit_mode_false )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_concordance") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_concordance_test, handle_toggle_edit_mode_true_view )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(4, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_concordance") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "<b>Search Results:</b><br />Found 0 matches.") ;
	}
	TEST( view_state_concordance_test, handle_toggle_edit_mode_true_listener )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_concordance_test, show_content_0_matches_view )
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		mem_engine::search_query_mainframe matches ;
		state.set_search_matches(&matches) ;

		state.show_content() ;

		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		ASSERT_TRUE(vso.view.m_sensing_variable[1].find("Found 0 matches.") != string::npos) ;
	}

	TEST( view_state_concordance_test, retrieve_edit_record_model )
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

		ASSERT_EQUALS_V(3, (int)vso.model.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[0].c_str()), "get_memories") ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[1].c_str()), "get_memory_by_id") ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[2].c_str()), "get_memories") ;

		SimpleString expected("source") ;
		SimpleString actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( view_state_concordance_test, retrieve_edit_record_listener )
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

		ASSERT_EQUALS_V(5, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;
		SimpleString expected_msgid(int2string(IDS_ADDED_TRANSLATION).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), expected_msgid) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), "0") ;
	}

	// get_current_match
	TEST( view_state_concordance_test, get_current_match_non_empty)
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

		SimpleString expected("record source") ;
		SimpleString actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( view_state_concordance_test, get_current_match_empty)
	{
		ViewStateConcordanceMain state ;
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
	TEST( view_state_concordance_test, on_user_edit_non_empty)
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

		SimpleString expected("record source") ;
		SimpleString actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	TEST( view_state_concordance_test, on_user_edit_empty)
	{
		ViewStateConcordanceMain state ;
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
	TEST( view_state_concordance_test, activate)
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		state.activate() ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "set_menu_checkmark") ;
		SimpleString id_view_match(int2string(ID_VIEW_MATCH).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), id_view_match) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "false") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), "set_menu_checkmark") ;
		SimpleString id_view_search(int2string(ID_VIEW_SEARCH).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), id_view_search) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[5].c_str()), "true") ;
		ASSERT_EQUALS_V(6, (int)vso.listener.m_sensing_variable.size()) ;
	}

	// delete_match
	TEST( view_state_concordance_test, delete_match_empty)
	{
		ViewStateConcordanceMain state ;
		view_state_obj vso(&state) ;

		translation_match_query trans_matches; 
		state.set_search_matches(&trans_matches) ;

		state.delete_match(0) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "user_feedback") ;
		SimpleString ids_no_matches(int2string(IDS_NO_MATCHES).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), ids_no_matches) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "0") ;
		ASSERT_EQUALS_V(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

	TEST( view_state_concordance_test, delete_match_out_of_range)
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

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "user_feedback") ;
		SimpleString ids_out_of_range(int2string(IDS_OUT_OF_RANGE).c_str()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), ids_out_of_range) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "0") ;
		ASSERT_EQUALS_V(3, (int)vso.listener.m_sensing_variable.size()) ;
	}

	TEST( view_state_concordance_test, delete_match_check_delete)
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

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "check_delete") ;
		ASSERT_EQUALS_V(1, (int)vso.listener.m_sensing_variable.size()) ;
	}

	TEST( view_state_concordance_test, delete_match_to_empty)
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

		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "<center><h1>No Matches</h1></center>") ;
		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "user_feedback") ;
	}

	TEST( view_state_concordance_test, delete_match_to_1)
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

		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "set_text") ;
		ASSERT_TRUE(vso.view.m_sensing_variable[1].find("<td class=\"match_content\" id=\"source\">source</td>") != string::npos) ;
		ASSERT_TRUE(vso.view.m_sensing_variable[1].find("<td class=\"match_content\" id=\"trans\">trans</td>") != string::npos) ;
		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;

		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "user_feedback") ;
	}

	//////////////////////////////////////////////////////////////////////////
	// glossary
	//////////////////////////////////////////////////////////////////////////

	TEST( view_state_concordance_gloss_test, handle_toggle_edit_mode_false )
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(2, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_enter_edit_mode_new_record") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_ENTERING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_IN_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_concordance_gloss_test, handle_toggle_edit_mode_true )
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;
		vso.view.m_is_edit_mode = true ;

		state.handle_toggle_edit_mode() ;

		ASSERT_EQUALS_V(6, (int)vso.view.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[0].c_str()), "is_edit_mode") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[1].c_str()), "handle_leave_edit_mode_new") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[2].c_str()), "set_text") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[3].c_str()), "<center><h1>Deleted entry.</h1></center>") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[4].c_str()), "set_scroll_pos") ;
		ASSERT_EQUALS_V(SimpleString(vso.view.m_sensing_variable[5].c_str()), "0") ;

		ASSERT_EQUALS_V(2, (int)vso.listener.m_feedback_int.size()) ;
		ASSERT_EQUALS_V(IDS_LEAVING_EDIT_MODE, vso.listener.m_feedback_int[0]) ;
		ASSERT_EQUALS_V(IDS_LEFT_EDIT_MODE, vso.listener.m_feedback_int[1]) ;
	}

	TEST( view_state_concordance_gloss_test, retrieve_edit_record_model )
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

		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[0].c_str()), "get_memories") ;
		ASSERT_EQUALS_V(SimpleString(vso.model.m_sensing_variable[1].c_str()), "get_memory_by_id") ;
		ASSERT_EQUALS_V(2, (int)vso.model.m_sensing_variable.size()) ;

		SimpleString expected("source") ;
		SimpleString actual(string2string(vso.listener.item_under_edit->get_record()->get_source_rich()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

	}
	TEST( view_state_concordance_gloss_test, retrieve_edit_record_listener )
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

		ASSERT_EQUALS_V(5, (int)vso.listener.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[0].c_str()), "get_item_under_edit") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[1].c_str()), "set_new_record") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[2].c_str()), "user_feedback") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[3].c_str()), "345") ;
		ASSERT_EQUALS_V(SimpleString(vso.listener.m_sensing_variable[4].c_str()), "0") ;
		ASSERT_TRUE(vso.listener.new_rec->is_valid_record()) ;
	}

	// get_current_match
	TEST( view_state_concordance_gloss_test, get_current_match_non_empty)
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans") ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe trans_matches; 
		trans_matches.set_matches(matches) ;
		state.set_search_matches(&trans_matches) ;

		search_match_ptr current_match = state.get_current_match() ;

		SimpleString expected("record source") ;
		SimpleString actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

	TEST( view_state_concordance_gloss_test, get_current_match_empty)
	{
		ViewStateConcordanceGloss state ;
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
	TEST( view_state_concordance_gloss_test, on_user_edit_non_empty)
	{
		ViewStateConcordanceGloss state ;
		view_state_obj vso(&state) ;

		search_match_ptr match = make_match_con("record source", "record trans", vso.mem->get_id()) ;

		search_match_container matches ;
		matches.insert(match) ;
		search_query_mainframe trans_matches; 
		trans_matches.set_matches(matches) ;
		state.set_search_matches(&trans_matches) ;

		state.on_user_edit() ;
		search_match_ptr current_match = vso.listener.item_under_edit ;

		SimpleString expected("record source") ;
		SimpleString actual(string2string(current_match->get_record()->get_source_rich()).c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS_V(vso.mem->get_id(), current_match->get_memory_id()) ;
	}

	TEST( view_state_concordance_gloss_test, on_user_edit_empty)
	{
		ViewStateConcordanceGloss state ;
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

	// show_content
	TEST( view_state_concordance_gloss_test, show_content )
	{
		view_interface_fake view ;
		ASSERT_TRUE(! view.is_edit_mode()) ;
		WindowListenerFake listener; 

		ViewStateConcordanceGloss state ;
		state.set_view(&view) ;
		state.set_window_listener(&listener) ;
		state.show_content() ;

	}

}


#endif // #ifdef UNIT_TEST