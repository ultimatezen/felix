#include "StdAfx.h"
#include "view_interface.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_view_interface )

	using namespace mem_engine;

	BOOST_AUTO_TEST_CASE( get_doc_path)
	{
		frame_view vi ;
		string expected = "http://localhost/" ;
		wstring url = L"http://localhost/foo.html" ;
		wstring docpath = vi.get_doc_path(url) ;
		CStringA actual = docpath.c_str() ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( get_doc_path_filesystem)
	{
		frame_view vi ;
		string expected = "file://C:/Users/RyanVista/AppData/Local/Felix/html/en/" ;
		wstring url = L"file://C:\\Users\\RyanVista\\AppData\\Local\\Felix\\html\\en\\start.html" ;
		wstring docpath = vi.get_doc_path(url) ;
		CStringA actual = docpath.c_str() ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}

	BOOST_AUTO_TEST_CASE( get_match_record_one)
	{
		frame_view vi ;
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

		record_pointer rec = vi.get_match_record(&matches) ;

		string actual = string2string(rec->get_source_plain()).c_str() ;
		string expected = "egg" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_match_record_empty)
	{
		frame_view vi ;
		mem_engine::translation_match_query matches ;
		matches.set_query_rich(L"spam") ;

		mem_engine::record_pointer rec = vi.get_match_record(&matches) ;

		string actual = string2string(rec->get_source_plain()).c_str() ;
		string expected = "spam" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
