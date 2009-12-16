#include "StdAfx.h"
#include "view_interface.h"
#include "record_local.h"
#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine;

	TEST(test_view_interface, get_doc_path)
	{
		view_interface vi ;
		SimpleString expected = "http://localhost/" ;
		wstring url = L"http://localhost/foo.html" ;
		wstring docpath = vi.get_doc_path(url) ;
		CStringA actual = docpath.c_str() ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_view_interface, get_doc_path_filesystem)
	{
		view_interface vi ;
		SimpleString expected = "file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		wstring url = L"file://C:\\Users\\RyanVista\\AppData\\Local\\Felix\\html\\en\\start.html" ;
		wstring docpath = vi.get_doc_path(url) ;
		CStringA actual = docpath.c_str() ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	TEST(test_view_interface, get_match_record_one)
	{
		view_interface vi ;
		translation_match_query matches ;
		matches.set_query_rich(L"spam") ;

		search_match_ptr match = search_match_ptr(new search_match) ;
		record_pointer record = record_pointer(new record_local) ;
		record->set_source(L"egg") ;
		match->set_record(record) ;
		match->set_values_to_record() ;

		std::vector< search_match_ptr > ml ;
		ml.push_back(match) ;
		matches.set_matches(ml) ;

		record_pointer rec = vi.get_match_record(matches) ;

		SimpleString actual = string2string(rec->get_source_plain()).c_str() ;
		SimpleString expected = "egg" ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_view_interface, get_match_record_empty)
	{
		view_interface vi ;
		mem_engine::translation_match_query matches ;
		matches.set_query_rich(L"spam") ;

		mem_engine::record_pointer rec = vi.get_match_record(matches) ;

		SimpleString actual = string2string(rec->get_source_plain()).c_str() ;
		SimpleString expected = "spam" ;

		ASSERT_EQUALS_V(expected, actual) ;
	}

}

#endif