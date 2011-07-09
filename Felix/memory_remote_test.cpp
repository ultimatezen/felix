/*!
@brief Unit tests for memory class
@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "memory_remote.h"
#include "Path.h"
#include "record_local.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_memory_remote )



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

	BOOST_AUTO_TEST_CASE( update )
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			record_pointer record = add_record(mem, "dummy", "dummy") ;

			record_pointer modified = record->clone() ;
			modified->set_source(L"The modified source") ;

			mem.replace(record, modified) ;

			BOOST_CHECK_EQUAL( 1u, mem.size() ) ;

			mem.m_engine.method(L"Delete") ;
			BOOST_CHECK(true) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}

	BOOST_AUTO_TEST_CASE( AddRecord )
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_record(mem, "dummy", "dummy") ;

			BOOST_CHECK_EQUAL( 1u, mem.size() ) ;
			BOOST_CHECK_EQUAL( false, mem.empty() ) ;

			mem.m_engine.method(L"Delete") ;
			BOOST_CHECK(true) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}
	BOOST_AUTO_TEST_CASE( add_by_id_exists )
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;
			add_record(mem, "dummy", "dummy") ;

			BOOST_CHECK_EQUAL( 1u, mem.size() ) ;

			record_pointer record = mem.add_by_id(1, L"source", L"trans") ;
			BOOST_CHECK_EQUAL( 1u, mem.size() ) ;

			BOOST_CHECK_EQUAL((int)record->get_id(), 1) ;
			BOOST_CHECK(L"source" == record->get_source_rich()) ;
			BOOST_CHECK(L"trans" == record->get_trans_rich()) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}


	BOOST_AUTO_TEST_CASE(GetMatchesSize1)
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, "I luv spam", "Yes I do") ;

			trans_match_container matches ;
			search_query_params params ;
			params.m_rich_source = L"I luv spam" ;
			params.m_source = L"I luv spam" ;

			mem.find_matches(matches, params) ;

			BOOST_CHECK_EQUAL(1u, matches.size()) ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}

	// from bug found with increment_refcount
	BOOST_AUTO_TEST_CASE(increment_refcount)
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
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

			BOOST_CHECK_EQUAL(0u, record->get_refcount()) ;
			record->increment_refcount() ;
			BOOST_CHECK_EQUAL(1u, record->get_refcount()) ;

			mem.m_engine.method(L"Delete") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}



	BOOST_AUTO_TEST_CASE(GetMatchesMarkup)
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, L"baab", L"baab") ;

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

			mem.m_engine.method(L"Delete") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}


	// trans lookup
	BOOST_AUTO_TEST_CASE(GetTransMatchesMarkupWordAlgoIgnoreCaseQuery)
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

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
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}


	// get_best_match_score

	BOOST_AUTO_TEST_CASE(test_get_best_match_score_0_5)
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, L"aaaa", L"aaaa") ;
			add_hit(mem, L"bbbb", L"bbbb") ;
			add_hit(mem, L"cccc", L"cccc") ;

			wstring query = L"aadd" ;
			BOOST_CHECK_CLOSE((double)0.5f, mem.get_best_match_score(query), 0.001) ;

			mem.m_engine.method(L"Delete") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}
	}
	// get_glossary_matches


	BOOST_AUTO_TEST_CASE(test_get_glossary_matches_50)
	{
		try
		{
			app_props::properties_memory props ;
			memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, L"eggs", L"trans") ;

			search_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"spam, egg, sausage and spam" ;
			params.m_source = L"spam, egg, sausage and spam" ;
			params.m_ignore_case = true ;

			app_props::properties_glossary glossprops ;
			glossprops.m_data.m_min_score = 50 ;
			mem.set_properties_glossary(&glossprops) ;

			mem.get_glossary_matches(matches, params) ;
			BOOST_CHECK_EQUAL(1u, matches.size()) ;
			search_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			string expected = "<span class=\"partial_match1\">eggs</span>" ;
			string actual = string2string(match->get_markup()->GetSource()).c_str() ;
			BOOST_CHECK_EQUAL(expected, actual) ;
			BOOST_CHECK(match->get_markup()->GetTrans() == L"trans") ;
		}
		catch (_com_error& e)
		{
			TRACE(e.Description()) ;
			TRACE(e.ErrorMessage()) ;
			TRACE(e.Error()) ;
			CStringA msg(static_cast< LPCWSTR >( e.ErrorMessage() )) ;
			BOOST_FAIL(static_cast< LPCSTR >( msg )) ;
		}

	}

	BOOST_AUTO_TEST_CASE(is_local)
	{
		app_props::properties_memory props ;
		memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;
		BOOST_CHECK(! mem.is_local()) ;
	}



	// get matches
	BOOST_AUTO_TEST_CASE(make_match)
	{
		app_props::properties_memory props ;
		memory_remote mem(&props, .5, L"Felix.RemoteMemoryFake") ;

		search_match_ptr match = mem.make_match() ;
		BOOST_CHECK_EQUAL(match->get_memory_id(), mem.get_id()) ;
		BOOST_CHECK_EQUAL(match->get_memory_location(), L"Fake Memory") ;
	}


BOOST_AUTO_TEST_SUITE_END()