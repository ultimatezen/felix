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

	class CMockListener : public CProgressListener
	{
	public:
		void sense(string value)
		{
			m_calls += value ;
		}
		std::vector< string > m_calls ;
		void OnProgressInit( const CString &/*file_name*/, size_t /*min_val*/, size_t /*max_val*/ )
		{
			sense("OnProgressInit") ;
		}
		bool OnProgressLoadUpdate( size_t /*current_val*/ ) 
		{
			sense("OnProgressLoadUpdate") ;
			return true ;
		}
		bool OnProgressWriteUpdate( size_t /*current_val*/ ) 
		{
			sense("OnProgressWriteUpdate") ;
			return true ;
		}
		void OnProgressDoneWrite( size_t /*final_val*/ ) 
		{
			sense("OnProgressDoneWrite") ;
		}
		void OnProgressDoneLoad( size_t /*final_val*/ )
		{
			sense("OnProgressDoneLoad") ;
		}
		virtual bool RecoverFromExceptions()
		{
			sense("RecoverFromExceptions") ;
			return false ;
		}
	} ;

	TEST( test_memory_remote, MemoryId )
	{
		memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

		ASSERT_TRUE_M(mem.get_id() > 0, ulong2string(mem.get_id()).c_str()) ;
	}
	TEST( test_memory_remote, save_memory )
	{
		memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

		ASSERT_TRUE(mem.save_memory()) ;
	}
	TEST( test_memory_remote, is_new ) 
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			ASSERT_TRUE ( ! mem.is_new() ) ; 
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
	TEST( test_memory_remote, add_by_id_doesnt )
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;


			record_pointer record = mem.add_by_id(1, L"source", L"trans") ;
			ASSERT_EQUALS( 1u, mem.size() ) ;

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

	TEST( test_memory_remote, add_record_then_modify_reliability )
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

			record_pointer rec(new record_remote(L"Felix.RemoteRecordFake")) ;
			rec->set_source(L"a luva luva") ;
			rec->set_trans(L"hey!") ;
			ASSERT_EQUALS_V( 0, static_cast<int>(rec->get_id())) ;

			mem.add_record(rec) ;

			SimpleString err_msg = ulong2string(rec->get_id()).c_str() ;
			ASSERT_TRUE_M( static_cast<int>(rec->get_id()) > 0, err_msg) ;
			ASSERT_EQUALS_V( 0, static_cast<int>(rec->get_reliability())) ;

			trans_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"a luva luva" ;
			params.m_source = L"a luva luva" ;

			add_hit(mem, "a luva luva", "a luva luva") ;

			mem.find_matches(matches, params) ;
			ASSERT_EQUALS(1, matches.size()) ;

			search_match_ptr match = *(matches.begin()) ;
			match->get_record()->set_reliability(4u) ;
			ASSERT_EQUALS_V( 4, static_cast<int>(match->get_record()->get_reliability())) ;

			matches.clear() ;
			mem.find_matches(matches, params) ;
			ASSERT_EQUALS(1, matches.size()) ;

			match = *(matches.begin()) ;
			ASSERT_EQUALS_V( 4, static_cast<int>(match->get_record()->get_reliability())) ;
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
	TEST( test_memory_remote, ExtraStrings )
	{
		memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
		ASSERT_EQUALS( L"", mem.get_extra_string(L"foo") ) ;

		mem.set_extra_string( L"foo", L"bar" ) ;
		ASSERT_EQUALS( L"bar", mem.get_extra_string(L"foo") ) ;
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


	TEST(test_memory_remote, GetMatchesSize2)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, "I luv spam", "Yes I do") ;
			add_hit(mem, "I love spam", "Yes I do") ;

			trans_match_container matches ;
			search_query_params params ;
			params.m_rich_source = L"I luv spam" ;
			params.m_source = L"I luv spam" ;

			mem.find_matches(matches, params) ;

			ASSERT_EQUALS(2, matches.size()) ;

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

	TEST(test_memory_remote, GetMatchesMarkupWordAlgo)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

			trans_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"I love spam and eggs." ;
			params.m_source = L"I love spam and eggs." ;
			params.m_match_algo = IDC_ALGO_WORD ;

			mem.find_matches(matches, params) ;
			ASSERT_EQUALS(1, matches.size()) ;

			trans_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			SimpleString expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
			SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

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



	TEST(test_memory_remote, GetMatchesMarkupAutoAlgoIgnoreCasePerfect)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;

			add_hit(mem, L"I love Ham and Eggs.", L"Nailed to the perch.") ;

			trans_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"I love Ham and Eggs." ;
			params.m_source = L"I love Ham and Eggs." ;
			params.m_match_algo = IDC_ALGO_AUTO ;
			params.m_ignore_case = true ;

			mem.find_matches(matches, params) ;
			ASSERT_EQUALS_V(1, (int)matches.size()) ;

			trans_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			SimpleString expected = "I love Ham and Eggs." ;
			SimpleString actual = CStringA(match->get_markup()->GetSource().c_str()) ;

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
	TEST(test_memory_remote, test_get_best_match_score_1_0)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, L"spam", L"spam") ;
			add_hit(mem, L"eggs", L"eggs") ;
			add_hit(mem, L"sausage", L"sausage") ;

			wstring query = L"spam" ;
			ASSERT_EQUALS_DELTA_V(1.0f, mem.get_best_match_score(query), 0.001) ;

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
	TEST(test_memory_remote, test_get_glossary_matches_100)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			add_hit(mem, L"egg", L"trans") ;

			search_match_container matches ;
			search_query_params params ;

			params.m_rich_source = L"spam, egg, sausage and spam" ;
			params.m_source = L"spam, egg, sausage and spam" ;
			params.m_ignore_case = true ;

			app_props::properties_glossary props ;
			props.m_data.m_min_score = 100 ;
			mem.set_gloss_props(props) ;

			mem.get_glossary_matches(matches, params) ;
			ASSERT_EQUALS_V(1, (int)matches.size()) ;
			search_match_container::iterator pos = matches.begin() ;
			search_match_ptr match = *pos ;

			const SimpleString actual_source = string2string(match->get_markup()->GetSource()).c_str() ;
			ASSERT_EQUALS_V("egg", actual_source) ;
			const SimpleString actual_trans = string2string(match->get_markup()->GetTrans()).c_str() ;
			ASSERT_EQUALS_V("trans", actual_trans) ;

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

	// concordance
	TEST(test_memory_remote, concordance)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			search_match_container matches ;
			search_query_params params ;

			params.m_source = L"ham" ;
			params.m_ignore_case = true ;
			params.m_ignore_width = true ;
			params.m_ignore_hira_kata =	true ;
			params.m_use_regex = false ;


			add_record(mem, "I love ham and eggs.", "Nailed to the perch.") ;

			params.m_source = L"zzz" ;
			params.m_ignore_case = true ;
			params.m_ignore_width = true ;
			params.m_ignore_hira_kata =	true ;
			params.m_use_regex = false ;

			// one record but no match
			mem.perform_search( matches, params ) ;
			ASSERT_TRUE(matches.empty()) ;

			// this will hold our matches
			params.m_source = L"ham" ;
			params.m_ignore_case = true ;
			params.m_ignore_width = true ;
			params.m_ignore_hira_kata =	true ;
			params.m_use_regex = false ;

			add_hit(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;
			// one match
			mem.perform_search( matches, params ) ;
			ASSERT_EQUALS_V(1, (int)matches.size()) ;

			search_match_ptr match = *(matches.begin()) ;
			ASSERT_EQUALS(match->get_record()->get_source_rich(), L"I love ham and eggs.") ;
			ASSERT_EQUALS_V((int)match->get_memory_id(), (int)mem.get_id()) ;


			match = *(matches.begin()) ;
			ASSERT_EQUALS(match->get_record()->get_trans_rich(), L"Nailed to the perch.") ;

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

	// reverse concordance

	TEST(test_memory_remote, rconcordance)
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
			mem.m_engine.method(L"Create", L"spam", L"m") ;

			search_match_container matches ;
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


			// this will hold our matches
			params.m_trans = L"perch" ;
			params.m_ignore_case = true ;
			params.m_ignore_width = true ;
			params.m_ignore_hira_kata =	true ;
			params.m_use_regex = false ;

			add_hit(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;
			// one match
			mem.perform_search( matches, params ) ;
			ASSERT_EQUALS_V(1, (int)matches.size()) ;

			search_match_ptr match = *(matches.begin()) ;
			ASSERT_EQUALS(match->get_record()->get_trans_rich(), L"Nailed to the perch.") ;

			match = *(matches.begin()) ;
			ASSERT_EQUALS(match->get_record()->get_trans_rich(), L"Nailed to the perch.") ;

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

	TEST(test_memory_remote, is_local)
	{
		memory_remote mem(5.0, L"Felix.RemoteMemoryFake") ;
		ASSERT_TRUE(! mem.is_local()) ;
	}

	TEST(test_memory_remote, connect)
	{
		try
		{

			memory_remote mem(5.0, L"Felix.RemoteMemoryFake"); 

			ASSERT_TRUE(! mem.connect("spam")) ;
			FAIL_M("Should throw on bad connection string") ;
		}
		catch(CException &e)
		{
			e ;
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

	TEST( test_memory_remote, erase )
	{
		try
		{
			memory_remote mem(5.0, L"Felix.RemoteMemoryFake"); 

			ASSERT_TRUE(mem.connect("http://192.168.3.5:8765/api/mems/1/")) ;

			int orig_size = static_cast< int >( mem.size() ) ;

			record_pointer rec(new record_local()) ;
			rec->set_source(L"spam xxvi") ;
			rec->set_trans(L"egg") ;

			mem.add_record(rec) ;

			ASSERT_EQUALS_V( orig_size+1, static_cast< int >( mem.size() ) ) ;
			ASSERT_EQUALS( false, mem.empty() ) ;

			mem.erase(rec) ;
			ASSERT_EQUALS_V( orig_size, static_cast< int >( mem.size() ) ) ;
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

}


#endif // #ifdef UNIT_TEST
