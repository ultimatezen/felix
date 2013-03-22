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


BOOST_AUTO_TEST_SUITE( TestCSearchWindow )

	using namespace mem_engine ;
	using namespace action;
	using namespace app_props ;

	memory_pointer add_controller(CSearchWindow &window, CString name1, CString name2)
	{
		model_iface_ptr model(new FelixModelInterfaceFake) ;
		app_props::properties_memory props; 
		memory_pointer mem1 = model->add_memory() ;
		memory_pointer mem2 = model->add_memory() ;
		mem1->set_location(name1) ;
		mem2->set_location(name2) ;

		window.set_mem_controller(model) ;

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
	BOOST_AUTO_TEST_CASE(get_pos_arg_back_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo\\bar\\1\\deletefilter" ;
		BOOST_CHECK_EQUAL(1u, window.get_pos_arg(url)) ;
	}
	BOOST_AUTO_TEST_CASE(get_pos_arg_forward_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/1/deletefilter" ;
		BOOST_CHECK_EQUAL(1u, window.get_pos_arg(url)) ;
	}
	BOOST_AUTO_TEST_CASE(get_pos_arg_forward_15)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/15/deletefilter" ;
		BOOST_CHECK_EQUAL(15, (int)window.get_pos_arg(url)) ;
	}

	// get_replace_matches_mem
	BOOST_AUTO_TEST_CASE(get_replace_matches_mem_0)
	{
		CSearchWindow window ;
		memory_pointer mem(new memory_local(get_props())) ;
		add_record(mem, "aaa", "bbb") ;
		wstring replace_from = L"source:xxx" ;
		search_match_container matches ;
		window.get_replace_matches_mem(mem, replace_from, matches) ;
		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_replace_matches_mem_1)
	{
		CSearchWindow window ;
		memory_pointer mem(new memory_local(get_props())) ;
		add_record(mem, "aaa", "bbb") ;
		add_record(mem, "xxx", "yyy") ;
		wstring replace_from = L"source:xxx" ;
		search_match_container matches ;
		window.get_replace_matches_mem(mem, replace_from, matches) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}


	// url navigation
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(1u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
	}
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_deleterecord)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		search_match_ptr match = mem->make_match(rec) ;
		match->set_values_to_record() ;
		window.m_matches.push_back(match) ;

		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2u, window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_deleterecord"); 
	}


	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_undodelete)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec = add_record(mem, "foo", "bar") ;
		window.m_undo = undo_action_ptr(new ActionDeleteEntry(mem, rec)) ;

		_bstr_t url = L"/undodelete" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(3, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(window.m_sensing_variable[0], "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[1], "handle_undodelete"); 
		BOOST_CHECK_EQUAL(window.m_sensing_variable[2], "CSearchWindow::retrieve_and_show_matches"); 
	}

	
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_foo)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo" ;
		BOOST_CHECK(window.OnBeforeNavigate2(url)) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_deleterecord)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;

		search_match_ptr match = mem->make_match(rec) ;
		match->set_values_to_record() ;
		window.m_matches.push_back(match) ;

		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(0u, mem->size()) ;
	}

	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_deleterecord_page_stays_same)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;

		search_match_ptr match = mem->make_match(rec) ;
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


	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_undodelete)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec = add_record(mem, "foo", "bar") ;
		window.m_undo = undo_action_ptr(new ActionDeleteEntry(mem, rec)) ;
		mem->erase(rec) ;

		_bstr_t url = L"/undodelete" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		BOOST_CHECK_EQUAL(L"foo", mem->get_record_at(0)->get_source_rich()) ;
	}

	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_pound)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo#" ;
		BOOST_CHECK(! window.OnBeforeNavigate2(url)) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_save_results)
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

	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_delete_results)
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
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_delete_results_message)
	{
		FrameListenerFake listener ;
		CSearchWindow window(&listener) ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/delete_results" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(window.m_message, L"Performed action [Delete Matches]. <a href=\"/start/undo\">Undo</a>"); 
	}
	//////////////////////////////////////////////////////////////////////////
	// Sense url nav for replace window
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_gotoreplace)
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

	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_replace_find)
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
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_replace_replace)
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
	BOOST_AUTO_TEST_CASE(test_OnBeforeNavigate2_sense_replace_all)
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
	BOOST_AUTO_TEST_CASE(get_search_matches)
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
	BOOST_AUTO_TEST_CASE(handle_deleterecord)
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

	BOOST_AUTO_TEST_CASE(handle_editrecord_page_stays_same)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;

		search_match_ptr match = mem->make_match(rec) ;
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


	BOOST_AUTO_TEST_CASE(delete_record)
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
	BOOST_AUTO_TEST_CASE(delete_record_message)
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

		BOOST_CHECK_EQUAL(window.m_message, L"Deleted record (<a href=\"/undodelete\">Undo</a>)"); 
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_perform_replace )

	using namespace mem_engine ;
		// perform_replace
	BOOST_AUTO_TEST_CASE(source_spam_to_ham)
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
	BOOST_AUTO_TEST_CASE(trans_spam_to_ham)
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
	BOOST_AUTO_TEST_CASE(both_spam_to_ham)
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
		model_iface_ptr model(new FelixModelInterfaceFake) ;
		app_props::properties_memory props; 
		memory_pointer mem1 = model->add_memory() ;
		memory_pointer mem2 = model->add_memory() ;
		mem1->set_location(name1) ;
		mem2->set_location(name2) ;

		window.set_mem_controller(model) ;

		return mem1 ;
	}

	doc3_wrapper_ptr make_doc(wstring from, wstring to)
	{
		element_wrapper_ptr filterbox(new element_wrapper_fake()) ;
		element_wrapper_ptr replacelinks(new element_wrapper_fake()) ;
		element_wrapper_ptr searchresults(new element_wrapper_fake()) ;

		element_wrapper_ptr replacefrom(new element_wrapper_fake()) ;
		element_wrapper_ptr replaceto(new element_wrapper_fake()) ;

		replacefrom->set_attribute(L"value",from) ;
		replaceto->set_attribute(L"value", to) ;

		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		wrapper->add_element(L"filterbox", filterbox) ;
		wrapper->add_element(L"replacelinks", replacelinks) ;
		wrapper->add_element(L"searchresults", searchresults) ;
		wrapper->add_element(L"replacefrom", replacefrom) ;
		wrapper->add_element(L"replaceto", replaceto) ;

		return doc3_wrapper_ptr(wrapper) ;
	}

	record_pointer add_rec(memory_pointer mem, wstring source, wstring trans)
	{
		record_pointer rec(new record_local) ;
		rec->set_source(source) ;
		rec->set_trans(trans) ;
		mem->add_record(rec) ;
		return rec ;
	}

	BOOST_AUTO_TEST_CASE(result_empty_mems)
	{
		auto wrapped = make_doc(L"spam", L"ham") ;
		CSearchWindow window ;

		memory_pointer mem = add_controller(window, L"foo", L"bar") ;

		wstring text = L"{$result}" ;
		window.handle_replace_all(wrapped, text, L"") ;

		wstring expected = L"None" ;

		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"searchresults")->get_inner_text(), expected) ;
		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"filterbox")->get_inner_text(), L"") ;
	}
	BOOST_AUTO_TEST_CASE(result_none_found)
	{
		auto wrapped = make_doc(L"aaa", L"bbb") ;
		CSearchWindow window ;

		memory_pointer mem = add_controller(window, L"mem1", L"mem2") ;
		add_rec(mem, L"xxx", L"xxx") ;

		wstring text = L"{$result}" ;
		window.handle_replace_all(wrapped, text, L"") ;

		wstring expected = L"None" ;

		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"searchresults")->get_inner_text(), expected) ;
		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"filterbox")->get_inner_text(), L"") ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
	}
	BOOST_AUTO_TEST_CASE(one_match_one_record)
	{
		auto wrapped = make_doc(L"xxx", L"yyy") ;
		CSearchWindow window ;

		memory_pointer mem = add_controller(window, L"mem1", L"mem2") ;
		add_rec(mem, L"xxx", L"xxx") ;

		wstring text = L"{$result}" ;
		window.handle_replace_all(wrapped, text, L"") ;

		wstring expected = L"None" ;

		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"searchresults")->get_inner_text(), expected) ;
		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"filterbox")->get_inner_text(), L"") ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		record_pointer rec = mem->get_record_at(0) ;
		BOOST_CHECK_EQUAL(rec->get_source_plain(), wstring(L"yyy")) ;
		BOOST_CHECK_EQUAL(rec->get_trans_rich(), wstring(L"yyy")) ;
	}
	BOOST_AUTO_TEST_CASE(one_match_one_record_created)
	{
		auto wrapped = make_doc(L"created:", L"1990/11/12 07:10") ;
		CSearchWindow window ;

		memory_pointer mem = add_controller(window, L"mem1", L"mem2") ;
		add_rec(mem, L"xxx", L"xxx") ;

		wstring text = L"{$result}" ;
		window.handle_replace_all(wrapped, text, L"") ;

		wstring expected = L"None" ;

		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"searchresults")->get_inner_text(), expected) ;
		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"filterbox")->get_inner_text(), L"") ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		record_pointer rec = mem->get_record_at(0) ;
		BOOST_CHECK_EQUAL(rec->get_created().wYear, 1990) ;
	}
	BOOST_AUTO_TEST_CASE(created_with_filter)
	{
		auto wrapped = make_doc(L"created:", L"2000/11/12 07:10") ;
		CSearchWindow window ;
		window.m_search_runner.add_term(L"xxx") ;

		memory_pointer mem = add_controller(window, L"mem1", L"mem2") ;
		add_rec(mem, L"xxx", L"xxx") ;

		wstring text = L"{$result}" ;
		window.handle_replace_all(wrapped, text, L"") ;

		wstring expected = L"None" ;

		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"searchresults")->get_inner_text(), expected) ;
		BOOST_CHECK_EQUAL(wrapped->get_element_by_id(L"filterbox")->get_inner_text(), L"") ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
		record_pointer rec = mem->get_record_at(0) ;
		BOOST_CHECK_EQUAL(rec->get_created().wYear, 2000) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif