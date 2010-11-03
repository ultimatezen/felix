#include "stdafx.h"
#include "memory_local.h"

#include "Path.h"
#include "record_local.h"

#include "MockListener.h"
#include "DemoException.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

#include <amop/include/MockObject.h>
#include "input_device_fake.h"

using namespace mem_engine ;
using namespace except ;

record_pointer make_record(wstring source, wstring trans)
{
	record_pointer rec(new record_local) ;
	rec->set_source(string2wstring(source)) ;
	rec->set_trans(string2wstring(trans)) ;
	return rec ;
}
record_pointer make_record(string source, string trans)
{
	return make_record(string2wstring(source), string2wstring(trans)) ;
}
record_pointer add_record(memory_local &mem, string source, string trans)
{
	record_pointer rec = make_record(source, trans) ;
	mem.add_record(rec) ;
	return rec ;
}
record_pointer add_record(memory_local &mem, wstring source, wstring trans)
{
	record_pointer rec = make_record(source, trans) ;
	mem.add_record(make_record(source, trans)) ;
	return rec ;
}

BOOST_AUTO_TEST_SUITE( TestMemory )

	// should_check_for_demo
	BOOST_AUTO_TEST_CASE( should_check_for_demo_false_empty )
	{
		const memory_local mem ;
		BOOST_CHECK(! mem.should_check_for_demo()) ;
	}
	BOOST_AUTO_TEST_CASE( should_check_for_demo_false )
	{
		BOOST_STATIC_ASSERT(MAX_MEMORY_SIZE_FOR_DEMO == 10) ;
		memory_local mem ;
		const wstring dummy(L"a") ;
		for (size_t i=0 ; i<MAX_MEMORY_SIZE_FOR_DEMO+1 ; ++i)
		{
			add_record(mem, dummy, boost::lexical_cast<wstring>(i)) ;
		}
		BOOST_CHECK_EQUAL((int)mem.size(), (int)MAX_MEMORY_SIZE_FOR_DEMO+1) ;
		BOOST_CHECK(! mem.should_check_for_demo()) ;
	}
	BOOST_AUTO_TEST_CASE( should_check_for_demo_true )
	{
		memory_local mem ;
		const wstring dummy(L"a") ;
		for (size_t i=0 ; i< 100 ; ++i)
		{
			add_record(mem, dummy, boost::lexical_cast<wstring>(i)) ;
		}
		BOOST_CHECK_EQUAL((int)mem.size(), 100) ;
		BOOST_CHECK(mem.should_check_for_demo()) ;
	}

	// get_id
	BOOST_AUTO_TEST_CASE( MemoryId )
	{
		memory_local mem ;

		BOOST_CHECK(mem.get_id() > 0) ;
	}

	// is_new
	BOOST_AUTO_TEST_CASE( is_new )
	{
		memory_local mem ;

		BOOST_CHECK(mem.is_new()) ;
	}
	BOOST_AUTO_TEST_CASE( is_local)
	{
		memory_local mem ;
		BOOST_CHECK( mem.is_local()) ;
	}


	// add_record
	BOOST_AUTO_TEST_CASE( AddRecord )
	{
		memory_local mem ;
		BOOST_CHECK_EQUAL( 0u, mem.size() ) ;
		BOOST_CHECK_EQUAL( true, mem.empty() ) ;

		add_record(mem, "dummy", "dummy") ;

		BOOST_CHECK_EQUAL( 1u, mem.size() ) ;
		BOOST_CHECK_EQUAL( false, mem.empty() ) ;

	}
	BOOST_AUTO_TEST_CASE( add_record_same_ids )
	{
		memory_local mem ;

		record_pointer r1 = make_record("r1", "t1") ;
		record_pointer r2 = make_record("r2", "t2") ;

		r1->set_id(3) ;
		r2->set_id(3) ;

		mem.add_record(r1) ;
		mem.add_record(r2) ;

		BOOST_CHECK_EQUAL( 3, (int)r1->get_id() ) ;
		BOOST_CHECK( r2->get_id() != 3 ) ;

	}
	BOOST_AUTO_TEST_CASE( add_record_same_diff_ids )
	{
		memory_local mem ;

		record_pointer r1 = make_record("s", "t") ;
		record_pointer r2 = make_record("s", "t") ;

		r1->set_id(1) ;
		r2->set_id(3) ;

		mem.add_record(r1) ;
		mem.add_record(r2) ;

		BOOST_CHECK_EQUAL( 1u, mem.size() ) ;
	}

	// batch_set_reliability
	BOOST_AUTO_TEST_CASE( batch_set_reliability_5)
	{
		memory_local mem ;
		record_pointer rec1 = add_record(mem, "dummy1", "dummy1") ;
		record_pointer rec2 = add_record(mem, "dummy2", "dummy2") ;

		BOOST_CHECK_EQUAL( 0u, rec1->get_reliability() ) ;
		BOOST_CHECK_EQUAL( 0u, rec2->get_reliability() ) ;

		mem.batch_set_reliability(5u) ;

		BOOST_CHECK_EQUAL( 5, (int)rec1->get_reliability() ) ;
		BOOST_CHECK_EQUAL( 5, (int)rec2->get_reliability() ) ;
	}
	BOOST_AUTO_TEST_CASE( batch_set_reliability_15_plus)
	{
		memory_local mem ;
		record_pointer rec1 = add_record(mem, "dummy1", "dummy1") ;
		record_pointer rec2 = add_record(mem, "dummy2", "dummy2") ;

		BOOST_CHECK_EQUAL( 0u, rec1->get_reliability() ) ;
		BOOST_CHECK_EQUAL( 0u, rec2->get_reliability() ) ;

		mem.batch_set_reliability(mem_engine::MAX_RELIABILITY+15u) ;

		BOOST_CHECK_EQUAL( (int)mem_engine::MAX_RELIABILITY, (int)rec1->get_reliability() ) ;
		BOOST_CHECK_EQUAL( (int)mem_engine::MAX_RELIABILITY, (int)rec2->get_reliability() ) ;
	}


	// id
	BOOST_AUTO_TEST_CASE( add_record_id_set )
	{
		memory_local mem ;
		record_pointer rec = make_record("spam1", "spam2") ;

		BOOST_CHECK_EQUAL(0u, rec->get_id()) ;
		mem.add_record(rec) ;
		BOOST_CHECK(rec->get_id() != 0) ;
	}
	BOOST_AUTO_TEST_CASE( add_record_diff_ids_set )
	{
		memory_local mem ;
		record_pointer rec1 = make_record("spam1", "spam2") ;
		record_pointer rec2 = make_record("egg1", "egg2") ;
		BOOST_CHECK_EQUAL((int)rec1->get_id(), (int)rec2->get_id()) ;
		mem.add_record(rec1) ;
		mem.add_record(rec2) ;
		BOOST_CHECK(rec1->get_id() != rec2->get_id()) ;
	}

	BOOST_AUTO_TEST_CASE( id_of_record_not_changed_if_nonzero )
	{
		memory_local mem ;
		record_pointer rec = make_record("spam1", "spam2") ;
		rec->set_id(57) ;
		mem.add_record(rec) ;
		BOOST_CHECK_EQUAL(57, (int)rec->get_id()) ;
	}
	// add by id
	BOOST_AUTO_TEST_CASE( add_by_id_new )
	{
		memory_local mem ;
		record_pointer rec = mem.add_by_id(10, L"spam", L"egg") ;
		BOOST_CHECK_EQUAL(10, (int)rec->get_id()) ;
		CStringA actual = rec->get_source_plain().c_str() ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( add_by_id_dup )
	{
		memory_local mem ;
		record_pointer rec = make_record("spam1", "spam2") ;
		mem.add_record(rec) ;
		mem.add_by_id(rec->get_id(), L"added source", L"added trans") ;
		CStringA actual = rec->get_source_plain().c_str() ;
		string expected = "added source" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( add_by_id_non_empty )
	{
		memory_local mem ;
		add_record(mem, "dummy", "dummy") ;
		record_pointer rec = mem.add_by_id(10, L"spam", L"egg") ;
		BOOST_CHECK_EQUAL(10, (int)rec->get_id()) ;
		CStringA actual = rec->get_source_plain().c_str() ;
		string expected = "spam" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
		BOOST_CHECK_EQUAL(2u, mem.size()) ;
	}
	// clear_memory
	BOOST_AUTO_TEST_CASE( clear_memory_empty )
	{
		memory_local mem ;

		mem.clear_memory() ;
		BOOST_CHECK_EQUAL(0u, mem.size()) ;
		BOOST_CHECK_EQUAL(true, mem.is_saved()) ;
		BOOST_CHECK(mem.m_file_location.IsEmpty()) ;
	}
	BOOST_AUTO_TEST_CASE( clear_memory_nonempty )
	{
		memory_local mem ;
		mem.set_location(_T("C:\\test\\memory_serves\\aga.xml")) ;

		record_pointer rec(new record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		mem.add_record(rec) ;

		mem.clear_memory() ;
		BOOST_CHECK_EQUAL(0u, mem.size()) ;
		BOOST_CHECK_EQUAL(false, mem.is_saved()) ;
		BOOST_CHECK(mem.m_file_location.IsEmpty()) ;
	}
	// remove_extra_string
	BOOST_AUTO_TEST_CASE( remove_extra_string )
	{
		memory_local mem ;

		mem.m_extra_strings[L"foo"] = L"bar" ;
		BOOST_CHECK(! mem.m_extra_strings.empty()) ;

		mem.remove_extra_string(L"foo") ;

		BOOST_CHECK(mem.m_extra_strings.empty()) ;
	}

	// erase
	BOOST_AUTO_TEST_CASE( erase )
	{
		memory_local mem ;

		record_pointer rec(new record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		mem.add_record(rec) ;

		BOOST_CHECK_EQUAL( 1u, mem.size() ) ;
		BOOST_CHECK_EQUAL( false, mem.empty() ) ;

		mem.erase(rec) ;
		BOOST_CHECK_EQUAL( 0u, mem.size() ) ;
		BOOST_CHECK_EQUAL( true, mem.empty() ) ;
	}

	// set_extra_string
	BOOST_AUTO_TEST_CASE( set_extra_string_add )
	{
		memory_local mem ;
		BOOST_CHECK( L"" == mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"bar" ) ;
		BOOST_CHECK_EQUAL(1u, mem.m_extra_strings.size()) ;
		BOOST_CHECK( L"bar" == mem.get_extra_string(L"foo") ) ;
	}
	BOOST_AUTO_TEST_CASE( set_extra_string_erase )
	{
		memory_local mem ;
		BOOST_CHECK( L"" == mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"bar" ) ;
		BOOST_CHECK_EQUAL(1u, mem.m_extra_strings.size()) ;
		BOOST_CHECK( L"bar" == mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"" ) ;
		BOOST_CHECK_EQUAL(0u, mem.m_extra_strings.size()) ;
		BOOST_CHECK( L"" == mem.get_extra_string(L"foo") ) ;
	}


	// get_correct_encoding
	BOOST_AUTO_TEST_CASE( GetFileEncodingXmlUtf8 )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;

		// xml file
		path.Append( _T("TestMem.xml") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view xml_view ;
		char *text = (char *)xml_view.create_view( path.Path() ) ;

		BOOST_CHECK_EQUAL( (int)CP_UTF8, (int)mem.get_correct_encoding( text, file_size ) ) ;
	}
	BOOST_AUTO_TEST_CASE( GetFileEncodingTextUtf8 )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;

		// utf-8 text file
		path.ReplaceFileSpec( _T("UTF8.txt") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view utf8_view ;
		char *text = (char *)utf8_view.create_view( path.Path() ) ;

		BOOST_CHECK_EQUAL( (int)CP_UTF8, (int)mem.get_correct_encoding( text, file_size ) ) ;

	}
	BOOST_AUTO_TEST_CASE( GetFileEncodingTextSjis )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;
		// sjis text file

		path.ReplaceFileSpec( _T("SJIS.txt") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view sjis_view ;
		char *text = (char *)sjis_view.create_view( path.Path() ) ;

		BOOST_CHECK_EQUAL( 932u, mem.get_correct_encoding( text, file_size ) ) ;

	}
	BOOST_AUTO_TEST_CASE( GetFileEncodingTextUtf16 )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;
		// utf-16 text file

		path.ReplaceFileSpec( _T("UTF16.txt") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view utf16_view ;
		char *text = (char *)utf16_view.create_view( path.Path() ) ;

		BOOST_CHECK_EQUAL( 1200u, mem.get_correct_encoding( text, file_size ) ) ;
	}

	// load_header_raw_text
	BOOST_AUTO_TEST_CASE( load_header_raw_text)
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		LPSTR text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<!DOCTYPE memory >\n"
			"<!-- Created by Felix v 1.5 -->\n"
			"<memory>\n"

			"<head>\n"
			"<creator>Owner</creator>\n"
			"<created_on>2006/07/17 12:33:24</created_on>\n"
			"<creation_tool>Felix</creation_tool>\n"
			"<creation_tool_version>1.5</creation_tool_version>\n"
			"<num_records>10</num_records>\n"
			"<locked>false</locked>\n"
			"<is_memory>true</is_memory>\n"
			"</head>\n"

			"<records>\n"
			"</records>\n"
			"</memory>\n" ;
		mem.load_header_raw_text(text, strlen(text)) ;
		MemoryInfo *info = mem.get_memory_info() ;
		BOOST_CHECK_EQUAL(10, (int)info->get_count()) ;
		BOOST_CHECK_EQUAL(string("Felix"), string((LPCSTR)CW2A(info->get_creation_tool().c_str()))) ;
	}
	// load_text
	BOOST_AUTO_TEST_CASE( TestZeroEntries )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		LPSTR text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<!DOCTYPE memory >\n"
			"<!-- Created by TransAssist v 1.5 -->\n"
			"<memory>\n"

			"<head>\n"
			"<creator>Owner</creator>\n"
			"<created_on>2006/07/17 12:33:24</created_on>\n"
			"<creation_tool>TransAssist</creation_tool>\n"
			"<creation_tool_version>1.5</creation_tool_version>\n"
			"<num_records>0</num_records>\n"
			"<locked>false</locked>\n"
			"<is_memory>true</is_memory>\n"
			"</head>\n"

			"<records>\n"
			"</records>\n"
			"</memory>\n" ;
		CString filename = _T("C:\\dev\\Test Files\\MemoryFiles\\EmptyMemory.xml") ;
		mem.load_text(text, filename, strlen(text)) ;
		BOOST_CHECK_EQUAL( 0u, mem.size() ) ;
	}
	BOOST_AUTO_TEST_CASE( test_one_entry )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		CString filename = _T("C:\\test\\test_memories\\one_record.xml") ;
		LPSTR text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
			"<!DOCTYPE memory >"
			"<!-- Created by Felix v 1.0 -->"
			"<memory>"
			"<head>"
			"<creator>RyanVista</creator>"
			"<created_on>2008/08/15 12:04:14</created_on>"
			"<creation_tool>Felix</creation_tool>"
			"<creation_tool_version>1.0</creation_tool_version>"
			"<num_records>1</num_records>"
			"<locked>false</locked>"
			"<is_memory>true</is_memory>"
			"</head>"

			"<records><record>  <source><![CDATA[foo]]></source>"
			"<trans><![CDATA[bar]]></trans>"
			"<date_created>2008/08/15 12:04:14</date_created>"
			"<last_modified>2008/08/15 12:04:15</last_modified>"
			"<reliability>0</reliability>"
			"<validated>false</validated>"
			"<ref_count>0</ref_count>"
			"</record>"
			"</records></memory>" ;
		mem.load_text(text, filename, strlen(text)) ;
		BOOST_CHECK_EQUAL((int)mem.size(), 1) ;

		record_pointer rec = *(mem.begin()) ;

		BOOST_CHECK(L"foo" == rec->get_source_rich()) ;
		BOOST_CHECK(L"bar" == rec->get_trans_rich()) ;
		BOOST_CHECK_EQUAL(0u, rec->get_reliability()) ;
		BOOST_CHECK(! rec->is_validated()) ;
		BOOST_CHECK_EQUAL(0u, rec->get_refcount()) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// load
	//////////////////////////////////////////////////////////////////////////

	BOOST_AUTO_TEST_CASE( test_big_memory )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		// this will throw an AllocException on load. Make sure our backup
		// function works...
		mem.m_throw_exception = true ;
		try
		{
			mem.load( _T("C:\\test\\test_memories\\one_record.xml") ) ;
		}
		catch (CException&)
		{
			BOOST_FAIL("Failed to load big memory") ;
		}
		BOOST_CHECK_EQUAL((int)mem.size(), 1) ;

		/*
		<source><![CDATA[foo]]></source>
		<trans><![CDATA[bar]]></trans>
		<date_created>2008/08/15 12:04:14</date_created>
		<last_modified>2008/08/15 12:04:15</last_modified>
		<reliability>0</reliability>
		<validated>false</validated>
		<ref_count>0</ref_count>
		*/

		record_pointer rec = *(mem.begin()) ;

		BOOST_CHECK(L"foo" == rec->get_source_rich()) ;
		BOOST_CHECK(L"bar" == rec->get_trans_rich()) ;
		BOOST_CHECK_EQUAL(0u, rec->get_reliability()) ;
		BOOST_CHECK_EQUAL(false, rec->is_validated()) ;
		BOOST_CHECK_EQUAL(0u, rec->get_refcount()) ;
	}
	BOOST_AUTO_TEST_CASE( TestDegenerateEmpty )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		try
		{
			mem.load( _T("C:\\dev\\Test Files\\MemoryFiles\\EmptyFile.xml") ) ;
			BOOST_FAIL( "Must throw on malformed memory (empty file)" ) ;
		}
		catch (CException &)
		{
			BOOST_CHECK(true) ;
		}
	}

	BOOST_AUTO_TEST_CASE( TestDegenerateBogus )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		BOOST_CHECK_THROW(mem.load( _T("C:\\dev\\Test Files\\MemoryFiles\\ReallyTmx.xml") ), CException) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// find_matches
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_find_matches )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( GetMatchesSize1)
	{
		memory_local mem ;
		add_record(mem, "I luv spam", "Yes I do") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_matches(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesSize1_brackets)
	{
		memory_local mem ;
		add_record(mem, "&lt;I luv spam&gt;", "Yes I do") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"&lt;I luv spam&gt;" ;
		params.m_source = strip_tags(params.m_rich_source) ;

		mem.find_matches(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesSize1_not_active)
	{
		memory_local mem ;
		mem.set_active_off() ;
		add_record(mem, "I luv spam", "Yes I do") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_matches(matches, params) ;

		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesSize2)
	{
		memory_local mem ;
		add_record(mem, "I luv spam", "Yes I do") ;
		add_record(mem, "I love spam", "Yes I do") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_matches(matches, params) ;

		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkup)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "baab", "baab") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"aa" ;
		params.m_source = L"aa" ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "<span class=\"nomatch\">b</span>aa<span class=\"nomatch\">b</span>" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupIgnoreCaseQuery)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "baab", "baab") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"CAAC" ;
		params.m_source = L"CAAC" ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "<span class=\"nomatch\">C</span>AA<span class=\"nomatch\">C</span>" ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupIgnoreCaseSource)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "BAAB", "BAAB") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"CAAC" ;
		params.m_source = L"CAAC" ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "<span class=\"nomatch\">B</span>AA<span class=\"nomatch\">B</span>" ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupWordAlgo)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"I love spam and eggs." ;
		params.m_source = L"I love spam and eggs." ;
		params.m_match_algo = IDC_ALGO_WORD ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupWordAlgoIgnoreCase)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"I LOVE SPAM AND EGGS." ;
		params.m_source = L"I LOVE SPAM AND EGGS." ;
		params.m_match_algo = IDC_ALGO_WORD ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupAutoAlgoIgnoreCaseQuery)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"I LOVE SPAM AND EGGS." ;
		params.m_source = L"I LOVE SPAM AND EGGS." ;
		params.m_match_algo = IDC_ALGO_AUTO ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "I LOVE <span class=\"nomatch\">SPAM</span> AND EGGS." ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupAutoAlgoIgnoreCaseSource)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"I LOVE SPAM AND EGGS." ;
		params.m_source = L"I LOVE SPAM AND EGGS." ;
		params.m_match_algo = IDC_ALGO_AUTO ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetMatchesMarkupAutoAlgoIgnoreCasePerfect)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love Ham and Eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"I love Ham and Eggs." ;
		params.m_source = L"I love Ham and Eggs." ;
		params.m_match_algo = IDC_ALGO_AUTO ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "I love Ham and Eggs." ;
		string actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// find_trans_matches
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_find_trans_matches )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE(GetMatchesSize1)
	{
		memory_local mem ;
		add_record(mem, "I luv spam", "I luv spam") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_trans_matches(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}	
	BOOST_AUTO_TEST_CASE(GetMatchesSize1_brackets)
	{
		memory_local mem ;
		add_record(mem, "A random source string", "&lt;I luv spam trans&gt;") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"&lt;I luv spam trans&gt;" ;
		params.m_source = strip_tags(params.m_rich_source) ;

		mem.find_trans_matches(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(GetMatchesSize1_not_active)
	{
		memory_local mem ;
		mem.set_active_off() ;
		add_record(mem, "I luv spam", "I luv spam") ;

		trans_match_container matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_trans_matches(matches, params) ;

		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( GetTransMatchesMarkupWordAlgoIgnoreCaseQuery)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"NAILED TO THE PERCH." ;
		params.m_source = L"NAILED TO THE PERCH." ;
		params.m_match_algo = IDC_ALGO_WORD ;
		params.m_ignore_case = true ;

		mem.find_trans_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "NAILED TO THE PERCH." ;
		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}

	BOOST_AUTO_TEST_CASE( GetTransMatchesMarkupWordAlgoIgnoreCaseTrans)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"NAILED TO THE PERCH." ;
		params.m_source = L"NAILED TO THE PERCH." ;
		params.m_match_algo = IDC_ALGO_WORD ;
		params.m_ignore_case = true ;

		mem.find_trans_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "Nailed to the perch." ;
		string actual = CStringA(match->get_markup()->GetTrans().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE( GetTransMatchesMarkupCharAlgoIgnoreCaseTrans)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the porch.") ;

		trans_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"NAILED TO THE PERCH." ;
		params.m_source = L"NAILED TO THE PERCH." ;
		params.m_match_algo = IDC_ALGO_CHAR ;
		params.m_ignore_case = true ;

		mem.find_trans_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		trans_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "Nailed to the p<span class=\"nomatch\">o</span>rch." ;
		string actual = CStringA(match->get_markup()->GetTrans().c_str()) ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// get_best_match_score
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_get_best_match_score )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( test_get_best_match_score_1_0)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "spam", "spam") ;
		add_record(mem, "eggs", "eggs") ;
		add_record(mem, "sausage", "sausage") ;

		wstring query = L"spam" ;
		BOOST_CHECK_CLOSE((double)1.0f, mem.get_best_match_score(query), 0.001) ;
	}
	BOOST_AUTO_TEST_CASE( test_get_best_match_score_0_5)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "aaaa", "aaaa") ;
		add_record(mem, "bbbb", "bbbb") ;
		add_record(mem, "cccc", "cccc") ;

		wstring query = L"aadd" ;
		BOOST_CHECK_CLOSE((double)0.5f, mem.get_best_match_score(query), 0.001) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// get_glossary_matches
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_get_glossary_matches )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( test_get_glossary_matches_100_char)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "egg", "trans") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 100 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
		search_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		BOOST_CHECK(match->get_markup()->GetSource() == L"egg") ;
		BOOST_CHECK(match->get_markup()->GetTrans() == L"trans") ;
	}
	BOOST_AUTO_TEST_CASE( test_get_glossary_matches_100_word)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "egg", "trans") ;
		add_record(mem, "spam", "tasty pork") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 100 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
		search_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		BOOST_CHECK(match->get_markup()->GetSource() == L"spam") ;
		BOOST_CHECK(match->get_markup()->GetTrans() == L"tasty pork") ;
	}
	BOOST_AUTO_TEST_CASE( test_get_glossary_matches_50_char)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "eggs", "trans") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 50 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
		search_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "<span class=\"partial_match1\">eggs</span>" ;
		string actual = string2string(match->get_markup()->GetSource()).c_str() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_markup()->GetTrans() == L"trans") ;
	}
	BOOST_AUTO_TEST_CASE( test_get_glossary_matches_50_word)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "eggs", "trans") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 50 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
		search_match_container::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		string expected = "<span class=\"partial_match1\">eggs</span>" ;
		string actual = string2string(match->get_markup()->GetSource()).c_str() ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(match->get_markup()->GetTrans() == L"trans") ;
	}
	BOOST_AUTO_TEST_CASE( test_get_glossary_matches_ignore_case_query_char)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "spam", "spam") ;
		add_record(mem, "egg", "egg") ;
		add_record(mem, "sausage", "sausage") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"I'll HAVE SPAM, EGG, SAUSAGE AND SPAM." ;
		params.m_source = L"I'll HAVE SPAM, EGG, SAUSAGE AND SPAM." ;
		params.m_ignore_case = true ;

		mem.get_glossary_matches(matches, params) ;

		BOOST_CHECK_EQUAL(3, (int)matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( test_get_glossary_matches_ignore_case_source)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "SPAM", "SPAM") ;
		add_record(mem, "EGG", "EGG") ;
		add_record(mem, "SAUSAGE", "SAUSAGE") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"i'll have spam, egg, sausage and spam." ;
		params.m_source = L"i'll have spam, egg, sausage and spam." ;
		params.m_ignore_case = true ;

		mem.get_glossary_matches(matches, params) ;

		BOOST_CHECK_EQUAL(3, (int)matches.size()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// get_gloss_fuzzy
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_get_gloss_fuzzy )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( get_gloss_fuzzy_kata_100)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"豚肉", L"豚肉") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>スパム</b>が大好きだ。" ;
		params.m_source = L"私はスパムが大好きだ。" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( get_gloss_fuzzy_kanji_100)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"豚肉", L"豚肉") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>豚肉</b>が大好きだ。" ;
		params.m_source = L"私は豚肉が大好きだ。" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( get_gloss_fuzzy_hira_100)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"豚肉", L"豚肉") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>りんごム</b>が大好きだ。" ;
		params.m_source = L"私はりんごが大好きだ。" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}

	BOOST_AUTO_TEST_CASE( get_gloss_fuzzy_kata_exact)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"ビニロン", L"ビニロン") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"ビニロン" ;
		params.m_source = L"ビニロン" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
		search_match_ptr match = *matches.begin() ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.00001) ;
	}


	BOOST_AUTO_TEST_CASE( get_gloss_fuzzy_kata_50)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"ベーコン", L"ベーコン") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_container matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>ベー</b>が大好きだ。" ;
		params.m_source = L"私はベーが大好きだ。" ;
		params.m_ignore_case = true ;

		mem.m_gloss_properties.set_min_score(10) ;
		mem.get_gloss_fuzzy(matches, params) ;

		BOOST_CHECK_EQUAL(1u, matches.size()) ;
		search_match_ptr match = *matches.begin() ;
		BOOST_CHECK_CLOSE(0.5, match->get_score(), 0.00001) ;
	}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// tabulate_fonts
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_tabulate_fonts )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( test_tabulate_fonts_size_2)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "<font face=\"Helvetica\">spam</font>", "spam") ;
		add_record(mem, "<font face=\"Times\">spam</font>", "spam") ;

		font_tabulator tabulator ;

		mem.tabulate_fonts(tabulator) ;

		font_tabulator::font_set fonts = tabulator.get_font_set() ;
		BOOST_CHECK_EQUAL(2u, fonts.size()) ;
		BOOST_CHECK(fonts.find(L"helvetica") != fonts.end()) ;
		BOOST_CHECK(fonts.find(L"times") != fonts.end()) ;
		BOOST_CHECK(fonts.find(L"symbol") == fonts.end()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// perform_search (concordance)       
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_perform_search_concordance )

	using namespace mem_engine ;
	using namespace except ;

	BOOST_AUTO_TEST_CASE( concordance_0)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		search_match_container matches ;
		search_query_params params ;

		// this will hold our matches
		params.m_source = L"zzz" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		mem.perform_search( matches, params ) ;
		BOOST_CHECK_EQUAL(0u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE( concordance_1)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		search_match_container matches ;
		search_query_params params ;

		// this will hold our matches
		params.m_source = L"ham" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		mem.perform_search( matches, params ) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		search_match_ptr match = *(matches.begin()) ;
		BOOST_CHECK(match->get_record()->get_source_rich() == L"I love ham and eggs.") ;
		BOOST_CHECK_EQUAL((int)match->get_memory_id(), (int)mem.get_id()) ;
	}

	// perform_search (reverse concordance)

	BOOST_AUTO_TEST_CASE( rconcordance)
	{
		memory_local mem(0.0f) ;

		search_match_container matches ;
		search_query_params params ;

		params.m_trans = L"perch" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		// on empty memory
		mem.perform_search( matches, params ) ;
		BOOST_CHECK(matches.empty()) ;

		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		params.m_trans = L"zzz" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		// one record but no match
		mem.perform_search( matches, params ) ;
		BOOST_CHECK(matches.empty()) ;

		// this will hold our matches
		params.m_trans = L"perch" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		// one match
		mem.perform_search( matches, params ) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;

		search_match_ptr match = *(matches.begin()) ;
		BOOST_CHECK(match->get_record()->get_trans_rich() == L"Nailed to the perch.") ;

		add_record(mem, "I love ham and eggs with toast.", "Nailed to the perch.") ;

		// two matches
		matches.clear() ;
		mem.perform_search( matches, params ) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;

		match = *(matches.begin()) ;
		BOOST_CHECK(match->get_record()->get_trans_rich() == L"Nailed to the perch.") ;
	}


BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////
// test_other_mem_local_stuff
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_other_mem_local_stuff )

	using namespace mem_engine ;
	using namespace except ;

	// make_match
	BOOST_AUTO_TEST_CASE(make_match)
	{
		memory_local mem(0.0f) ;
		mem.m_file_location = _T("c:\\foo.ftm") ;

		search_match_ptr match = mem.make_match() ;
		BOOST_CHECK_EQUAL(match->get_memory_id(), mem.get_id()) ;
		BOOST_CHECK_EQUAL(match->get_memory_location(), L"foo") ;
	}

	// adding records
	BOOST_AUTO_TEST_CASE(cant_add_record_to_locked_mem)
	{
		memory_local mem(0.0f) ;

		mem.set_locked_on() ;

		BOOST_CHECK(! mem.add_record(make_record("foo", "bar"))) ;
	}
	BOOST_AUTO_TEST_CASE(cant_add_invalid_record)
	{
		memory_local mem(0.0f) ;

		BOOST_CHECK_THROW(mem.add_record(make_record("foo", "")), CException) ;
	}

	// get_progress_interval
	BOOST_AUTO_TEST_CASE(get_progress_interval_4000)
	{
		memory_local mem(0.0f) ;

		BOOST_CHECK_EQUAL(4000 / 200, mem.get_progress_interval(4000)) ;
	}
	BOOST_AUTO_TEST_CASE(get_progress_interval_10)
	{
		memory_local mem(0.0f) ;

		BOOST_CHECK_EQUAL(10, mem.get_progress_interval(10)) ;
	}
	BOOST_AUTO_TEST_CASE(get_progress_interval_0)
	{
		memory_local mem(0.0f) ;

		BOOST_CHECK_EQUAL(10, mem.get_progress_interval(0)) ;
	}

	// handle exception on load
	BOOST_AUTO_TEST_CASE(exception_size_over_max_demo)
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;

		mem.m_is_demo = true ;

		for (int i = 0 ; i < MAX_MEMORY_SIZE_FOR_DEMO ; ++i )
		{
			mem.add_record(make_record(int2string(i), "trans")) ;
		}
		BOOST_CHECK_EQUAL(MAX_MEMORY_SIZE_FOR_DEMO, mem.size()) ;

		CString filename("c:\\foo.ftm") ;
		CException the_error(_T("boom")) ;
		BOOST_CHECK_THROW(mem.handle_cexception_on_load(filename, false, the_error), CDemoException) ;
	}
	BOOST_AUTO_TEST_CASE(exception_size_over_max_user_says_bail)
	{
		CMockListener listener ;
		listener.m_should_bail = true ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;

		CString filename("c:\\foo.ftm") ;
		CException the_error(_T("boom")) ;
		BOOST_CHECK_THROW(mem.handle_cexception_on_load(filename, false, the_error), CException) ;
		BOOST_CHECK_EQUAL(2u, listener.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("ShouldBailFromException", listener.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL("OnProgressDoneLoad", listener.m_sensing_variable[1]) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_memory_info )

	using namespace mem_engine ;
	using namespace except ;

	// header info
	BOOST_AUTO_TEST_CASE(init)
	{
		memory_local mem(0.0f) ;

		MemoryInfo *info = mem.get_memory_info() ;

		BOOST_CHECK_EQUAL(VERSION, string2string(info->get_creation_tool_version())) ;
		BOOST_CHECK_EQUAL(L"Felix", info->get_creation_tool()) ;
		BOOST_CHECK(! info->is_locked()) ;
	}
	BOOST_AUTO_TEST_CASE(set_locked_on)
	{
		memory_local mem(0.0f) ;

		MemoryInfo *info = mem.get_memory_info() ;

		BOOST_CHECK(! info->is_locked()) ;
		mem.set_locked_on() ;
		BOOST_CHECK(info->is_locked()) ;
	}

	BOOST_AUTO_TEST_CASE( RefreshUserName )
	{
		memory_local mem ;

		MemoryInfo *mem_info = mem.get_memory_info() ;
		mem_info->set_creator( L"FooBar" ) ;
		BOOST_CHECK_EQUAL( L"FooBar", mem.get_memory_info()->get_creator() ) ;

		app_props::properties_general props ;
		props.read_from_registry() ;
		_tcscpy_s(props.m_data.m_user_name, MAX_PATH, _T("Ryan")) ;
		props.write_to_registry() ;

		BOOST_CHECK_EQUAL( L"Ryan",  mem_info->get_current_user()) ;
	}

	BOOST_AUTO_TEST_CASE(get_creation_date)
	{
		memory_local mem ;
		input_device_ptr input(new InputDeviceFake) ;
		input.m_filetime = L"2001/11/12 11:12:13" ;
		mem.get_date_created(_T("foo.txt"), input) ;
		wstring actual = mem.get_memory_info()->get_created_on() ;
		BOOST_CHECK_EQUAL(actual, input.m_filetime) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_copy_mem_info )

using namespace mem_engine ;
using namespace except ;
	// header info
	BOOST_AUTO_TEST_CASE(new_mem)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		copy_mem_info(from, to) ;
		wstring actual = static_cast<LPCWSTR>(to->get_location()) ;
		wstring expected = L"New" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(to->is_new()) ;
	}
	BOOST_AUTO_TEST_CASE(has_loc)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;
		from->set_location("C:\\foo\\bar.ftm") ;

		copy_mem_info(from, to) ;
		wstring actual = static_cast<LPCWSTR>(to->get_location()) ;
		wstring expected = L"C:\\foo\\bar.ftm" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK(! to->is_new()) ;
	}
	BOOST_AUTO_TEST_CASE(set_creator)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_creator(L"Barbie") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_creator() ;
		wstring expected = L"Barbie" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(set_field)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_field(L"Electronics") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_field() ;
		wstring expected = L"Electronics" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE(set_created_on)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_created_on(L"2005/10/05 10:11:12") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_created_on() ;
		wstring expected = L"2005/10/05 10:11:12" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE(set_source_language)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_source_language(L"Pargunese") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_source_language() ;
		wstring expected = L"Pargunese" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE(set_target_language)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_target_language(L"Wookie") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_target_language() ;
		wstring expected = L"Wookie" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE(set_client)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_client(L"Elmo") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_client() ;
		wstring expected = L"Elmo" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}	
	BOOST_AUTO_TEST_CASE(set_count)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;
		from->add_record(make_record("foo", "bar")) ;

		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;

		BOOST_CHECK_EQUAL(1, to_info->get_count()) ;
	}	
	BOOST_AUTO_TEST_CASE(set_locked_on)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_locked_on() ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;

		BOOST_CHECK(to_info->is_locked()) ;
	}	
	BOOST_AUTO_TEST_CASE(set_locked_off)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_locked_off() ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;

		BOOST_CHECK(! to_info->is_locked()) ;
	}	
	BOOST_AUTO_TEST_CASE(set_is_memory_on)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_is_memory_on() ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;

		BOOST_CHECK(to_info->is_memory()) ;
	}	
	BOOST_AUTO_TEST_CASE(set_is_memory_off)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_is_memory_off() ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;

		BOOST_CHECK(! to_info->is_memory()) ;
	}	
	BOOST_AUTO_TEST_CASE(set_creation_tool)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_creation_tool(L"Felix") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_creation_tool() ;
		wstring expected = L"Felix" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(set_creation_tool_version)
	{
		memory_pointer from(new memory_local(0.0f)) ;
		memory_pointer to(new memory_local(0.0f)) ;

		MemoryInfo *from_info = from->get_memory_info() ;
		from_info->set_creation_tool_version(L"1.6.1") ;
		MemoryInfo *to_info = to->get_memory_info() ;

		copy_mem_info(from, to) ;
		wstring actual = to_info->get_creation_tool_version() ;
		wstring expected = L"1.6.1" ;

		BOOST_CHECK_EQUAL(expected, actual) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif
