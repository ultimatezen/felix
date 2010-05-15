#include "StdAfx.h"
#include "SearchWindow.h"
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestSearchWindow_perform_search )


	using namespace mem_engine ;

	// retrieve_input_value
	BOOST_AUTO_TEST_CASE(foo)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;
		element->set_attribute(L"value", L"foo") ;
		// should retrieve value and clear
		BOOST_CHECK(retrieve_input_value(element) == L"foo") ;
		BOOST_CHECK(element->get_attribute(L"value") == L"") ;
	}

	// generating HTML
	BOOST_AUTO_TEST_CASE(searchbox_emptied)
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

		BOOST_CHECK_EQUAL(searchbox->get_attribute(L"value"), L"") ;
		BOOST_CHECK_EQUAL(1u, window.m_search_runner.get_terms().size()) ;
		BOOST_CHECK_EQUAL(window.m_search_runner.get_terms()[0], L"foo") ;
	}
	BOOST_AUTO_TEST_CASE(filterbox_text)
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
		BOOST_CHECK(text.find(L"spambot") == wstring::npos) ;
		BOOST_CHECK(text.find(L"<h3>Filters</h3>") != wstring::npos) ;
		BOOST_CHECK(text.find(L"foo (<a href=\"/0/deletefilter\" style=\"font-size:80%\">Delete</a>)<br />") != wstring::npos) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestSearchWindow_browser )
	// set_filterbox_text
	BOOST_AUTO_TEST_CASE( set_filterbox_text)
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

		BOOST_CHECK(text.find(L"spambot") == wstring::npos) ;
		BOOST_CHECK(text.find(L"<h3>Filters</h3>") != wstring::npos) ;
		BOOST_CHECK(text.find(L"foo (<a href=\"/0/deletefilter\" style=\"font-size:80%\">Delete</a>)<br />") != wstring::npos) ;
		BOOST_CHECK(text.find(L"bar (<a href=\"/1/deletefilter\" style=\"font-size:80%\">Delete</a>)<br />") != wstring::npos) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif