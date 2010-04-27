#include "StdAfx.h"
#include "SearchWindow.h"
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#include "record_local.h"
#include "memory_local.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCSearchWindow )

	using namespace mem_engine ;

	memory_pointer add_controller(CSearchWindow &window, CString name1, CString name2)
	{
		boost::shared_ptr<memory_model> model(new memory_model_mem) ;
		memory_pointer mem1(new memory_local) ;
		mem1->set_location(name1) ;
		memory_pointer mem2(new memory_local) ;
		mem2->set_location(name2) ;

		model->insert_memory(mem1) ;
		model->insert_memory(mem2) ;

		window.set_mem_controller(model) ;

		return mem1 ;
	}

	// get_pos_arg
	BOOST_AUTO_TEST_CASE( get_pos_arg_back_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo\\bar\\1\\deletefilter" ;
		BOOST_CHECK_EQUAL(1, (int)window.get_pos_arg(url)) ;
	}
	BOOST_AUTO_TEST_CASE( get_pos_arg_forward_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/1/deletefilter" ;
		BOOST_CHECK_EQUAL(1, (int)window.get_pos_arg(url)) ;
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
		BOOST_CHECK_EQUAL(1, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_deleterecord)
	{
		CSearchWindow window ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(3, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[1].c_str()), "handle_deleterecord"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[2].c_str()), "CSearchWindow::retrieve_and_show_matches"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_undodelete)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		window.m_deleted_match = search_match_ptr(new search_match);
		window.m_deleted_match->set_memory_id(mem->get_id()) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_deleted_match->set_record(rec) ;

		_bstr_t url = L"/undodelete" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(3, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[1].c_str()), "handle_undodelete"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[2].c_str()), "CSearchWindow::retrieve_and_show_matches"); 
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
		window.m_search_runner.add_term(L"foo") ;
		_bstr_t url = L"/0/deleterecord" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(0, (int)mem->size()) ;
		BOOST_CHECK_EQUAL(L"foo", window.m_deleted_match->get_record()->get_source_rich()) ;
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_undodelete)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		window.m_deleted_match = search_match_ptr(new search_match);
		window.m_deleted_match->set_memory_id(mem->get_id()) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_deleted_match->set_record(rec) ;

		_bstr_t url = L"/undodelete" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(1, (int)mem->size()) ;
		BOOST_CHECK_EQUAL(L"foo", mem->get_record_at(0)->get_source_rich()) ;
	}

	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_pound)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo#" ;
		BOOST_CHECK(! window.OnBeforeNavigate2(url)) ;
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
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[1].c_str()), "handle_gotoreplace"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[2].c_str()), "CSearchWindow::set_filterbox_text"); 
	}

	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_replace_find)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		window.m_deleted_match = search_match_ptr(new search_match);
		window.m_deleted_match->set_memory_id(mem->get_id()) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_deleted_match->set_record(rec) ;

		_bstr_t url = L"/replace_find" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[1].c_str()), "handle_replace_find"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_replace_replace)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		window.m_deleted_match = search_match_ptr(new search_match);
		window.m_deleted_match->set_memory_id(mem->get_id()) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_deleted_match->set_record(rec) ;

		_bstr_t url = L"/replace_replace" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[1].c_str()), "handle_replace_replace"); 
	}
	BOOST_AUTO_TEST_CASE( test_OnBeforeNavigate2_sense_replace_all)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;
		memory_pointer mem = add_controller(window, L"foo", L"bar") ;
		window.m_deleted_match = search_match_ptr(new search_match);
		window.m_deleted_match->set_memory_id(mem->get_id()) ;
		record_pointer rec(new record_local) ;
		rec->set_source(L"foo") ;
		rec->set_trans(L"bar") ;
		mem->add_record(rec) ;
		window.m_deleted_match->set_record(rec) ;

		_bstr_t url = L"/replace_all" ;
		window.OnBeforeNavigate2(url) ;
		BOOST_CHECK_EQUAL(2, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[1].c_str()), "handle_replace_all"); 
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
		BOOST_CHECK_EQUAL(1, (int)matches.size()) ;

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

		match_vec matches_before ;
		window.get_search_matches(matches_before) ;
		BOOST_CHECK_EQUAL(1, (int)matches_before.size()) ;
		BOOST_CHECK_EQUAL(1, (int)mem->size()) ;

		wstring url = L"/0/deleterecord" ;
		window.handle_deleterecord(window.get_doc3(), url) ;

		match_vec matches ;
		window.get_search_matches(matches) ;

		BOOST_CHECK_EQUAL(0, (int)matches.size()) ;
		BOOST_CHECK_EQUAL(0, (int)mem->size()) ;
		BOOST_CHECK_EQUAL(L"foo", window.m_deleted_match->get_record()->get_source_rich()) ;
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
		BOOST_CHECK_EQUAL(1, (int)matches_before.size()) ;
		BOOST_CHECK_EQUAL(1, (int)mem->size()) ;

		window.delete_record(matches_before[0]) ;

		match_vec matches ;
		window.get_search_matches(matches) ;

		BOOST_CHECK_EQUAL(0, (int)matches.size()) ;
		BOOST_CHECK_EQUAL(0, (int)mem->size()) ;
	}




	// message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_CREATE)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "OnCreate"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_DESTROY)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(window.m_sensing_variable[0].c_str()), "OnDestroy"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_SIZE)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_SIZE, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)window.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(window.m_sensing_variable[0].c_str()), "OnSize"); 
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

#endif