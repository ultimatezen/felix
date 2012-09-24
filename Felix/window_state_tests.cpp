#include "stdafx.h"
#include "view_state_initial.h"
#include "view_interface_fake.h"
#include "window_listener_fake.h"
#include "felix_model_fake.h"
#include "memory_local.h"
#include "record_local.h"
#include "mainfrm.h"
#include "GlossaryDialog.h"
#include "GlossaryDlgListenerFake.h"
#include "FelixApp.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( window_state_tests_mainframe )


	using namespace mem_engine ;
	search_match_ptr make_match_ws(string source, string trans, int id=0, string name="new")
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
	// set_display_state
	//////////////////////////////////////////////////////////////////////////

	// mainframe
	BOOST_AUTO_TEST_CASE( set_display_state_init)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::INIT_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(main_frame.m_view_state, &main_frame.m_view_state_initial) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_new)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::NEW_RECORD_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(main_frame.m_view_state, &main_frame.m_view_state_new) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_review)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::TRANS_REVIEW_STATE) ;
		BOOST_CHECK_EQUAL(main_frame.m_view_state, &main_frame.m_view_state_review) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_match)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(main_frame.m_view_state, &main_frame.m_view_state_match) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_concordance)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::CONCORDANCE_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(main_frame.m_view_state, &main_frame.m_view_state_concordance) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// get_current_match
	//////////////////////////////////////////////////////////////////////////

	// mainframe
	BOOST_AUTO_TEST_CASE( get_current_match_init)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;

		BOOST_CHECK_EQUAL((int)CCommonWindowFunctionality::INIT_DISPLAY_STATE, (int)main_frame.m_display_state) ;
		search_match_ptr match = main_frame.get_current_match() ;

		string expected("") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_new)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::NEW_RECORD_DISPLAY_STATE) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		main_frame.set_new_record(rec) ;

		search_match_ptr match = main_frame.get_current_match() ;

		string expected("source") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.001) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_match_empty)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;

		wstring query(L"query") ;
		main_frame.m_trans_matches.set_query_rich(query) ;

		search_match_ptr match = main_frame.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_match_non_empty)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;

		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"record source") ;
		rec->set_trans(L"record trans") ;
		match->set_record(rec) ;
		match->set_values_to_record() ;

		trans_match_container matches ;
		matches.insert(match) ;
		main_frame.m_trans_matches.set_matches(matches) ;

		search_match_ptr current_match = main_frame.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_concordance_match_empty)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::CONCORDANCE_DISPLAY_STATE) ;

		wstring query(L"query") ;
		main_frame.m_search_matches.set_query_rich(query) ;

		search_match_ptr match = main_frame.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// review mode
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( get_current_match_review_empty)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::TRANS_REVIEW_STATE) ;

		wstring query(L"query") ;
		main_frame.m_trans_matches.set_query_rich(query) ;

		search_match_ptr match = main_frame.get_current_match() ;

		string expected("") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_review_non_empty)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::TRANS_REVIEW_STATE) ;

		record_pointer newrec(new record_local) ;
		newrec->set_source(L"get_current_match_review_non_empty") ;

		main_frame.set_review_match(newrec, 0) ;

		search_match_ptr current_match = main_frame.get_current_match() ;

		string expected("get_current_match_review_non_empty") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}


	// concordance
	BOOST_AUTO_TEST_CASE( get_current_match_concordance)
	{
		CMainFrame main_frame(app::get_model_interface(), app::get_props()) ;
		main_frame.set_display_state(CCommonWindowFunctionality::CONCORDANCE_DISPLAY_STATE) ;

		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"record source") ;
		rec->set_trans(L"record trans") ;
		match->set_record(rec) ;
		match->set_values_to_record() ;

		search_match_container matches ;
		matches.insert(match) ;
		main_frame.m_search_matches.set_matches(matches) ;

		search_match_ptr current_match = main_frame.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = "record trans" ;
		actual = string2string(current_match->get_record()->get_trans_rich()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;

	}

BOOST_AUTO_TEST_SUITE_END()
	
