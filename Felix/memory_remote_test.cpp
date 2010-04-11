/*!
@brief Unit tests for memory class
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "memory_remote.h"
#include "Path.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	using namespace except ;

	record_pointer make_record(const string source, const string trans, const string context = "")
	{
		record_pointer rec(new record_local()) ;
		rec->set_source(string2wstring(source)) ;
		rec->set_trans(string2wstring(trans)) ;
		if (! context.empty())
		{
			rec->set_context(string2wstring(context)) ;
		}
		return rec ;
	}

record_pointer add_record(memory_remote &mem, const string source, const string trans)
{
	record_pointer rec = make_record(source, trans) ;
	mem.add_record(rec) ;
	return rec ;
}

void add_hit(memory_remote &mem, const wstring source, const wstring trans)
{
	CDispatchWrapper record(L"Felix.RemoteRecordFake") ;
	record.prop_put(L"Source", source.c_str()) ;
	record.prop_put(L"Trans", trans.c_str()) ;
	CComVariant v_record((LPDISPATCH)record.get_dispatch());
	v_record.vt = VT_DISPATCH ; 
	mem.m_engine.method(L"AddHit", v_record) ;
}
void add_hit(memory_remote &mem, const string source, const string trans)
{
	add_hit(mem, string2wstring(source), string2wstring(trans)) ;
}

	TEST( test_memory_remote, update )
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			record_pointer record = add_record(mem, "dummy", "dummy") ;

			record_pointer modified = record->clone() ;
			modified->set_source(L"The modified source") ;

			mem.replace(record, modified) ;

			ASSERT_EQUALS( 1u, mem.size() ) ;

			mem.m_engine.method(L"Delete") ;
			ASSERT_TRUE(true) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}

	TEST( test_memory_remote, AddRecord )
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_record(mem, "dummy", "dummy") ;

			ASSERT_EQUALS( 1u, mem.size() ) ;
			ASSERT_EQUALS( false, mem.empty() ) ;

			mem.m_engine.method(L"Delete") ;
			ASSERT_TRUE(true) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	TEST( test_memory_remote, add_by_id_exists )
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;
			add_record(mem, "dummy", "dummy") ;

			ASSERT_EQUALS_V( 1, (int)mem.size() ) ;

			record_pointer record = mem.add_by_id(1, L"source", L"trans") ;
			ASSERT_EQUALS_V( 1, (int)mem.size() ) ;

			ASSERT_EQUALS_V((int)record->get_id(), 1) ;
			ASSERT_EQUALS(L"source", record->get_source_rich()) ;
			ASSERT_EQUALS(L"trans", record->get_trans_rich()) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}


	TEST(test_memory_remote, GetMatchesSize1)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, "I luv spam", "Yes I do") ;

			trans_match_container matches ;
			search_query_params params ;
			params.m_rich_source = L"I luv spam" ;
			params.m_source = L"I luv spam" ;

			mem.find_matches(matches, params) ;

			ASSERT_EQUALS(1, matches.size()) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}

	// from bug found with increment_refcount
	TEST(test_memory_remote, increment_refcount)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, "I luv spam", "Yes I do") ;

			trans_match_container matches ;
			search_query_params params ;
			params.m_rich_source = L"I luv spam" ;
			params.m_source = L"I luv spam" ;

			mem.find_matches(matches, params) ;

			trans_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			record_pointer record = match->get_record() ;

			ASSERT_EQUALS_V(0, (int)record->get_refcount()) ;
			record->increment_refcount() ;
			ASSERT_EQUALS_V(1, (int)record->get_refcount()) ;

			mem.m_engine.method(L"Delete") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}



	TEST(test_memory_remote, GetMatchesMarkup)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, L"baab", L"baab") ;

			trans_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"aa" ;
			params.m_source = L"aa" ;

			mem.find_matches(matches, params) ;
			ASSERT_EQUALS(1, matches.size()) ;

			trans_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			SimpleString expected = "<span class=\"nomatch\">b</span>aa<span class=\"nomatch\">b</span>" ;
			SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

			ASSERT_EQUALS(actual, expected) ;

			mem.m_engine.method(L"Delete") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}


	// trans lookup
	TEST(test_memory_remote, GetTransMatchesMarkupWordAlgoIgnoreCaseQuery)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

			trans_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"NAILED TO THE PERCH." ;
			params.m_source = L"NAILED TO THE PERCH." ;
			params.m_match_algo = IDC_ALGO_WORD ;
			params.m_ignore_case = true ;

			mem.find_trans_matches(matches, params) ;
			ASSERT_EQUALS_V(1, (int)matches.size()) ;

			trans_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			SimpleString expected = "NAILED TO THE PERCH." ;
			SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;

			ASSERT_EQUALS_M(actual, expected, SimpleString("\nexpected: ") + expected + SimpleString("\nactual: ") + actual) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}


	// get_best_match_score

	TEST(test_memory_remote, test_get_best_match_score_0_5)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, L"aaaa", L"aaaa") ;
			add_hit(mem, L"bbbb", L"bbbb") ;
			add_hit(mem, L"cccc", L"cccc") ;

			wstring query = L"aadd" ;
			ASSERT_EQUALS_DELTA_V(0.5f, mem.get_best_match_score(query), 0.001) ;

			mem.m_engine.method(L"Delete") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}
	}
	// get_glossary_matches


	TEST(test_memory_remote, test_get_glossary_matches_50)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, L"eggs", L"trans") ;

			search_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"spam, egg, sausage and spam" ;
			params.m_source = L"spam, egg, sausage and spam" ;
			params.m_ignore_case = true ;

			app_props::properties_glossary props ;
			props.m_data.m_min_score = 50 ;
			mem.set_gloss_props(props) ;

			mem.get_glossary_matches(matches, params) ;
			ASSERT_EQUALS_V(1, (int)matches.size()) ;
			search_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			SimpleString expected = "<span class=\"partial_match1\">eggs</span>" ;
			SimpleString actual = string2string(match->get_markup()->GetSource()).c_str() ;
			ASSERT_EQUALS_V(expected, actual) ;
			ASSERT_EQUALS(match->get_markup()->GetTrans(), L"trans") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			FAIL_M(static_cast< LPCSTR >( msg )) ;
		}

	}

	TEST(test_memory_remote, is_local)
	{
		memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
		ASSERT_TRUE(! mem.is_local()) ;
	}





}


#endif // #ifdef UNIT_TEST
