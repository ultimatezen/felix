#include "StdAfx.h"
#include "TranslationMemory.h"
#include "StringEx.h"
#include "record_local.h"

#include <boost/test/unit_test.hpp>

#define DELCARE_MATCH( match_name, source, trans) \
	search_match_ptr match_name(new search_match) ;\
	{\
	record_pointer rec(new record_local()) ;\
	rec->set_source( wstring(source) ) ;\
	rec->set_trans( wstring(trans) ) ;\
	match_name->set_record( rec ) ;\
	}

BOOST_AUTO_TEST_SUITE( match_makerTestCase )

	using namespace mem_engine ;
	const static wstring nomatch_tag_open1(L"<span class=\"nomatch\">") ;
	const static wstring nomatch_tag_open2(L"<span class=\"partial_match1\">") ;
	const static wstring nomatch_tag_open3(L"<span class=\"partial_match2\">") ;
	const static wstring nomatch_tag_close(L"</span>") ;

	BOOST_AUTO_TEST_CASE( test_has_asian_yes )
	{
		BOOST_CHECK(mem_engine::has_asian(L"自動車に乗る")) ;
		BOOST_CHECK(mem_engine::has_asian(L"This has 一つ j-word")) ;
	}
	BOOST_AUTO_TEST_CASE( test_has_asian_no )
	{
		BOOST_CHECK(! mem_engine::has_asian(L"This has no j-words?")) ;
		BOOST_CHECK(! mem_engine::has_asian(L"")) ;
	}
	BOOST_AUTO_TEST_CASE( test_has_spaces_yes )
	{
		BOOST_CHECK(mem_engine::has_spaces(L"Gits in the car!")) ;
		BOOST_CHECK(mem_engine::has_spaces(L"This has no j-words")) ;
	}
	BOOST_AUTO_TEST_CASE( test_has_spaces_no )
	{
		BOOST_CHECK(! mem_engine::has_spaces(L"snurfle")) ;
		BOOST_CHECK(! mem_engine::has_spaces(L"")) ;
	}
	BOOST_AUTO_TEST_CASE( detect_match_algo_char_algo )
	{
		BOOST_CHECK(IDC_ALGO_CHAR == mem_engine::detect_match_algo(L"snurfle")) ;
		BOOST_CHECK(IDC_ALGO_CHAR == mem_engine::detect_match_algo(L"自動車に乗る")) ;
	}
	BOOST_AUTO_TEST_CASE( detect_match_algo_word_algo )
	{
		BOOST_CHECK(IDC_ALGO_WORD == mem_engine::detect_match_algo(L"Gits in the car!")) ;
		BOOST_CHECK(IDC_ALGO_WORD == mem_engine::detect_match_algo(L"foo (bar)")) ;
	}

	BOOST_AUTO_TEST_CASE( Trivial )
	{
		match_maker mm( 0.5f ) ;

		BOOST_CHECK_CLOSE( 0.5, mm.get_minimum_score(), 0.0001 ) ;
	}

	BOOST_AUTO_TEST_CASE( TestComposeMarkup )
	{
		match_maker mm( 0.5f ) ;

		std::list< wstring > elements ;
		elements.push_back(L"one") ;
		elements.push_back(L"two") ;
		elements.push_back(L"three") ;

		BOOST_CHECK_EQUAL( L"onetwothree", mm.compose_markup_string( elements ) ) ;
	}

	BOOST_AUTO_TEST_CASE( GetGlossMarkup )
	{
		match_maker mm( 0.5f ) ;

		BOOST_CHECK_EQUAL( nomatch_tag_open1, mm.get_gloss_markup_start( 0.91f ) ) ;
		BOOST_CHECK_EQUAL( nomatch_tag_open2, mm.get_gloss_markup_start( 0.89f ) ) ;
		BOOST_CHECK_EQUAL( nomatch_tag_open3, mm.get_gloss_markup_start( 0.3f ) ) ;
		BOOST_CHECK_EQUAL( nomatch_tag_close, mm.get_gloss_markup_end( ) ) ;
	}

	BOOST_AUTO_TEST_CASE( SimpleMatch )
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"aaa", L"aaa") ;

		Segment query; query.set_value(L"aaa") ;
		Segment source; source.set_value(L"aaa") ;
		mm.get_score( query, source, IDC_ALGO_CHAR, match ) ;

		BOOST_CHECK_CLOSE( 1.0, match->get_score(), 0.00001 ) ;

		CStringA expected = "aaa" ;
		CStringA actual = match->get_markup()->GetSource().c_str() ;

		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "aaa" ;
		actual = match->get_markup()->GetQuery().c_str() ;

		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( SimpleNoMatch )
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"baab", L"baab") ;

		Segment query; query.set_value(L"aa") ;
		Segment source; source.set_value(L"baab") ;
		mm.get_score( query, source, IDC_ALGO_CHAR, match ) ;

		BOOST_CHECK_CLOSE( 0.5, match->get_score(), 0.00001 ) ;

		CStringA expected = "<span class=\"nomatch\">b</span>aa<span class=\"nomatch\">b</span>" ;

		CStringA actual = match->get_markup()->GetSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "aa" ;
		actual = match->get_markup()->GetQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( WordAlgoSimple)
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"I love spam and eggs.", L"Nailed to the perch.") ;

		Segment query; query.set_value(L"I love ham and eggs.") ;
		Segment source; source.set_value(L"I love spam and eggs.") ;
		mm.get_score( query, source, IDC_ALGO_WORD, match ) ;

		string actual = CStringA(match->get_markup()->GetQuery().c_str()) ;
		string expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	/************************************************************************/
	/* tests for fuzzy glossary matches                                     */
	/************************************************************************/

	BOOST_AUTO_TEST_SUITE( match_maker_test )

	using namespace mem_engine ;
const static wstring nomatch_tag_open1(L"<span class=\"nomatch\">") ;
const static wstring nomatch_tag_open2(L"<span class=\"partial_match1\">") ;
const static wstring nomatch_tag_open3(L"<span class=\"partial_match2\">") ;
const static wstring nomatch_tag_close(L"</span>") ;

	BOOST_AUTO_TEST_CASE( fuzzy_gloss_kata_100)
	{
		match_maker mm( 0.1f ) ;

		Segment query; query.set_value(L"ベーコン") ;
		Segment source; source.set_value(L"豚がベーコンになる。") ;
		DELCARE_MATCH(match, L"I love spam and eggs.", L"Nailed to the perch.") ;

		BOOST_CHECK( mm.fuzzy_gloss_score(query, source, match)) ;
		BOOST_CHECK_CLOSE(1.0, match->get_score(), 0.00001) ;
	}
	BOOST_AUTO_TEST_CASE( fuzzy_gloss_kata_50)
	{
		match_maker mm( 0.1f ) ;

		Segment query; query.set_value(L"ベーコン") ;
		Segment source; source.set_value(L"豚がベーになる。") ;
		DELCARE_MATCH(match, L"I love spam and eggs.", L"Nailed to the perch.") ;

		BOOST_CHECK(mm.fuzzy_gloss_score(query, source, match)) ;

		BOOST_CHECK_CLOSE(0.5, match->get_score(), 0.00001) ;
	}


BOOST_AUTO_TEST_SUITE_END()
