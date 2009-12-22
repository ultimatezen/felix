#include "StdAfx.h"
#include "SearchWindow.h"
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#include "easyunit/testharness.h"
#include "record_local.h"
#include "memory_local.h"

#ifdef UNIT_TEST

namespace easyunit
{
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
	// ends_with
	TEST(test_ends_with, foobar_bar)
	{
		wstring target = L"foobar" ;
		ASSERT_TRUE(ends_with(target, L"bar")) ;
	}
	TEST(test_ends_with, foobarf_bar)
	{
		wstring target = L"foobarf" ;
		ASSERT_TRUE(! ends_with(target, L"bar")) ;
	}

	// get_pos_arg
	TEST(TestCSearchWindow, get_pos_arg_back_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo\\bar\\1\\deletefilter" ;
		ASSERT_EQUALS_V(1, (int)window.get_pos_arg(url)) ;
	}
	TEST(TestCSearchWindow, get_pos_arg_forward_1)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/1/deletefilter" ;
		ASSERT_EQUALS_V(1, (int)window.get_pos_arg(url)) ;
	}
	TEST(TestCSearchWindow, get_pos_arg_forward_15)
	{
		CSearchWindow window ;
		wstring url = L"c:\\foo/bar/15/deletefilter" ;
		ASSERT_EQUALS_V(15, (int)window.get_pos_arg(url)) ;
	}

	// perform_replace
	TEST(test_perform_replace, source_spam_to_ham)
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

		ASSERT_EQUALS(L"ham", rec->get_source_rich()) ;
		ASSERT_EQUALS(L"spam", rec->get_trans_rich()) ;
	}
	TEST(test_perform_replace, trans_spam_to_ham)
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

		ASSERT_EQUALS(L"spam", rec->get_source_rich()) ;
		ASSERT_EQUALS(L"ham", rec->get_trans_rich()) ;
	}
	TEST(test_perform_replace, both_spam_to_ham)
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

		ASSERT_EQUALS(L"ham", rec->get_source_rich()) ;
		ASSERT_EQUALS(L"ham", rec->get_trans_rich()) ;
	}

	// url navigation
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo" ;
		window.OnBeforeNavigate2(url) ;
		ASSERT_EQUALS_V(1, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
	}
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense_deleterecord)
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
		ASSERT_EQUALS_V(3, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[1].c_str()), "handle_deleterecord"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[2].c_str()), "CSearchWindow::retrieve_and_show_matches"); 
	}
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense_undodelete)
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
		ASSERT_EQUALS_V(3, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[1].c_str()), "handle_undodelete"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[2].c_str()), "CSearchWindow::retrieve_and_show_matches"); 
	}

	
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_foo)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo" ;
		ASSERT_TRUE(window.OnBeforeNavigate2(url)) ;
	}
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_deleterecord)
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
		ASSERT_EQUALS(0, (int)mem->size()) ;
		ASSERT_EQUALS(L"foo", window.m_deleted_match->get_record()->get_source_rich()) ;
	}
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_undodelete)
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
		ASSERT_EQUALS(1, (int)mem->size()) ;
		ASSERT_EQUALS(L"foo", mem->get_record_at(0)->get_source_rich()) ;
	}

	TEST(TestCSearchWindow, test_OnBeforeNavigate2_pound)
	{
		CSearchWindow window ;
		_bstr_t url = L"foo#" ;
		ASSERT_TRUE(! window.OnBeforeNavigate2(url)) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Sense url nav for replace window
	//////////////////////////////////////////////////////////////////////////
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense_gotoreplace)
	{
		CSearchWindow window ;
		add_controller(window, L"foo", L"bar") ;

		_bstr_t url = L"/gotoreplace" ;
		window.OnBeforeNavigate2(url) ;
		ASSERT_EQUALS_V(3, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[1].c_str()), "handle_gotoreplace"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[2].c_str()), "CSearchWindow::set_filterbox_text"); 
	}

	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense_replace_find)
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
		ASSERT_EQUALS_V(2, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[1].c_str()), "handle_replace_find"); 
	}
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense_replace_replace)
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
		ASSERT_EQUALS_V(2, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[1].c_str()), "handle_replace_replace"); 
	}
	TEST(TestCSearchWindow, test_OnBeforeNavigate2_sense_replace_all)
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
		ASSERT_EQUALS_V(2, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "CSearchWindow::OnBeforeNavigate2"); 
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[1].c_str()), "handle_replace_all"); 
	}


	// handlers
	TEST(TestCSearchWindow, get_search_matches)
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
		ASSERT_EQUALS(1, (int)matches.size()) ;

		search_match_ptr match = matches[0] ;
		ASSERT_EQUALS(L"foo", match->get_record()->get_source_rich()) ;
	}
	TEST(TestCSearchWindow, handle_deleterecord)
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
		ASSERT_EQUALS(1, (int)matches_before.size()) ;
		ASSERT_EQUALS(1, (int)mem->size()) ;

		wstring url = L"/0/deleterecord" ;
		window.handle_deleterecord(window.get_doc3(), url) ;

		match_vec matches ;
		window.get_search_matches(matches) ;

		ASSERT_EQUALS(0, (int)matches.size()) ;
		ASSERT_EQUALS(0, (int)mem->size()) ;
		ASSERT_EQUALS(L"foo", window.m_deleted_match->get_record()->get_source_rich()) ;
	}
	TEST(TestCSearchWindow, delete_record)
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
		ASSERT_EQUALS(1, (int)matches_before.size()) ;
		ASSERT_EQUALS(1, (int)mem->size()) ;

		window.delete_record(matches_before[0]) ;

		match_vec matches ;
		window.get_search_matches(matches) ;

		ASSERT_EQUALS(0, (int)matches.size()) ;
		ASSERT_EQUALS(0, (int)mem->size()) ;
	}




	// message processing
	TEST(TestCSearchWindow, test_message_WM_CREATE)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_CREATE, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "OnCreate"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST(TestCSearchWindow, test_message_WM_DESTROY)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(window.m_sensing_variable[0].c_str()), "OnDestroy"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST(TestCSearchWindow, test_message_WM_SIZE)
	{
		CSearchWindow window ;
		LRESULT lResult = 1 ;
		window.ProcessWindowMessage(NULL, WM_SIZE, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)window.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(window.m_sensing_variable[0].c_str()), "OnSize"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}

}

#endif