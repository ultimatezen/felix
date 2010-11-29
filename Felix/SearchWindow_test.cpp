#include "StdAfx.h"
#include "SearchWindow.h"
#include "element_wrapper_fake.h"
#include "record_local.h"
#include "memory_local.h"
#include "action_delete_entry.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

#include "document_wrapper_fake.h"
#include "felix_model_fake.h"
#include "frame_listener_fake.h"

FelixModelInterfaceFake sw_model ;

BOOST_AUTO_TEST_SUITE( TestCSearchWindow )

	using namespace mem_engine ;
	using namespace action;

	memory_pointer add_controller(CSearchWindow &window, CString name1, CString name2)
	{
		sw_model.clear() ;
		memory_pointer mem1(new memory_local) ;
		mem1->set_location(name1) ;
		memory_pointer mem2(new memory_local) ;
		mem2->set_location(name2) ;

		sw_model.m_model->insert_memory(mem1) ;
		sw_model.m_model->insert_memory(mem2) ;

		window.set_mem_controller(&sw_model) ;

		return mem1 ;
	}

	record_pointer add_record(memory_pointer &mem, string source, string trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		mem->add_record(rec) ;
		return rec ;
	}

	// get_pos_arg
	BOOST_AUTO_TEST_CASE( get_pos_arg_back_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo\\bar\\1\\deletefilter" ;
		BOOST_CHECK_EQUAL(1u, window.get_pos_arg(url)) ;
	}
	BOOST_AUTO_TEST_CASE( get_pos_arg_forward_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/1/deletefilter" ;
		BOOST_CHECK_EQUAL(1u, window.get_pos_arg(url)) ;
	}
	BOOST_AUTO_TEST_CASE( get_pos_arg_forward_15)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/15/deletefilter" ;
		BOOST_CHECK_EQUAL(15, (int)window.get_pos_arg(url)) ;
	}



	// url navigation
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(1u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_deleterecord)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		search_match_ptr match = mem->make_match() ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		window.m_matches.push_back(match) ;

		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_deleterecord"); 
	}


	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_undodelete)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec = add_record(mem, "foo", "bar") ;
		window.m_undo = undo_action_ptr(new DeleteEntryAction(mem, rec)) ;

		_bstr_t url = L"/undodelete" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(3, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_undodelete"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "CSearchWindow::retrieve_and_show_matches"); 
	}

	
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_foo)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo" ;
		BOOST_CHECK(window.OnBeforeNavigate2(url)) ;
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_deleterecord)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;

		search_match_ptr match = mem->make_match() ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		window.m_matches.push_back(match) ;

		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(0u, mem->size()) ;
	}

	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_deleterecord_page_stays_same)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;

		search_match_ptr match = mem->make_match() ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		for (int i = 0 ; i < 230 ; ++i)
		{
			window.m_matches.push_back(match) ;
		}
		window.m_current_match = 100 ;
		window.m_paginator.set_num_records(window.m_matches.size()) ;
		window.m_paginator.goto_page(10) ;
		BOOST_CHECK_EQUAL(10u, window.m_paginator.get_current_page()) ;

		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(10u, window.m_paginator.get_current_page()) ;
	}


	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_undodelete)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec = add_record(mem, "foo", "bar") ;
		window.m_undo = undo_action_ptr(new DeleteEntryAction(mem, rec)) ;
		mem->erase(rec) ;

		_bstr_t url = L"/undodelete" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		BOOST_CHECK_EQUAL(L"foo", mem->get_record_at(0)->get_source_rich()) ;
	}

	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_pound)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo#" ;
		BOOST_CHECK(! window.OnBeforeNavigate2(url)) ;
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_save_results)
	{
		FrameListenerFake listener ;
		CSearchWindow window(&listener) ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/save_results" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "save_results"); 

		BOOST_CHECK_EQUAL(1u, listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(listener.m_sensing_variable[0], "save_memory_as"); 

	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_delete_results)
	{
		FrameListenerFake listener ;
		CSearchWindow window(&listener) ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/delete_results" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(3u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "delete_results"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "CSearchWindow::show_search_page"); 
	}
	//////////////////////////////////////////////////////////////////////////
	// Sense url nav for replace window
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_gotoreplace)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/gotoreplace" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(3, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_gotoreplace"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "CSearchWindow::set_filterbox_text"); 
	}

	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_replace_find)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/replace_find" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_replace_find"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_replace_replace)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/replace_replace" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_replace_replace"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_replace_all)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/replace_all" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(1u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
	}


	// handlers
	BOOST_AUTO_TEST_CASE( get_search_matches)
	{
		typedef std::vector<mem_engine::search_match_ptr> match_vec ;

		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_search_runner.add_term(L"foo") ;

		match_vec matches ;
		window.get_search_matches(matches) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		search_match_ptr match = matches[0] ;
		BOOST_CHECK_EQUAL(L"foo", match->get_record()->get_source_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( handle_deleterecord)
	{
		typedef std::vector<mem_engine::search_match_ptr> match_vec ;

		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_search_runner.add_term(L"foo") ;

		window.get_search_matches(window.m_matches) ;
		BOOST_CHECK_EQUAL(1u, window.m_matches.size()) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;

		wstring url = L"/0/deleterecord" ;
		window.handle_deleterecord(window.get_doc3(), url) ;

		match_vec matches ;
		window.get_search_matches(matches) ;

		BOOST_CHECK_EQUAL(0u, matches.size()) ;
		BOOST_CHECK_EQUAL(0u, mem->size()) ;
	}

	BOOST_AUTO_TEST_CASE( handle_editrecord_page_stays_same)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;

		search_match_ptr match = mem->make_match() ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		for (int i = 0 ; i < 130 ; ++i)
		{
			window.m_matches.push_back(match) ;
		}
		window.m_current_match = 50 ;
		window.m_paginator.set_num_records(window.m_matches.size()) ;
		window.m_paginator.goto_page(5) ;
		BOOST_CHECK_EQUAL(5u, window.m_paginator.get_current_page()) ;

		window.m_search_runner.add_term(L"foo") ;
		wstring url = L"/0/editrecord" ;
		window.handle_editrecord(window.get_doc3(), url) ;
		BOOST_CHECK_EQUAL(5u, window.m_paginator.get_current_page()) ;
	}


	BOOST_AUTO_TEST_CASE( delete_record)
	{
		typedef std::vector<mem_engine::search_match_ptr> match_vec ;

		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_search_runner.add_term(L"foo") ;

		match_vec matches_before ;
		window.get_search_matches(matches_before) ;
		BOOST_CHECK_EQUAL(1u, matches_before.size()) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;

		window.delete_record(matches_before[0]) ;

		match_vec matches ;
		window.get_search_matches(matches) ;

		BOOST_CHECK_EQUAL(0u, matches.size()) ;
		BOOST_CHECK_EQUAL(0u, mem->size()) ;
	}




	// message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_CREATE)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "OnCreate"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_DESTROY)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( window.m_sensing_variable[0], "OnDestroy"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_SIZE)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_SIZE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "OnSize"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_perform_replace )

	using namespace mem_engine ;
		// perform_replace
	BOOST_AUTO_TEST_CASE( source_spam_to_ham)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"spam") ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		// replace stuff
		element_wrapper_ptr replacefrom_box(new element_wrapper_fake()) ;
		replacefrom_box->set_attribute(L"value", L"source:spam") ;
		element_wrapper_ptr replaceto_box(new element_wrapper_fake()) ;
		replaceto_box->set_attribute(L"value", L"ham") ;

		wrapper->add_element(L"replacefrom", replacefrom_box) ;
		wrapper->add_element(L"replaceto", replaceto_box) ;

		CSearchWindow window ;
		window.perform_replace(doc3_wrapper_ptr(wrapper), rec) ;

		BOOST_CHECK_EQUAL(L"ham", rec->get_source_rich()) ;
		BOOST_CHECK_EQUAL(L"spam", rec->get_trans_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( trans_spam_to_ham)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"spam") ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		// replace stuff
		element_wrapper_ptr replacefrom_box(new element_wrapper_fake()) ;
		replacefrom_box->set_attribute(L"value", L"trans:spam") ;
		element_wrapper_ptr replaceto_box(new element_wrapper_fake()) ;
		replaceto_box->set_attribute(L"value", L"ham") ;

		wrapper->add_element(L"replacefrom", replacefrom_box) ;
		wrapper->add_element(L"replaceto", replaceto_box) ;

		CSearchWindow window ;
		window.perform_replace(doc3_wrapper_ptr(wrapper), rec) ;

		BOOST_CHECK_EQUAL(L"spam", rec->get_source_rich()) ;
		BOOST_CHECK_EQUAL(L"ham", rec->get_trans_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( both_spam_to_ham)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"spam") ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		// replace stuff
		element_wrapper_ptr replacefrom_box(new element_wrapper_fake()) ;
		replacefrom_box->set_attribute(L"value", L"spam") ;
		element_wrapper_ptr replaceto_box(new element_wrapper_fake()) ;
		replaceto_box->set_attribute(L"value", L"ham") ;

		wrapper->add_element(L"replacefrom", replacefrom_box) ;
		wrapper->add_element(L"replaceto", replaceto_box) ;

		CSearchWindow window ;
		window.perform_replace(doc3_wrapper_ptr(wrapper), rec) ;

		BOOST_CHECK_EQUAL(L"ham", rec->get_source_rich()) ;
		BOOST_CHECK_EQUAL(L"ham", rec->get_trans_rich()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// handle_replace_all
//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE( test_handle_replace_all )

	using namespace mem_engine;


	memory_pointer add_controller(CSearchWindow &window, CString name1, CString name2)
	{
		sw_model.clear() ;
		memory_pointer mem1(new memory_local) ;
		mem1->set_location(name1) ;
		memory_pointer mem2(new memory_local) ;
		mem2->set_location(name2) ;

		sw_model.m_model->insert_memory(mem1) ;
		sw_model.m_model->insert_memory(mem2) ;

		window.set_mem_controller(&sw_model) ;

		return mem1 ;
	}

	BOOST_AUTO_TEST_CASE(result_none)
	{
		// replace stuff
		element_wrapper_ptr filterbox(new element_wrapper_fake()) ;
		element_wrapper_ptr replacelinks(new element_wrapper_fake()) ;
		element_wrapper_ptr searchresults(new element_wrapper_fake()) ;

		element_wrapper_ptr replacefrom(new element_wrapper_fake()) ;
		element_wrapper_ptr replaceto(new element_wrapper_fake()) ;

		replacefrom->set_attribute(L"value", L"spam") ;
		replaceto->set_attribute(L"value", L"ham") ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		wrapper->add_element(L"filterbox", filterbox) ;
		wrapper->add_element(L"replacelinks", replacelinks) ;
		wrapper->add_element(L"searchresults", searchresults) ;
		wrapper->add_element(L"replacefrom", replacefrom) ;
		wrapper->add_element(L"replaceto", replaceto) ;


		CSearchWindow window ;

		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		wstring text = L"{$result}" ;
		window.handle_replace_all(doc3_wrapper_ptr(wrapper), text, L"") ;

		wstring expected = L"None" ;

		BOOST_CHECK_EQUAL(searchresults->get_inner_text(), expected) ;
		BOOST_CHECK_EQUAL(filterbox->get_inner_text(), L"") ;

	}

BOOST_AUTO_TEST_SUITE_END()

#endif