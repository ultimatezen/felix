
#include "StdAfx.h"
#include "query.h"
#include "TranslationMemory.h"
#include "File.h"
#include "record_local.h"
#include "memory_local.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( translation_match_queryTestCase )


	using namespace mem_engine ;
	using namespace app_props ;
	
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

	BOOST_AUTO_TEST_CASE( get_html_short_twice )
	{
		memory_local mem(get_props(), 0.0f) ;
		ADD_RECORD(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

		translation_match_query match_query ;
		search_query_params params ;
		GET_MATCHES(mem, L"I love spam and eggs.", match_query) ;

		string first = string2string(match_query.get_html_short()) ;
		string second = string2string(match_query.get_html_short()) ;

		BOOST_CHECK_EQUAL(first, second) ;
	}
	BOOST_AUTO_TEST_CASE( get_html_short_word )
	{
		memory_local mem(get_props(), 0.0f) ;
		ADD_RECORD(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

		translation_match_query match_query ;
		search_query_params params ;
		params.m_match_algo = IDC_ALGO_WORD ;
		GET_MATCHES(mem, L"I love spam and eggs.", match_query) ;

		string actual = string2string(match_query.get_html_short()) ;
		BOOST_CHECK(actual.find("Nailed to the perch") != string::npos) ;
		BOOST_CHECK(actual.find("I love") != string::npos) ;
		BOOST_CHECK(actual.find("and eggs") != string::npos) ;
		BOOST_CHECK(actual.find("ham") != string::npos) ;
		BOOST_CHECK(actual.find("spam") != string::npos) ;
	}
	BOOST_AUTO_TEST_CASE( get_html_short_word_twice )
	{
		memory_local mem(get_props(), 0.0f) ;
		ADD_RECORD(mem, L"I love ham and eggs.", L"Nailed to the perch.") ;

		translation_match_query match_query ;
		search_query_params params ;
		params.m_match_algo = IDC_ALGO_WORD ;
		GET_MATCHES(mem, L"I love spam and eggs.", match_query) ;

		string first = string2string(match_query.get_html_short()) ;
		string second = string2string(match_query.get_html_short()) ;

		BOOST_CHECK_EQUAL(first, second) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef _DEBUG
