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
		BOOST_CHECK(has_asian(L"自動車に乗る")) ;
		BOOST_CHECK(has_asian(L"This has 一つ j-word")) ;
	}
	BOOST_AUTO_TEST_CASE( test_has_asian_no )
	{
		BOOST_CHECK(! has_asian(L"This has no j-words?")) ;
		BOOST_CHECK(! has_asian(L"")) ;
	}
	BOOST_AUTO_TEST_CASE( test_has_spaces_yes )
	{
		BOOST_CHECK(has_spaces(L"Gits in the car!")) ;
		BOOST_CHECK(has_spaces(L"This has no j-words")) ;
	}
	BOOST_AUTO_TEST_CASE( test_has_spaces_no )
	{
		BOOST_CHECK(! has_spaces(L"snurfle")) ;
		BOOST_CHECK(! has_spaces(L"")) ;
	}
	BOOST_AUTO_TEST_CASE( detect_match_algo_char_algo )
	{
		BOOST_CHECK(IDC_ALGO_CHAR == detect_match_algo(L"snurfle")) ;
		BOOST_CHECK(IDC_ALGO_CHAR == detect_match_algo(L"自動車に乗る")) ;
	}
	BOOST_AUTO_TEST_CASE( detect_match_algo_word_algo )
	{
		BOOST_CHECK(IDC_ALGO_WORD == detect_match_algo(L"Gits in the car!")) ;
		BOOST_CHECK(IDC_ALGO_WORD == detect_match_algo(L"foo (bar)")) ;
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
	BOOST_AUTO_TEST_CASE( SimpleNoMatchFailSoon )
	{
		match_maker mm( 0.9f ) ;

		DELCARE_MATCH(match, L"baab", L"baab") ;

		Segment query; query.set_value(L"xxxx") ;
		Segment source; source.set_value(L"baab") ;
		BOOST_CHECK( ! mm.get_score( query, source, IDC_ALGO_CHAR, match ) ) ;

		BOOST_CHECK_CLOSE( 0.0, match->get_score(), 0.00001 ) ;
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
	BOOST_AUTO_TEST_CASE(WordAlgoSourceLong)
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"aaa bbb ccc ddd ", L"zzz 111") ;

		Segment query; query.set_value(L"aaa bbb ccc ") ;
		Segment source; source.set_value(L"aaa bbb ccc ddd ") ;
		mm.get_score( query, source, IDC_ALGO_WORD, match ) ;

		BOOST_CHECK_CLOSE(match->get_score(), 0.75, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE(WordAlgoQueryLong)
	{
		match_maker mm( 0.1f ) ;

		DELCARE_MATCH(match, L"aaa bbb ccc ddd ", L"zzz 111") ;

		Segment query; query.set_value(L"aaa bbb ccc ddd ") ;
		Segment source; source.set_value(L"aaa bbb ccc ") ;
		mm.get_score( query, source, IDC_ALGO_WORD, match ) ;

		BOOST_CHECK_CLOSE(match->get_score(), 0.75, 0.0001) ;
	}

	// get_tags
	BOOST_AUTO_TEST_CASE(get_tags_none)
	{
		match_maker mm( 0.1f ) ;

		wstring text = L"aaa" ;
		std::multiset<wstring> tags ;

		mm.get_tags(text, tags) ;

		BOOST_CHECK_EQUAL(0u, tags.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_tags_one)
	{
		match_maker mm( 0.1f ) ;

		wstring text = L"<b>aaa</b>" ;
		std::multiset<wstring> tags ;

		mm.get_tags(text, tags) ;

		BOOST_CHECK_EQUAL(1u, tags.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_tags_two)
	{
		match_maker mm( 0.1f ) ;

		wstring text = L"<font color=\"red\"><font face=\"Courier\">aaa</font></font>" ;
		std::multiset<wstring> tags ;

		mm.get_tags(text, tags) ;

		BOOST_CHECK_EQUAL(2u, tags.size()) ;
	}

	// get_format_penalty
	BOOST_AUTO_TEST_CASE(get_format_penalty_none)
	{
		match_maker mm( 0.1f ) ;

		wstring row = L"aaa" ;
		wstring col = L"bbb" ;

		double actual = mm.get_format_penalty(row, col) ;

		BOOST_CHECK_CLOSE(actual, 0.0, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE(get_format_penalty_one_row)
	{
		match_maker mm( 0.1f ) ;

		wstring row = L"<font face=\"courier\">aaa</font>" ;
		wstring col = L"bbb" ;

		double actual = mm.get_format_penalty(row, col) ;

		BOOST_CHECK_CLOSE(actual, 0.01, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE(get_format_penalty_one_col)
	{
		match_maker mm( 0.1f ) ;

		wstring row = L"bbb" ;
		wstring col = L"<font face=\"courier\">aaa</font>" ;

		double actual = mm.get_format_penalty(row, col) ;

		BOOST_CHECK_CLOSE(actual, 0.01, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE(get_format_penalty_one_each)
	{
		match_maker mm( 0.1f ) ;

		wstring row = L"<b>bbb</b>" ;
		wstring col = L"<font face=\"courier\">aaa</font>" ;

		double actual = mm.get_format_penalty(row, col) ;

		BOOST_CHECK_CLOSE(actual, 0.02, 0.0001) ;
	}
	BOOST_AUTO_TEST_CASE(get_format_penalty_one_each_one_match)
	{
		match_maker mm( 0.1f ) ;

		wstring row = L"<i><b>bbb</b></i>" ;
		wstring col = L"<font face=\"courier\"><i>aaa</i></font>" ;

		double actual = mm.get_format_penalty(row, col) ;

		BOOST_CHECK_CLOSE(actual, 0.02, 0.0001) ;
	}
	
BOOST_AUTO_TEST_SUITE_END()

/************************************************************************/
/* tests for fuzzy glossary matches                                     */
/************************************************************************/

BOOST_AUTO_TEST_SUITE( match_maker_fuzzy_gloss_test )

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
	// pass_minimum_tests
	BOOST_AUTO_TEST_CASE(pass_minimum_tests_true)
	{
		match_maker mm( 0.5f ) ;
		Segment col, row ;
		col.set_value(L"1234567890") ;
		row.set_value(L"12345678") ;
		mm.set_row_col_strings(row, col) ;
		BOOST_CHECK(mm.pass_minimum_tests()) ;
	}
	BOOST_AUTO_TEST_CASE(pass_minimum_tests_false)
	{
		match_maker mm( 0.5f ) ;
		Segment col, row ;
		col.set_value(L"1234567890") ;
		row.set_value(L"1234") ;
		mm.set_row_col_strings(row, col) ;
		BOOST_CHECK(! mm.pass_minimum_tests()) ;
	}
	// compute cost
	BOOST_AUTO_TEST_CASE(compute_cost_0)
	{
		match_maker mm( 0.1f ) ;
		BOOST_CHECK_EQUAL(mm.compute_cost(L'a', L'a'), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(compute_cost_1)
	{
		match_maker mm( 0.1f ) ;
		BOOST_CHECK_EQUAL(mm.compute_cost(L'a', L'z'), 1u) ;
	}
	// calculate score
	BOOST_AUTO_TEST_CASE(calculate_score_1)
	{
		match_maker mm( 0.1f ) ;
		BOOST_CHECK_CLOSE(mm.calculate_score(5u, 5u, 0), 1.0, 0.001) ;
	}
	BOOST_AUTO_TEST_CASE(calculate_score_0_5)
	{
		match_maker mm( 0.1f ) ;
		BOOST_CHECK_CLOSE(mm.calculate_score(10u, 10u, 5), 0.5, 0.001) ;
	}

BOOST_AUTO_TEST_SUITE_END()
