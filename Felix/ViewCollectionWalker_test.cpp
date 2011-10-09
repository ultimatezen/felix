#include "StdAfx.h"
#include "ViewCollectionWalker.h"
#include "element_wrapper_fake.h"
#include "record_local.h"
#include "memory_local.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestViewCollectionWalker )

	using namespace mem_engine;

	BOOST_AUTO_TEST_CASE( is_other_tag_false)
	{
		BOOST_CHECK(! is_other_tag(L"ref_count", tag_name_holder::instance())) ;
		BOOST_CHECK(! is_other_tag(L"date_created", tag_name_holder::instance())) ;
		BOOST_CHECK(! is_other_tag(L"modified", tag_name_holder::instance())) ;
		BOOST_CHECK(! is_other_tag(L"memory", tag_name_holder::instance())) ;
		BOOST_CHECK(! is_other_tag(L"rich_query", tag_name_holder::instance())) ;
		BOOST_CHECK(! is_other_tag(L"rich_source", tag_name_holder::instance())) ;
	}
	BOOST_AUTO_TEST_CASE( EraseCurrentMatch)
	{
		// memories
		boost::shared_ptr<mem_engine::memory_model> memories(new mem_engine::memory_model_mem(app_props::get_props())) ;

		// memory
		mem_engine::memory_pointer mem = memories->create_memory() ;
		memories->insert_memory(mem) ;
		mem_engine::record_pointer rec(new mem_engine::record_local) ;
		rec->set_source(L"source") ;
		rec->set_trans(L"trans") ;
		mem->add_record(rec) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;

		// match
		search_match_ptr match(new search_match) ;
		match->set_record(rec) ;
		match->set_memory_id(mem->get_id()) ;

		// matches
		mem_engine::search_query_mainframe matches ;
		search_match_container match_con ;
		match_con.insert(match) ;
		matches.set_matches(match_con) ;
		wstring id = L"0" ;

		CViewCollectionWalker walker ;
		walker.EraseCurrentRecord(match, &matches, id, memories) ;

		// record is deleted
		BOOST_CHECK_EQUAL(0u, mem->size()) ;

	}
	// reliability
	BOOST_AUTO_TEST_CASE( set_reliability_0)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"0") ;
		record_pointer rec(new record_local()) ;
		rec->set_reliability(5u) ;

		CViewCollectionWalker walker ;
		walker.SetReliabilityFromElement( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK_EQUAL(0u, rec->get_reliability()) ;

	}
	BOOST_AUTO_TEST_CASE( set_reliability_9)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"9") ;
		record_pointer rec(new record_local()) ;
		BOOST_CHECK_EQUAL(0u, rec->get_reliability()) ;

		CViewCollectionWalker walker ;
		walker.SetReliabilityFromElement( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK_EQUAL(9, (int)rec->get_reliability()) ;

	}
	BOOST_AUTO_TEST_CASE( reliability_throws_on_spam)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"spam") ;
		record_pointer rec(new record_local()) ;
		BOOST_CHECK_EQUAL(0u, rec->get_reliability()) ;

		try
		{
			CViewCollectionWalker walker ;
			walker.SetReliabilityFromElement( element_wrapper_ptr(wrapper), rec ) ;
			BOOST_FAIL("Should have thrown on value of 'spam'") ;
		}
		catch (std::exception&)
		{
			BOOST_CHECK_EQUAL(0u, rec->get_reliability()) ;
		}
	}
	// validated
	BOOST_AUTO_TEST_CASE( set_validated_on_yes)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"yes") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.SetValidatedFromElement( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( set_validated_on_true)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"TRUE") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.SetValidatedFromElement( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( set_validated_off)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"spam") ;
		record_pointer rec(new record_local()) ;
		rec->set_validated_off() ;

		CViewCollectionWalker walker ;
		walker.SetValidatedFromElement( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(! rec->is_validated()) ;
	}

	// set_item
	BOOST_AUTO_TEST_CASE( SetItemFromElement_td)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_tag(L"td") ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;
		BOOST_CHECK(rec->get_item(L"spam").empty()) ;

		CViewCollectionWalker walker ;
		walker.SetItemFromElement( element_wrapper_ptr(wrapper), rec, L"spam" ) ;

		CStringA actual = rec->get_item(L"spam").c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( SetItemFromElement_other)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_tag(L"p") ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.SetItemFromElement( element_wrapper_ptr(wrapper), rec, L"spam" ) ;

		BOOST_CHECK(rec->get_item(L"spam").empty()) ;
	}
	// element to record
	BOOST_AUTO_TEST_CASE( ElementToRecord_empty_id)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(rec->get_item(L"spam").empty()) ;
		BOOST_CHECK(rec->get_source_rich().empty()) ;
		BOOST_CHECK(rec->get_trans_rich().empty()) ;
		BOOST_CHECK(rec->get_context_rich().empty()) ;
		BOOST_CHECK(! rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_source)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"source" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_source_rich().c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_trans)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"trans" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(rec->get_source_rich().empty()) ;
		CStringA actual = rec->get_trans_rich().c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_context)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"context" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_context_rich().c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_reliability_5)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"reliability" ;
		wrapper->set_inner_text(L"5") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK_EQUAL(5, (int)rec->get_reliability()) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_validated)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"validated" ;
		wrapper->set_inner_text(L"true") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(rec->is_validated()) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_dummy_trans)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"dummy_trans" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		BOOST_CHECK(rec->get_source_rich().empty()) ;
		CStringA actual = rec->get_trans_rich().c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_dummy_source)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"dummy_source" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_source_rich().c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( ElementToRecord_spam)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"spam" ;
		wrapper->set_tag(L"td") ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_item(L"spam").c_str() ;
		string expected = "egg" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	// adding records

	// "/foo.html" -> "file:///C:/foo.html"
	// "bar.html" -> "file:///C:/Users/RyanVista/AppData/Local/Felix/html/en/bar.html"
	// doc_url = "file://C:\Users\RyanVista\AppData\Local\Felix\html\en\start.html"
	BOOST_AUTO_TEST_CASE( RepairLinkUrl_foo)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;

		wrapper->set_attribute(L"HREF", L"file:///C:/foo.html") ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.RepairLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		string expected = "/foo.html" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( RepairLinkUrl_bar)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;

		wrapper->set_attribute(L"HREF", L"file:///C:/Users/RyanVista/AppData/Local/Felix/html/en/bar.html") ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.RepairLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		string expected = "bar.html" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( RepairLinkUrl_ginstrom)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;

		wrapper->set_attribute(L"HREF", L"http://ginstrom.com/") ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.RepairLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		string expected = "http://ginstrom.com/" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( CheckLinkUrl_ginstrom)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_tag(L"a") ;
		wrapper->set_attribute(L"HREF", L"http://ginstrom.com/") ;
		element_wrapper_ptr element(wrapper) ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.CheckLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		string expected = "http://ginstrom.com/" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif