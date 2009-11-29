#include "StdAfx.h"
#include "SearchWindow.h"
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_engine ;

	// retrieve_input_value
	TEST(test_retrieve_input_value, foo)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;
		element->set_attribute(L"value", L"foo") ;
		// should retrieve value and clear
		ASSERT_TRUE(retrieve_input_value(element) == L"foo") ;
		ASSERT_TRUE(element->get_attribute(L"value") == L"") ;
	}

	// generating HTML
	TEST(TestSearchWindow_perform_search, searchbox_emptied)
	{
		element_wrapper_ptr searchbox(new element_wrapper_fake()) ;
		searchbox->set_attribute(L"value", L"foo") ;
		element_wrapper_ptr filterbox(new element_wrapper_fake()) ;
		element_wrapper_ptr searchresults(new element_wrapper_fake()) ;
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		wrapper->add_element(L"searchbox", searchbox) ;
		wrapper->add_element(L"filterbox", filterbox) ;
		wrapper->add_element(L"searchresults", searchresults) ;
		doc3_wrapper_ptr doc(wrapper) ;

		CSearchWindow window ;
		boost::shared_ptr<memory_model> controller(new memory_model_mem) ;
		window.set_mem_controller(controller) ;
		window.perform_search(doc) ;

		ASSERT_EQUALS(searchbox->get_attribute(L"value"), L"") ;
		ASSERT_EQUALS_V(1, (int)window.m_search_runner.get_terms().size()) ;
		ASSERT_EQUALS(window.m_search_runner.get_terms()[0], L"foo") ;
	}
	TEST(TestSearchWindow_perform_search, filterbox_text)
	{
		element_wrapper_ptr searchbox(new element_wrapper_fake()) ;
		searchbox->set_attribute(L"value", L"foo") ;
		element_wrapper_ptr filterbox(new element_wrapper_fake()) ;
		element_wrapper_ptr searchresults(new element_wrapper_fake()) ;
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		wrapper->add_element(L"searchbox", searchbox) ;
		wrapper->add_element(L"filterbox", filterbox) ;
		wrapper->add_element(L"searchresults", searchresults) ;
		doc3_wrapper_ptr doc(wrapper) ;

		CSearchWindow window ;
		boost::shared_ptr<memory_model> controller(new memory_model_mem) ;
		window.set_mem_controller(controller) ;
		window.perform_search(doc) ;
		wstring text = filterbox->get_inner_text() ;
		ASSERT_TRUE(text.find(L"spambot") == wstring::npos) ;
		ASSERT_TRUE(text.find(L"<h3>Filters</h3>") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"foo (<a href=\"/0/deletefilter\" style=\"font-size:80%\">Delete</a>)<br />") != wstring::npos) ;
	}

	// set_filterbox_text
	TEST(TestSearchWindow_browser, set_filterbox_text)
	{
		element_wrapper_ptr filterbox(new element_wrapper_fake()) ;
		doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
		wrapper->add_element(L"filterbox", filterbox) ;
		doc3_wrapper_ptr doc(wrapper) ;

		std::vector<wstring> terms ;
		terms.push_back(L"foo") ;
		terms.push_back(L"bar") ;

		CSearchWindow window ;
		window.set_filterbox_text(doc, terms) ;
		wstring text = filterbox->get_inner_text() ;

		ASSERT_TRUE(text.find(L"spambot") == wstring::npos) ;
		ASSERT_TRUE(text.find(L"<h3>Filters</h3>") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"foo (<a href=\"/0/deletefilter\" style=\"font-size:80%\">Delete</a>)<br />") != wstring::npos) ;
		ASSERT_TRUE(text.find(L"bar (<a href=\"/1/deletefilter\" style=\"font-size:80%\">Delete</a>)<br />") != wstring::npos) ;
	}

}

#endif