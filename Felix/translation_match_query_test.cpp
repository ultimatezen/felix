
#include "StdAfx.h"
#include "query.h"
#include "TranslationMemory.h"
#include "easyunit/testharness.h"
#include "File.h"
#include "record_local.h"
#include "memory_local.h"

/************************************************************************/
/* Unit Tests                                                           */
/************************************************************************/

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
#define ADD_RECORD(mem, source, trans) \
	{\
	record_pointer rec(new record_local()) ;\
	rec->set_source(wstring(source)) ;\
	rec->set_trans(wstring(trans)) ;\
	mem.add_record(rec) ;\
	}

#define GET_MATCHES(mem, query, match_query)\
	trans_match_container matches ;\
	\
	params.m_rich_source = query ;\
	params.m_source = query ;\
	mem.find_matches(matches, params) ;\
	\
	match_query.set_matches(matches) ;

	TEST( translation_match_queryTestCase, get_html_short_twice )
	{
		memory_local mem(0.0f) ;
		ADD_RECORD(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

		translation_match_query match_query ;
		search_query_params params ;
		GET_MATCHES(mem, L"I love spam and eggs.", match_query) ;

		string first = string2string(match_query.get_html_short()) ;
		string second = string2string(match_query.get_html_short()) ;

		SimpleString err_msg = SimpleString(first.c_str()) +
			SimpleString("\n\n") +
			SimpleString(second.c_str()) ;
		ASSERT_EQUALS_M(first, second, err_msg) ;
	}
	TEST( translation_match_queryTestCase, get_html_short_word )
	{
		memory_local mem(0.0f) ;
		ADD_RECORD(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

		translation_match_query match_query ;
		search_query_params params ;
		params.m_match_algo = IDC_ALGO_WORD ;
		GET_MATCHES(mem, L"I love spam and eggs.", match_query) ;

		string actual = string2string(match_query.get_html_short()) ;
		ASSERT_TRUE(actual.find("Nailed to the perch") != string::npos) ;
		ASSERT_TRUE(actual.find("I love") != string::npos) ;
		ASSERT_TRUE(actual.find("and eggs") != string::npos) ;
		ASSERT_TRUE(actual.find("ham") != string::npos) ;
		ASSERT_TRUE(actual.find("spam") != string::npos) ;
	}
	TEST( translation_match_queryTestCase, get_html_short_word_twice )
	{
		memory_local mem(0.0f) ;
		ADD_RECORD(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

		translation_match_query match_query ;
		search_query_params params ;
		params.m_match_algo = IDC_ALGO_WORD ;
		GET_MATCHES(mem, L"I love spam and eggs.", match_query) ;

		string first = string2string(match_query.get_html_short()) ;
		string second = string2string(match_query.get_html_short()) ;

		SimpleString err_msg = SimpleString(first.c_str()) +
			SimpleString("\n\n") +
			SimpleString(second.c_str()) ;
		ASSERT_EQUALS_M(first, second, err_msg) ;
	}
}


#endif // #ifdef _DEBUG
