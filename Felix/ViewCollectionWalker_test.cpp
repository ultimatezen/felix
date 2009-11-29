#include "StdAfx.h"
#include "ViewCollectionWalker.h"
#include "element_wrapper_fake.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	using namespace memory_engine;

	TEST(TestViewCollectionWalker, is_other_tag_false)
	{
		ASSERT_TRUE(! is_other_tag(L"ref_count", tag_name_holder::instance())) ;
		ASSERT_TRUE(! is_other_tag(L"date_created", tag_name_holder::instance())) ;
		ASSERT_TRUE(! is_other_tag(L"modified", tag_name_holder::instance())) ;
		ASSERT_TRUE(! is_other_tag(L"memory", tag_name_holder::instance())) ;
		ASSERT_TRUE(! is_other_tag(L"rich_query", tag_name_holder::instance())) ;
		ASSERT_TRUE(! is_other_tag(L"rich_source", tag_name_holder::instance())) ;
	}
	// reliability
	TEST(TestViewCollectionWalker, set_reliability_0)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"0") ;
		record_pointer rec(new record_local()) ;
		rec->set_reliability(5u) ;

		CViewCollectionWalker walker ;
		walker.SetReliabilityFromElement( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_EQUALS_V(0, (int)rec->get_reliability()) ;

	}
	TEST(TestViewCollectionWalker, set_reliability_9)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"9") ;
		record_pointer rec(new record_local()) ;
		ASSERT_EQUALS_V(0, (int)rec->get_reliability()) ;

		CViewCollectionWalker walker ;
		walker.SetReliabilityFromElement( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_EQUALS_V(9, (int)rec->get_reliability()) ;

	}
	TEST(TestViewCollectionWalker, reliability_throws_on_spam)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"spam") ;
		record_pointer rec(new record_local()) ;
		ASSERT_EQUALS_V(0, (int)rec->get_reliability()) ;

		try
		{
			CViewCollectionWalker walker ;
			walker.SetReliabilityFromElement( element_wrapper_ptr(wrapper), rec ) ;
			FAIL_M("Should have thrown on value of 'spam'") ;
		}
		catch (std::exception&)
		{
			ASSERT_EQUALS_V(0, (int)rec->get_reliability()) ;
		}
	}
	// validated
	TEST(TestViewCollectionWalker, set_validated_on_yes)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"yes") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.SetValidatedFromElement( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(rec->is_validated()) ;
	}
	TEST(TestViewCollectionWalker, set_validated_on_true)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"TRUE") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.SetValidatedFromElement( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(rec->is_validated()) ;
	}
	TEST(TestViewCollectionWalker, set_validated_off)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_inner_text(L"spam") ;
		record_pointer rec(new record_local()) ;
		rec->set_validated_off() ;

		CViewCollectionWalker walker ;
		walker.SetValidatedFromElement( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(! rec->is_validated()) ;
	}

	// set_item
	TEST(TestViewCollectionWalker, SetItemFromElement_td)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_tag(L"td") ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;
		ASSERT_TRUE(rec->get_item(L"spam").empty()) ;

		CViewCollectionWalker walker ;
		walker.SetItemFromElement( element_wrapper_ptr(wrapper), rec, L"spam" ) ;

		CStringA actual = rec->get_item(L"spam").c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, SetItemFromElement_other)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_tag(L"p") ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.SetItemFromElement( element_wrapper_ptr(wrapper), rec, L"spam" ) ;

		ASSERT_TRUE(rec->get_item(L"spam").empty()) ;
	}
	// element to record
	TEST(TestViewCollectionWalker, ElementToRecord_empty_id)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(rec->get_item(L"spam").empty()) ;
		ASSERT_TRUE(rec->get_source_rich().empty()) ;
		ASSERT_TRUE(rec->get_trans_rich().empty()) ;
		ASSERT_TRUE(rec->get_context_rich().empty()) ;
		ASSERT_TRUE(! rec->is_validated()) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_source)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"source" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_source_rich().c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_trans)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"trans" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(rec->get_source_rich().empty()) ;
		CStringA actual = rec->get_trans_rich().c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_context)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"context" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_context_rich().c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_reliability_5)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"reliability" ;
		wrapper->set_inner_text(L"5") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_EQUALS_V(5, (int)rec->get_reliability()) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_validated)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"validated" ;
		wrapper->set_inner_text(L"true") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(rec->is_validated()) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_dummy_trans)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"dummy_trans" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		ASSERT_TRUE(rec->get_source_rich().empty()) ;
		CStringA actual = rec->get_trans_rich().c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_dummy_source)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"dummy_source" ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_source_rich().c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, ElementToRecord_spam)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->m_id = L"spam" ;
		wrapper->set_tag(L"td") ;
		wrapper->set_inner_text(L"egg") ;
		record_pointer rec(new record_local()) ;

		CViewCollectionWalker walker ;
		walker.ElementToRecord( element_wrapper_ptr(wrapper), rec ) ;

		CStringA actual = rec->get_item(L"spam").c_str() ;
		SimpleString expected = "egg" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	// adding records

	// "/foo.html" -> "file:///C:/foo.html"
	// "bar.html" -> "file:///C:/Users/RyanVista/AppData/Local/Felix/html/en/bar.html"
	// doc_url = "file://C:\Users\RyanVista\AppData\Local\Felix\html\en\start.html"
	TEST(TestViewCollectionWalker, RepairLinkUrl_foo)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;

		wrapper->set_attribute(L"HREF", L"file:///C:/foo.html") ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.RepairLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		SimpleString expected = "/foo.html" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, RepairLinkUrl_bar)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;

		wrapper->set_attribute(L"HREF", L"file:///C:/Users/RyanVista/AppData/Local/Felix/html/en/bar.html") ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.RepairLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		SimpleString expected = "bar.html" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, RepairLinkUrl_ginstrom)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		element_wrapper_ptr element(wrapper) ;

		wrapper->set_attribute(L"HREF", L"http://ginstrom.com/") ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.RepairLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		SimpleString expected = "http://ginstrom.com/" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(TestViewCollectionWalker, CheckLinkUrl_ginstrom)
	{
		element_wrapper_fake *wrapper = new element_wrapper_fake() ;
		wrapper->set_tag(L"a") ;
		wrapper->set_attribute(L"HREF", L"http://ginstrom.com/") ;
		element_wrapper_ptr element(wrapper) ;

		CViewCollectionWalker walker ;
		wstring doc_url = L"file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		walker.CheckLinkUrl(element, doc_url) ;

		CStringA actual = wrapper->get_attribute(L"HREF").c_str() ;
		SimpleString expected = "http://ginstrom.com/" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}


}

#endif