BOOST_AUTO_TEST_SUITE( window_state_tests_glossary )


	using namespace mem_engine ;
	search_match_ptr make_match_ws(string source, string trans, int id=0, string name="new")
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

	// glossary
	BOOST_AUTO_TEST_CASE( set_display_state_init)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::INIT_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_view_state, &gloss_placer.m_view_state_initial) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_new)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::NEW_RECORD_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_view_state, &gloss_placer.m_view_state_new) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_match)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_view_state, &gloss_placer.m_view_state_match) ;
	}
	BOOST_AUTO_TEST_CASE( set_display_state_concordance)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::CONCORDANCE_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(gloss_placer.m_view_state, &gloss_placer.m_view_state_concordance) ;
	}



	//////////////////////////////////////////////////////////////////////////
	// get_current_match
	//////////////////////////////////////////////////////////////////////////

	// glossary
	BOOST_AUTO_TEST_CASE( get_current_match_init)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;

		BOOST_CHECK_EQUAL((int)CCommonWindowFunctionality::INIT_DISPLAY_STATE, (int)gloss_placer.m_display_state) ;
		search_match_ptr match = gloss_placer.get_current_match() ;

		string expected("") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_new)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::NEW_RECORD_DISPLAY_STATE) ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"source") ;
		gloss_placer.set_new_record(rec) ;

		search_match_ptr match = gloss_placer.get_current_match() ;

		string expected("source") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.001) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_match_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;

		wstring query(L"query") ;
		gloss_placer.m_search_matches.set_query_rich(query) ;

		search_match_ptr match = gloss_placer.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_current_match_match_non_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;

		search_match_ptr match(new search_match) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"record source") ;
		rec->set_trans(L"record trans") ;
		match->set_record(rec) ;
		match->set_values_to_record() ;

		search_match_container matches ;
		matches.insert(match) ;
		gloss_placer.m_search_matches.set_matches(matches) ;

		search_match_ptr current_match = gloss_placer.get_current_match() ;

		string expected("record source") ;
		string actual(string2string(current_match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	BOOST_AUTO_TEST_CASE( get_current_match_concordance_match_empty)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		gloss_placer.set_display_state(CCommonWindowFunctionality::CONCORDANCE_DISPLAY_STATE) ;

		wstring query(L"query") ;
		gloss_placer.m_concordance_matches.set_query_rich(query) ;

		search_match_ptr match = gloss_placer.get_current_match() ;

		string expected("query") ;
		string actual(string2string(match->get_record()->get_source_rich())) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_record()->get_trans_rich().empty()) ;
	}



	//////////////////////////////////////////////////////////////////////////
	// CGlossaryDialog::OnUserAdd
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( on_user_add_init)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CGlossaryWinListenerFake listener ;
		gloss_placer.set_listener(&listener) ;

		gloss_placer.set_display_state(CCommonWindowFunctionality::INIT_DISPLAY_STATE) ;

		gloss_placer.OnUserAdd(0) ;

		string actual(string2string(listener.added_rec->get_source_rich())) ;

		BOOST_CHECK_EQUAL("", actual) ;

		BOOST_CHECK_EQUAL("gloss_add_record", listener.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1u, listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_add_new)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CGlossaryWinListenerFake listener ;
		gloss_placer.set_listener(&listener) ;

		gloss_placer.set_display_state(CCommonWindowFunctionality::NEW_RECORD_DISPLAY_STATE) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"new state") ;
		rec->set_trans(L"new trans") ;
		rec->set_id(10) ;
		gloss_placer.set_new_record(rec) ;

		gloss_placer.OnUserAdd(0) ;

		string actual(string2string(listener.added_rec->get_source_rich())) ;

		BOOST_CHECK_EQUAL("new state", actual) ;

		BOOST_CHECK_EQUAL("gloss_add_record", listener.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1u, listener.m_sensing_variable.size()) ;

		BOOST_CHECK_EQUAL((int)rec->get_id(), (int)gloss_placer.get_new_record()->get_id()) ;
	}


	BOOST_AUTO_TEST_CASE( on_user_add_match)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CGlossaryWinListenerFake listener ;
		gloss_placer.set_listener(&listener) ;

		search_match_ptr m0 = make_match_ws("match 0", "match 0") ;
		search_match_ptr m1 = make_match_ws("match 1", "match 1") ;
		search_match_ptr m2 = make_match_ws("match 2", "match 2") ;

		felix_query::match_list matches ;
		matches.push_back(m0) ;
		matches.push_back(m1) ;
		matches.push_back(m2) ;

		gloss_placer.m_search_matches.set_matches(matches) ;

		gloss_placer.set_display_state(CCommonWindowFunctionality::MATCH_DISPLAY_STATE) ;

		gloss_placer.OnUserAdd(1) ;

		string actual(string2string(listener.added_rec->get_source_rich())) ;

		BOOST_CHECK_EQUAL("match 1", actual) ;

		BOOST_CHECK_EQUAL("gloss_add_record", listener.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1u, listener.m_sensing_variable.size()) ;
	}

	BOOST_AUTO_TEST_CASE( on_user_add_concordance)
	{
		app_props::props_ptr props(new app_props::properties) ;
		CGlossaryDialog gloss_placer(props) ;
		CGlossaryWinListenerFake listener ;
		gloss_placer.set_listener(&listener) ;

		search_match_ptr m0 = make_match_ws("match 0", "match 0") ;
		search_match_ptr m1 = make_match_ws("match 1", "match 1") ;
		search_match_ptr m2 = make_match_ws("match 2", "match 2") ;

		felix_query::match_list matches ;
		matches.push_back(m0) ;
		matches.push_back(m1) ;
		matches.push_back(m2) ;

		gloss_placer.m_concordance_matches.set_matches(matches) ;

		gloss_placer.set_display_state(CCommonWindowFunctionality::CONCORDANCE_DISPLAY_STATE) ;

		gloss_placer.OnUserAdd(2) ;

		string actual(string2string(listener.added_rec->get_source_rich())) ;

		BOOST_CHECK_EQUAL("match 2", actual) ;

		BOOST_CHECK_EQUAL("gloss_add_record", listener.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1u, listener.m_sensing_variable.size()) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef UNIT_TEST