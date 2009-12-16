#include "StdAfx.h"
#include "TranslationMemory.h"
#include "StringEx.h"
#include "record_local.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	using namespace mem_engine ;
	const static wstring nomatch_tag_open1(L"<span class=\"nomatch\">") ;
	const static wstring nomatch_tag_open2(L"<span class=\"partial_match1\">") ;
	const static wstring nomatch_tag_open3(L"<span class=\"partial_match2\">") ;
	const static wstring nomatch_tag_close(L"</span>") ;

#define DELCARE_MATCH( match_name, source, trans) \
	search_match_ptr match_name(new search_match) ;\
	{\
		record_pointer rec(new record_local()) ;\
		rec->set_source( wstring(source) ) ;\
		rec->set_trans( wstring(trans) ) ;\
		match_name->set_record( rec ) ;\
	}

	TEST( test_has_asian, yes )
	{
		ASSERT_TRUE(mem_engine::has_asian(L"自動車に乗る")) ;
		ASSERT_TRUE(mem_engine::has_asian(L"This has 一つ j-word")) ;
	}
	TEST( test_has_asian, no )
	{
		ASSERT_TRUE(! mem_engine::has_asian(L"This has no j-words?")) ;
		ASSERT_TRUE(! mem_engine::has_asian(L"")) ;
	}
	TEST( test_has_spaces, yes )
	{
		ASSERT_TRUE(mem_engine::has_spaces(L"Gits in the car!")) ;
		ASSERT_TRUE(mem_engine::has_spaces(L"This has no j-words")) ;
	}
	TEST( test_has_spaces, no )
	{
		ASSERT_TRUE(! mem_engine::has_spaces(L"snurfle")) ;
		ASSERT_TRUE(! mem_engine::has_spaces(L"")) ;
	}
	TEST( detect_match_algo, char_algo )
	{
		ASSERT_TRUE(IDC_ALGO_CHAR == mem_engine::detect_match_algo(L"snurfle")) ;
		ASSERT_TRUE(IDC_ALGO_CHAR == mem_engine::detect_match_algo(L"自動車に乗る")) ;
	}
	TEST( detect_match_algo, word_algo )
	{
		ASSERT_TRUE(IDC_ALGO_WORD == mem_engine::detect_match_algo(L"Gits in the car!")) ;
		ASSERT_TRUE(IDC_ALGO_WORD == mem_engine::detect_match_algo(L"foo (bar)")) ;
	}

	TEST( match_makerTestCase, Trivial )
	{
		match_maker mm( 0.5f ) ;

		ASSERT_EQUALS_DELTA( 0.5f, mm.get_minimum_score(), 0.0001 ) ;
	}

	TEST( match_makerTestCase, TestComposeMarkup )
	{
		match_maker mm( 0.5f ) ;

		std::list< wstring > elements ;
		elements.push_back(L"one") ;
		elements.push_back(L"two") ;
		elements.push_back(L"three") ;

		ASSERT_EQUALS( L"onetwothree", mm.compose_markup_string( elements ) ) ;
	}

	TEST( match_makerTestCase, GetGlossMarkup )
	{
		match_maker mm( 0.5f ) ;

		ASSERT_EQUALS( nomatch_tag_open1, mm.get_gloss_markup_start( 0.91f ) ) ;
		ASSERT_EQUALS( nomatch_tag_open2, mm.get_gloss_markup_start( 0.89f ) ) ;
		ASSERT_EQUALS( nomatch_tag_open3, mm.get_gloss_markup_start( 0.3f ) ) ;
		ASSERT_EQUALS( nomatch_tag_close, mm.get_gloss_markup_end( ) ) ;
	}

	TEST( match_makerTestCase, SimpleMatch )
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"aaa", L"aaa") ;

		Segment query; query.set_value(L"aaa") ;
		Segment source; source.set_value(L"aaa") ;
		mm.get_score( query, source, IDC_ALGO_CHAR, match ) ;

		ASSERT_EQUALS_DELTA( 1.0f, match->get_score(), 0.00001 ) ;

		CStringA expected = "aaa" ;
		CStringA actual = match->get_markup()->GetSource().c_str() ;

		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "aaa" ;
		actual = match->get_markup()->GetQuery().c_str() ;

		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( match_makerTestCase, SimpleNoMatch )
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"baab", L"baab") ;

		Segment query; query.set_value(L"aa") ;
		Segment source; source.set_value(L"baab") ;
		mm.get_score( query, source, IDC_ALGO_CHAR, match ) ;

		ASSERT_EQUALS_DELTA( 0.5f, match->get_score(), 0.00001 ) ;

		CStringA expected = "<span class=\"nomatch\">b</span>aa<span class=\"nomatch\">b</span>" ;

		CStringA actual = match->get_markup()->GetSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "aa" ;
		actual = match->get_markup()->GetQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST(match_makerTest, WordAlgoSimple)
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"I love spam and eggs.", L"Nailed to the perch.") ;

		Segment query; query.set_value(L"I love ham and eggs.") ;
		Segment source; source.set_value(L"I love spam and eggs.") ;
		mm.get_score( query, source, IDC_ALGO_WORD, match ) ;

		SimpleString actual = CStringA(match->get_markup()->GetQuery().c_str()) ;
		SimpleString expected = "I love <span class=\"nomatch\">ham</span> and eggs." ;
		ASSERT_EQUALS(actual, expected) ;
	}

	/************************************************************************/
	/* tests for fuzzy glossary matches                                     */
	/************************************************************************/
	TEST(match_maker_test, fuzzy_gloss_kata_100)
	{
		match_maker mm( 0.1f ) ;

		Segment query; query.set_value(L"ベーコン") ;
		Segment source; source.set_value(L"豚がベーコンになる。") ;
		DELCARE_MATCH(match, L"I love spam and eggs.", L"Nailed to the perch.") ;

		bool result = mm.fuzzy_gloss_score(query, source, match) ;
		ASSERT_TRUE_M(result, "A match should have been found") ;

		ASSERT_EQUALS_DELTA_V(1.0, match->get_score(), 0.00001) ;
	}
	TEST(match_maker_test, fuzzy_gloss_kata_50)
	{
		match_maker mm( 0.1f ) ;

		Segment query; query.set_value(L"ベーコン") ;
		Segment source; source.set_value(L"豚がベーになる。") ;
		DELCARE_MATCH(match, L"I love spam and eggs.", L"Nailed to the perch.") ;

		bool result = mm.fuzzy_gloss_score(query, source, match) ;
		ASSERT_TRUE_M(result, "A match should have been found") ;

		ASSERT_EQUALS_DELTA_V(0.5, match->get_score(), 0.00001) ;
	}


}

#endif 
