/*!
	@brief Unit tests for memory class
	@author Ryan Ginstrom
 */

#include "StdAfx.h"
#include "TranslationMemory.h"

#include "Path.h"

#include "easyunit/testharness.h"
#include "MockListener.h"

#ifdef _DEBUG
#ifdef UNIT_TEST

namespace easyunit
{
	using namespace memory_engine ;

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
	void add_record(memory_local &mem, string source, string trans)
	{
		mem.add_record(make_record(source, trans)) ;
	}
	void add_record(memory_local &mem, wstring source, wstring trans)
	{
		mem.add_record(make_record(source, trans)) ;
	}

	TEST(TestMemoryFunctions, get_load_failure_msg)
	{
		CString filename = "spam" ;
		CStringA actual = get_load_failure_msg(filename) ;
		SimpleString expected = "Failed to load file [spam]" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

	TEST( TestMemory, MemoryId )
	{
		memory_local mem ;

		ASSERT_TRUE_M(mem.get_id() > 0, ulong2string(mem.get_id()).c_str()) ;
	}
	TEST( TestMemory, is_new )
	{
		memory_local mem ;

		ASSERT_TRUE_M(mem.is_new(), "New memories must be new") ;
	}
	TEST(TestMemory, is_local)
	{
		memory_local mem ;
		ASSERT_TRUE( mem.is_local()) ;
	}

	TEST( TestMemory, RefreshUserName )
	{
		memory_local mem ;

		memory_header *header = &mem.get_header() ;
		header->set_creator( L"FooBar" ) ;
		ASSERT_EQUALS( L"FooBar", mem.get_header().get_creator() ) ;

		app_props::properties_general props ;
		props.read_from_registry() ;
		_tcscpy_s(props.m_data.m_user_name, MAX_PATH, _T("Ryan")) ;
		props.write_to_registry() ;

		SimpleString actual = string2string(header->get_current_user()).c_str() ;
		ASSERT_EQUALS_V( "Ryan",  actual) ;
	}

	TEST( TestMemory, AddRecord )
	{
		memory_local mem ;
		ASSERT_EQUALS( 0u, mem.size() ) ;
		ASSERT_EQUALS( true, mem.empty() ) ;

		add_record(mem, "dummy", "dummy") ;

		ASSERT_EQUALS( 1u, mem.size() ) ;
		ASSERT_EQUALS( false, mem.empty() ) ;

	}

	TEST( TestMemory, add_record_same_ids )
	{
		memory_local mem ;

		record_pointer r1 = make_record("r1", "t1") ;
		record_pointer r2 = make_record("r2", "t2") ;

		r1->set_id(3) ;
		r2->set_id(3) ;

		mem.add_record(r1) ;
		mem.add_record(r2) ;

		ASSERT_EQUALS_V( 3, (int)r1->get_id() ) ;
		ASSERT_TRUE( r2->get_id() != 3 ) ;

	}

	// id
	TEST( TestMemory, add_record_id_set )
	{
		memory_local mem ;
		record_pointer rec = make_record("spam1", "spam2") ;

		ASSERT_EQUALS_V(0, (int)rec->get_id()) ;
		mem.add_record(rec) ;
		ASSERT_TRUE_M(rec->get_id() != 0, "The id of the record should have been set") ;
	}
	TEST( TestMemory, add_record_diff_ids_set )
	{
		memory_local mem ;
		record_pointer rec1 = make_record("spam1", "spam2") ;
		record_pointer rec2 = make_record("egg1", "egg2") ;
		ASSERT_EQUALS_V((int)rec1->get_id(), (int)rec2->get_id()) ;
		mem.add_record(rec1) ;
		mem.add_record(rec2) ;
		ASSERT_TRUE_M(rec1->get_id() != rec2->get_id(), "The ids of the records should not be equal") ;
	}
	TEST( TestMemory, id_of_record_not_changed_if_nonzero )
	{
		memory_local mem ;
		record_pointer rec = make_record("spam1", "spam2") ;
		rec->set_id(57) ;
		mem.add_record(rec) ;
		ASSERT_EQUALS_V(57, (int)rec->get_id()) ;
	}
	// add by id
	TEST( TestMemory, add_by_id_new )
	{
		memory_local mem ;
		record_pointer rec = mem.add_by_id(10, L"spam", L"egg") ;
		ASSERT_EQUALS_V(10, (int)rec->get_id()) ;
		CStringA actual = rec->get_source_plain().c_str() ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST( TestMemory, add_by_id_dup )
	{
		memory_local mem ;
		record_pointer rec = make_record("spam1", "spam2") ;
		mem.add_record(rec) ;
		mem.add_by_id(rec->get_id(), L"added source", L"added trans") ;
		CStringA actual = rec->get_source_plain().c_str() ;
		SimpleString expected = "added source" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST( TestMemory, add_by_id_non_empty )
	{
		memory_local mem ;
		add_record(mem, "dummy", "dummy") ;
		record_pointer rec = mem.add_by_id(10, L"spam", L"egg") ;
		ASSERT_EQUALS_V(10, (int)rec->get_id()) ;
		CStringA actual = rec->get_source_plain().c_str() ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
		ASSERT_EQUALS_V(2, (int)mem.size()) ;
	}
	// clear_memory
	TEST( TestMemory, clear_memory_empty )
	{
		memory_local mem ;

		mem.clear_memory() ;
		ASSERT_EQUALS(0u, mem.size()) ;
		ASSERT_EQUALS(true, mem.is_saved()) ;
		ASSERT_TRUE(mem.m_file_location.IsEmpty()) ;
	}
	TEST( TestMemory, clear_memory_nonempty )
	{
		memory_local mem ;
		mem.set_location(_T("C:\\test\\memory_serves\\aga.xml")) ;

		record_pointer rec(new record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		mem.add_record(rec) ;

		mem.clear_memory() ;
		ASSERT_EQUALS(0u, mem.size()) ;
		ASSERT_EQUALS(false, mem.is_saved()) ;
		ASSERT_TRUE(mem.m_file_location.IsEmpty()) ;
	}
	// remove_extra_string
	TEST( TestMemory, remove_extra_string )
	{
		memory_local mem ;

		mem.m_extra_strings[L"foo"] = L"bar" ;
		ASSERT_TRUE(! mem.m_extra_strings.empty()) ;

		mem.remove_extra_string(L"foo") ;

		ASSERT_TRUE(mem.m_extra_strings.empty()) ;
	}

	// erase
	TEST( TestMemory, erase )
	{
		memory_local mem ;

		record_pointer rec(new record_local()) ;
		rec->set_source(L"spam") ;
		rec->set_trans(L"egg") ;
		mem.add_record(rec) ;

		ASSERT_EQUALS( 1u, mem.size() ) ;
		ASSERT_EQUALS( false, mem.empty() ) ;

		mem.erase(rec) ;
		ASSERT_EQUALS( 0u, mem.size() ) ;
		ASSERT_EQUALS( true, mem.empty() ) ;
	}

	// set_extra_string
	TEST( TestMemory, set_extra_string_add )
	{
		memory_local mem ;
		ASSERT_EQUALS( L"", mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"bar" ) ;
		ASSERT_EQUALS_V(1, (int)mem.m_extra_strings.size()) ;
		ASSERT_EQUALS( L"bar", mem.get_extra_string(L"foo") ) ;
	}
	TEST( TestMemory, set_extra_string_erase )
	{
		memory_local mem ;
		ASSERT_EQUALS( L"", mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"bar" ) ;
		ASSERT_EQUALS_V(1, (int)mem.m_extra_strings.size()) ;
		ASSERT_EQUALS( L"bar", mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"" ) ;
		ASSERT_EQUALS_V(0, (int)mem.m_extra_strings.size()) ;
		ASSERT_EQUALS( L"", mem.get_extra_string(L"foo") ) ;
	}


	// get_correct_encoding
	TEST( memory_localTestCase, GetFileEncodingXmlUtf8 )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;

		// xml file
		path.Append( _T("TestMem.xml") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view xml_view ;
		char *text = (char *)xml_view.create_view( path.Path() ) ;

		ASSERT_EQUALS( CP_UTF8, mem.get_correct_encoding( text, file_size ) ) ;
	}
	TEST( memory_localTestCase, GetFileEncodingTextUtf8 )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;

		// utf-8 text file
		path.ReplaceFileSpec( _T("UTF8.txt") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view utf8_view ;
		char *text = (char *)utf8_view.create_view( path.Path() ) ;

		ASSERT_EQUALS( CP_UTF8, mem.get_correct_encoding( text, file_size ) ) ;

	}
	TEST( memory_localTestCase, GetFileEncodingTextSjis )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;
		// sjis text file

		path.ReplaceFileSpec( _T("SJIS.txt") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view sjis_view ;
		char *text = (char *)sjis_view.create_view( path.Path() ) ;

		ASSERT_EQUALS( 932u, mem.get_correct_encoding( text, file_size ) ) ;

	}
	TEST( memory_localTestCase, GetFileEncodingTextUtf16 )
	{
		file::CPath path( CString( _T("c:\\dev\\Test Files\\MemoryFiles\\") ) ) ;
		memory_local mem ;
		// utf-16 text file

		path.ReplaceFileSpec( _T("UTF16.txt") ) ;

		UINT file_size = file::file::size( path.Path() ) ;

		file::view utf16_view ;
		char *text = (char *)utf16_view.create_view( path.Path() ) ;

		ASSERT_EQUALS( 1200u, mem.get_correct_encoding( text, file_size ) ) ;
	}

	// load_text
	TEST( test_memory_local, TestZeroEntries )
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
		ASSERT_EQUALS( 0, mem.size() ) ;
	}
	TEST( test_memory_local, test_one_entry )
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
		ASSERT_EQUALS_V((int)mem.size(), 1) ;

		record_pointer rec = *(mem.begin()) ;

		ASSERT_EQUALS(L"foo", rec->get_source_rich()) ;
		ASSERT_EQUALS(L"bar", rec->get_trans_rich()) ;
		ASSERT_EQUALS(0, rec->get_reliability()) ;
		ASSERT_EQUALS(false, rec->is_validated()) ;
		ASSERT_EQUALS(0, rec->get_refcount()) ;
	}
	TEST( test_memory_local, test_big_memory )
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
			FAIL_M("Failed to load big memory") ;
		}
		ASSERT_EQUALS_V((int)mem.size(), 1) ;

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

		ASSERT_EQUALS(L"foo", rec->get_source_rich()) ;
		ASSERT_EQUALS(L"bar", rec->get_trans_rich()) ;
		ASSERT_EQUALS(0, rec->get_reliability()) ;
		ASSERT_EQUALS(false, rec->is_validated()) ;
		ASSERT_EQUALS(0, rec->get_refcount()) ;
	}
	TEST( test_memory_local, TestDegenerateEmpty )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		try
		{
			mem.load( _T("C:\\dev\\Test Files\\MemoryFiles\\EmptyFile.xml") ) ;
			FAIL_M( "Must throw on malformed memory (empty file)" ) ;
		}
		catch (CException &)
		{
			ASSERT_TRUE(true) ;
		}
	}

	TEST( test_memory_local, TestDegenerateBogus )
	{
		CMockListener listener ;
		memory_local mem ;
		mem.set_listener( static_cast< CProgressListener* >( &listener )  ) ;
		try
		{
			mem.load( _T("C:\\dev\\Test Files\\MemoryFiles\\ReallyTmx.xml") ) ;
			FAIL_M( "Must throw on malformed memory (really tmx)" ) ;
		}
		catch (CException &)
		{
			ASSERT_TRUE(true) ;
		}
	}

	TEST(test_memory_local, GetMatchesSize1)
	{
		memory_local mem ;
		add_record(mem, "I luv spam", "Yes I do") ;

		TransMatchContainer matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_matches(matches, params) ;

		ASSERT_EQUALS(1, matches.size()) ;
	}
	TEST(test_memory_local, GetMatchesSize2)
	{
		memory_local mem ;
		add_record(mem, "I luv spam", "Yes I do") ;
		add_record(mem, "I love spam", "Yes I do") ;

		TransMatchContainer matches ;
		search_query_params params ;
		params.m_rich_source = L"I luv spam" ;
		params.m_source = L"I luv spam" ;

		mem.find_matches(matches, params) ;

		ASSERT_EQUALS(2, matches.size()) ;
	}
	TEST(test_memory_local, GetMatchesMarkup)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "baab", "baab") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"aa" ;
		params.m_source = L"aa" ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS(1, matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "<span class=\"nomatch\">b</span>aa<span class=\"nomatch\">b</span>" ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS(actual, expected) ;
	}
	TEST(test_memory_local, GetMatchesMarkupIgnoreCaseQuery)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "baab", "baab") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"CAAC" ;
		params.m_source = L"CAAC" ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "<span class=\"nomatch\">C</span>AA<span class=\"nomatch\">C</span>" ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS(actual, expected) ;
	}
	TEST(test_memory_local, GetMatchesMarkupIgnoreCaseSource)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "BAAB", "BAAB") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"CAAC" ;
		params.m_source = L"CAAC" ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS(1, matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "<span class=\"nomatch\">B</span>AA<span class=\"nomatch\">B</span>" ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST(test_memory_local, GetMatchesMarkupWordAlgo)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"I love spam and eggs." ;
		params.m_source = L"I love spam and eggs." ;
		params.m_match_algo = IDC_ALGO_WORD ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS(1, matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	TEST(test_memory_local, GetMatchesMarkupWordAlgoIgnoreCase)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"I LOVE SPAM AND EGGS." ;
		params.m_source = L"I LOVE SPAM AND EGGS." ;
		params.m_match_algo = IDC_ALGO_WORD ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	TEST(test_memory_local, GetMatchesMarkupAutoAlgoIgnoreCaseQuery)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"I LOVE SPAM AND EGGS." ;
		params.m_source = L"I LOVE SPAM AND EGGS." ;
		params.m_match_algo = IDC_ALGO_AUTO ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "I LOVE <span class=\"nomatch\">SPAM</span> AND EGGS." ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	TEST(test_memory_local, GetMatchesMarkupAutoAlgoIgnoreCaseSource)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"I LOVE SPAM AND EGGS." ;
		params.m_source = L"I LOVE SPAM AND EGGS." ;
		params.m_match_algo = IDC_ALGO_AUTO ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	TEST(test_memory_local, GetMatchesMarkupAutoAlgoIgnoreCasePerfect)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love Ham and Eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"I love Ham and Eggs." ;
		params.m_source = L"I love Ham and Eggs." ;
		params.m_match_algo = IDC_ALGO_AUTO ;
		params.m_ignore_case = true ;

		mem.find_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "I love Ham and Eggs." ;
		SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	// trans lookup
	TEST(test_memory_local, GetTransMatchesMarkupWordAlgoIgnoreCaseQuery)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"NAILED TO THE PERCH." ;
		params.m_source = L"NAILED TO THE PERCH." ;
		params.m_match_algo = IDC_ALGO_WORD ;
		params.m_ignore_case = true ;

		mem.find_trans_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "NAILED TO THE PERCH." ;
		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	TEST(test_memory_local, GetTransMatchesMarkupWordAlgoIgnoreCaseTrans)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"NAILED TO THE PERCH." ;
		params.m_source = L"NAILED TO THE PERCH." ;
		params.m_match_algo = IDC_ALGO_WORD ;
		params.m_ignore_case = true ;

		mem.find_trans_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "Nailed to the perch." ;
		SimpleString actual = CStringA(match->get_markup()->GetTrans().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	TEST(test_memory_local, GetTransMatchesMarkupCharAlgoIgnoreCaseTrans)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the porch.") ;

		TransMatchContainer matches ;
		search_query_params params ;

		params.m_rich_source = L"NAILED TO THE PERCH." ;
		params.m_source = L"NAILED TO THE PERCH." ;
		params.m_match_algo = IDC_ALGO_CHAR ;
		params.m_ignore_case = true ;

		mem.find_trans_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		TransMatchContainer::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "Nailed to the p<span class=\"nomatch\">o</span>rch." ;
		SimpleString actual = CStringA(match->get_markup()->GetTrans().c_str()) ;

		ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
	}
	// get_best_match_score
	TEST(test_memory_local, test_get_best_match_score_1_0)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "spam", "spam") ;
		add_record(mem, "eggs", "eggs") ;
		add_record(mem, "sausage", "sausage") ;

		wstring query = L"spam" ;
		ASSERT_EQUALS_DELTA_V(1.0f, mem.get_best_match_score(query), 0.001) ;
	}
	TEST(test_memory_local, test_get_best_match_score_0_5)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "aaaa", "aaaa") ;
		add_record(mem, "bbbb", "bbbb") ;
		add_record(mem, "cccc", "cccc") ;

		wstring query = L"aadd" ;
		ASSERT_EQUALS_DELTA_V(0.5f, mem.get_best_match_score(query), 0.001) ;
	}

	/************************************************************************/
	/* get_glossary_matches                                                 */
	/************************************************************************/
	TEST(test_memory_local, test_get_glossary_matches_100_char)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "egg", "trans") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 100 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;
		search_match_multiset::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		ASSERT_EQUALS(match->get_markup()->GetSource(), L"egg") ;
		ASSERT_EQUALS(match->get_markup()->GetTrans(), L"trans") ;
	}
	TEST(test_memory_local, test_get_glossary_matches_100_word)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "egg", "trans") ;
		add_record(mem, "spam", "tasty pork") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 100 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		ASSERT_EQUALS_V(2, (int)matches.size()) ;
		search_match_multiset::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		ASSERT_EQUALS(match->get_markup()->GetSource(), L"spam") ;
		ASSERT_EQUALS(match->get_markup()->GetTrans(), L"tasty pork") ;
	}
	TEST(test_memory_local, test_get_glossary_matches_50_char)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "eggs", "trans") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 50 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;
		search_match_multiset::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "<span class=\"partial_match1\">eggs</span>" ;
		SimpleString actual = string2string(match->get_markup()->GetSource()).c_str() ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS(match->get_markup()->GetTrans(), L"trans") ;
	}
	TEST(test_memory_local, test_get_glossary_matches_50_word)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "eggs", "trans") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"spam, egg, sausage and spam" ;
		params.m_source = L"spam, egg, sausage and spam" ;
		params.m_ignore_case = true ;

		app_props::properties_glossary props ;
		props.m_data.m_min_score = 50 ;
		mem.set_gloss_props(props) ;

		mem.get_glossary_matches(matches, params) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;
		search_match_multiset::iterator pos = matches.begin() ;
		search_match_ptr match = *pos ;

		SimpleString expected = "<span class=\"partial_match1\">eggs</span>" ;
		SimpleString actual = string2string(match->get_markup()->GetSource()).c_str() ;
		ASSERT_EQUALS_V(expected, actual) ;
		ASSERT_EQUALS(match->get_markup()->GetTrans(), L"trans") ;
	}
	TEST(test_memory_local, test_get_glossary_matches_ignore_case_query_char)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "spam", "spam") ;
		add_record(mem, "egg", "egg") ;
		add_record(mem, "sausage", "sausage") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"I'll HAVE SPAM, EGG, SAUSAGE AND SPAM." ;
		params.m_source = L"I'll HAVE SPAM, EGG, SAUSAGE AND SPAM." ;
		params.m_ignore_case = true ;

		mem.get_glossary_matches(matches, params) ;

		ASSERT_EQUALS_V(3, (int)matches.size()) ;
	}

	TEST(test_memory_local, test_get_glossary_matches_ignore_case_source)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "SPAM", "SPAM") ;
		add_record(mem, "EGG", "EGG") ;
		add_record(mem, "SAUSAGE", "SAUSAGE") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"i'll have spam, egg, sausage and spam." ;
		params.m_source = L"i'll have spam, egg, sausage and spam." ;
		params.m_ignore_case = true ;

		mem.get_glossary_matches(matches, params) ;

		ASSERT_EQUALS_V(3, (int)matches.size()) ;
	}

	/************************************************************************/
	/* get_gloss_fuzzy                                                      */
	/************************************************************************/
	TEST(test_memory_local, get_gloss_fuzzy_kata_100)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"豚肉", L"豚肉") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>スパム</b>が大好きだ。" ;
		params.m_source = L"私はスパムが大好きだ。" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		ASSERT_EQUALS_V(1, (int)matches.size()) ;
	}

	TEST(test_memory_local, get_gloss_fuzzy_kanji_100)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"豚肉", L"豚肉") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>豚肉</b>が大好きだ。" ;
		params.m_source = L"私は豚肉が大好きだ。" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		ASSERT_EQUALS_V(1, (int)matches.size()) ;
	}

	TEST(test_memory_local, get_gloss_fuzzy_hira_100)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"豚肉", L"豚肉") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>りんごム</b>が大好きだ。" ;
		params.m_source = L"私はりんごが大好きだ。" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		ASSERT_EQUALS_V(1, (int)matches.size()) ;
	}

	TEST(test_memory_local, get_gloss_fuzzy_kata_exact)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"ビニロン", L"ビニロン") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"ビニロン" ;
		params.m_source = L"ビニロン" ;
		params.m_ignore_case = true ;

		mem.get_gloss_fuzzy(matches, params) ;

		ASSERT_EQUALS_V(1, (int)matches.size()) ;
		search_match_ptr match = *matches.begin() ;
		ASSERT_EQUALS_DELTA_V(1.0, match->get_score(), 0.00001) ;
	}


	TEST(test_memory_local, get_gloss_fuzzy_kata_50)
	{
		memory_local mem(0.0f) ;
		add_record(mem, L"スパム", L"スパム") ;
		add_record(mem, L"ベーコン", L"ベーコン") ;
		add_record(mem, L"りんご", L"りんご") ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_rich_source = L"私は<b>ベー</b>が大好きだ。" ;
		params.m_source = L"私はベーが大好きだ。" ;
		params.m_ignore_case = true ;

		mem.m_gloss_properties.set_min_score(10) ;
		mem.get_gloss_fuzzy(matches, params) ;

		ASSERT_EQUALS_V(1, (int)matches.size()) ;
		search_match_ptr match = *matches.begin() ;
		ASSERT_EQUALS_DELTA_V(0.5, match->get_score(), 0.00001) ;
	}


	//////////////////////////////////////////////////////////////////////////
	// tabulate_fonts
	//////////////////////////////////////////////////////////////////////////
	TEST(test_memory_local, test_tabulate_fonts_size_2)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "<font face=\"Helvetica\">spam</font>", "spam") ;
		add_record(mem, "<font face=\"Times\">spam</font>", "spam") ;

		font_tabulator tabulator ;

		mem.tabulate_fonts(tabulator) ;

		font_tabulator::font_set fonts = tabulator.get_font_set() ;
		ASSERT_EQUALS_V(2, (int)fonts.size()) ;
		ASSERT_TRUE(fonts.find(L"helvetica") != fonts.end()) ;
		ASSERT_TRUE(fonts.find(L"times") != fonts.end()) ;
		ASSERT_TRUE(fonts.find(L"symbol") == fonts.end()) ;
	}
	
	/************************************************************************/
	/* concordance                                                          */
	/************************************************************************/
	TEST(test_memory_local, concordance_0)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		search_match_multiset matches ;
		search_query_params params ;

		// this will hold our matches
		params.m_source = L"zzz" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		mem.perform_search( matches, params ) ;
		ASSERT_EQUALS_V(0, (int)matches.size()) ;
	}
	TEST(test_memory_local, concordance_1)
	{
		memory_local mem(0.0f) ;
		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		search_match_multiset matches ;
		search_query_params params ;

		// this will hold our matches
		params.m_source = L"ham" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		mem.perform_search( matches, params ) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		search_match_ptr match = *(matches.begin()) ;
		ASSERT_EQUALS(match->get_record()->get_source_rich(), L"I love ham and eggs.") ;
		ASSERT_EQUALS_V((int)match->get_memory_id(), (int)mem.get_id()) ;
	}

	// reverse concordance

	TEST(test_memory_local, rconcordance)
	{
		memory_local mem(0.0f) ;

		search_match_multiset matches ;
		search_query_params params ;

		params.m_trans = L"perch" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		// on empty memory
		mem.perform_search( matches, params ) ;
		ASSERT_TRUE(matches.empty()) ;

		add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

		params.m_trans = L"zzz" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		// one record but no match
		mem.perform_search( matches, params ) ;
		ASSERT_TRUE(matches.empty()) ;

		// this will hold our matches
		params.m_trans = L"perch" ;
		params.m_ignore_case = true ;
		params.m_ignore_width = true ;
		params.m_ignore_hira_kata =	true ;
		params.m_use_regex = false ;

		// one match
		mem.perform_search( matches, params ) ;
		ASSERT_EQUALS_V(1, (int)matches.size()) ;

		search_match_ptr match = *(matches.begin()) ;
		ASSERT_EQUALS(match->get_record()->get_trans_rich(), L"Nailed to the perch.") ;

		add_record(mem, "I love ham and eggs with toast.", "Nailed to the perch.") ;

		// two matches
		matches.clear() ;
		mem.perform_search( matches, params ) ;
		ASSERT_EQUALS_V(2, (int)matches.size()) ;

		match = *(matches.begin()) ;
		ASSERT_EQUALS(match->get_record()->get_trans_rich(), L"Nailed to the perch.") ;
	}


}
#endif // UNIT_TEST
#endif // #ifdef _DEBUG
