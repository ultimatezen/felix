﻿#include "StdAfx.h"
#include "MatchStringPairing.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

using namespace mem_engine ;
using namespace placement ;

void pair_up(match_string_pairing &pairing, wstring source, wstring query)
{
	for (int i = source.size() - 1 ; i >= 0 ; --i)
	{
		const wchar_t s = source[i] ;
		const wchar_t q = query[i] ;

		if (s == L'*')
		{
			pairing.query_to_epsilon(q) ;
		}
		else if (q == L'*')
		{
			pairing.source_to_epsilon(s) ;
		}
		else if ( s == q )
		{
			pairing.match(s, q) ;
		}
		else
		{
			pairing.no_match(s, q) ;
		}
	}
}

BOOST_AUTO_TEST_SUITE( Test_CMatchStringPairing_is_num_rep )

	BOOST_AUTO_TEST_CASE( is_num_rep_tag )
	{
		wstring PotentialNum = L"\\TD00003" ;
		BOOST_CHECK( ! is_num_rep( PotentialNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( is_num_rep_dash )
	{
		wstring PotentialNum = L"30-03" ;
		BOOST_CHECK( is_num_rep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( is_num_rep_with_letter )
	{
		wstring PotentialNum = L"0003-a-0004" ;
		BOOST_CHECK( ! is_num_rep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( is_num_rep_dashes )
	{
		wstring PotentialNum = L"-00990-" ;
		BOOST_CHECK( is_num_rep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( is_num_rep_paren_with_commas )
	{
		wstring PotentialNum = L"0003(1,7)0004" ;
		BOOST_CHECK( ! is_num_rep( PotentialNum ) ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_IsSubstitution )

	BOOST_AUTO_TEST_CASE( IsSubstitution_non_num_source )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"cow" ;
		wstring QueryNum = L"3" ;
		BOOST_CHECK( ! is_substitution( trans, SourceNum, 0, QueryNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( IsSubstitution_non_num_query )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"3" ;
		wstring QueryNum = L"moo" ;
		BOOST_CHECK( ! is_substitution( trans, SourceNum, 0, QueryNum ) ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_has_gloss_match_in_query_at )

	BOOST_AUTO_TEST_CASE( no_match )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"apple", 0), false) ;
	}

	BOOST_AUTO_TEST_CASE( match )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"bacon", 0), true) ;
	}

	BOOST_AUTO_TEST_CASE( match_first_half )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"bac", 0), true) ;
	}

	BOOST_AUTO_TEST_CASE( match_second_half )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"con", 2), true) ;
	}

	BOOST_AUTO_TEST_CASE( match_with_epsilon )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"barcon",
				L"ba*con") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"bacon", 0), true) ;
	}

	BOOST_AUTO_TEST_CASE( gloss_not_consumed_start )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"back", 0), false) ;
	}

	BOOST_AUTO_TEST_CASE( gloss_not_consumed_end )
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		BOOST_CHECK_EQUAL(has_gloss_match_in_query_at(pairs.get(), L"condor", 2), false) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_mark_gloss_match_in_query_at )

	BOOST_AUTO_TEST_CASE( mark_up_con)
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		pairings_t &pairings = pairs.get() ;
		mark_gloss_match_in_query_at(pairings, L"con", 2) ;

		MatchType compound = static_cast<MatchType>(
			MATCH | GLOSS_MATCH ) ;
		BOOST_CHECK_EQUAL(pairings[0].match_type(), MATCH) ;
		BOOST_CHECK_EQUAL(pairings[1].match_type(), MATCH) ;
		BOOST_CHECK_EQUAL(pairings[2].match_type(), compound) ;
		BOOST_CHECK_EQUAL(pairings[3].match_type(), compound) ;
		BOOST_CHECK_EQUAL(pairings[4].match_type(), compound) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_mark_up_gloss_matches )

	BOOST_AUTO_TEST_CASE( mark_up_con)
	{
		match_string_pairing pairs;
		pair_up(pairs,
				L"bacon",
				L"bacon") ;

		mem_engine::gloss_match_set gloss_matches ;
		gloss_matches.insert(L"con") ;

		pairings_t &pairings = pairs.get() ;
		mark_up_gloss_matches(pairings, gloss_matches) ;

		MatchType compound = static_cast<MatchType>(
			MATCH | GLOSS_MATCH ) ;
		BOOST_CHECK_EQUAL(pairings[0].match_type(), MATCH) ;
		BOOST_CHECK_EQUAL(pairings[1].match_type(), MATCH) ;
		BOOST_CHECK_EQUAL(pairings[2].match_type(), compound) ;
		BOOST_CHECK_EQUAL(pairings[3].match_type(), compound) ;
		BOOST_CHECK_EQUAL(pairings[4].match_type(), compound) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_Markup )

	BOOST_AUTO_TEST_CASE( TrivialMatch )
	{
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;

		BOOST_CHECK_EQUAL( pairing.mark_up_source(), L"a") ;
		BOOST_CHECK_EQUAL( pairing.mark_up_query(), L"a") ;

		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairing.get()), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialMatchWithBrackets_source )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"<a>", 
			L"<a>") ;

		wstring markup = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL(L"&lt;a&gt;", markup) ;
		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairing.get()), 0.00001 ) ;
	}

	// gloss-match markup for query string
	BOOST_AUTO_TEST_CASE( gloss_placement_query )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"<a>", 
			L"<a>") ;
		auto pairs = pairing.get() ;
		pairs[1].add_match_type(GLOSS_MATCH) ;
		pairing.set(pairs) ;

		wstring markup = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL(L"&lt;<span class=\"gloss_match\">a</span>&gt;", markup) ;
		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairing.get()), 0.00001 ) ;
	}

	// no gloss-match markup for source string
	BOOST_AUTO_TEST_CASE( gloss_placement_source )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"<a>", 
			L"<a>") ;
		auto pairs = pairing.get() ;
		pairs[1].add_match_type(GLOSS_MATCH) ;
		pairing.set(pairs) ;

		wstring markup = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL(L"&lt;a&gt;", markup) ;
		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairing.get()), 0.00001 ) ;
	}

	BOOST_AUTO_TEST_CASE( TrivialMatchWithBrackets_query )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"<a>", 
			L"<a>") ;

		wstring markup = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL(L"&lt;a&gt;", markup) ;
		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairing.get()), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialNoMatch )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'a', L'a' ) ;

		wstring expected = L"<span class=\"nomatch\">a</span>" ;

		wstring actual = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)0.0f, calc_score(pairing.get()), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( LargerMatch )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"abc") ;

		BOOST_CHECK_EQUAL( pairing.mark_up_source(), L"abc") ;
		BOOST_CHECK_EQUAL( pairing.mark_up_query(), L"abc") ;

		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairing.get()), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( LargerNoMatch_source )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"def") ;

		wstring expected = L"<span class=\"nomatch\">abc</span>" ;
		wstring actual = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
		BOOST_CHECK_CLOSE( (double)0.0f, calc_score(pairing.get()), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( LargerNoMatch_query )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"def") ;

		wstring expected = L"<span class=\"nomatch\">def</span>" ;
		wstring actual = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
		BOOST_CHECK_CLOSE( (double)0.0f, calc_score(pairing.get()), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( Mixed_Score )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"xbx") ;

		double expected_score = 1.0 / 3.0 ;
		BOOST_CHECK_CLOSE(expected_score, calc_score(pairing.get()), 0.001) ;
	}
	BOOST_AUTO_TEST_CASE( Mixed_Source )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"xbx") ;

		wstring expected = L"<span class=\"nomatch\">a</span>b<span class=\"nomatch\">c</span>" ;

		wstring actual = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( Mixed_Query )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"xbx") ;

		wstring expected = L"<span class=\"nomatch\">x</span>b<span class=\"nomatch\">x</span>" ;

		wstring actual = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( MixedEnds_source )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abba", 
			L"xbbx") ;

		wstring expected = L"<span class=\"nomatch\">a</span>bb<span class=\"nomatch\">a</span>" ;

		wstring actual = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( MixedEnds_query )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abba", 
			L"xbbx") ;

		wstring expected = L"<span class=\"nomatch\">x</span>bb<span class=\"nomatch\">x</span>" ;

		wstring actual = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( MixedMiddle )
	{
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'c', L'c' ) ;
		pairing.no_match( L'c', L'c' ) ;
		pairing.match( L'a', L'a' ) ;

		CStringA expected = "a<span class=\"nomatch\">cc</span>a" ;

		CStringA actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( Epsilons )
	{
		match_string_pairing pairing ;

		pairing.query_to_epsilon( L'c' ) ;
		pairing.no_match( L'b', L'b' ) ;
		pairing.source_to_epsilon( L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">ab</span>" ;

		CStringA actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"nomatch\">bc</span>" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( EpsilonsMixed )
	{
		match_string_pairing pairing ;

		pairing.query_to_epsilon( L'c' ) ;
		pairing.match( L'b', L'b' ) ;
		pairing.source_to_epsilon( L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>b" ;

		CStringA actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "b<span class=\"nomatch\">c</span>" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( EpsilonsMixedEnds )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'b', L'b' ) ;
		pairing.query_to_epsilon( L'c' ) ;
		pairing.match( L'b', L'b' ) ;
		pairing.source_to_epsilon( L'a' ) ;
		pairing.no_match( L'b', L'b' ) ;

		CStringA expected = "<span class=\"nomatch\">ba</span>b<span class=\"nomatch\">b</span>" ;

		CStringA actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"nomatch\">b</span>b<span class=\"nomatch\">cb</span>" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_MarkupString )

BOOST_AUTO_TEST_CASE(test_lt_source)
{
	match_string_pairing pairing ;

	pair_up(pairing, 
		L"<", 
		L"<") ;

	wstring markup = pairing.mark_up_source() ;

	wstring expected = L"&lt;" ;
	BOOST_CHECK_EQUAL( expected, markup ) ;
}

BOOST_AUTO_TEST_CASE(test_gt_source)
{
	match_string_pairing pairing ;

	pair_up(pairing, 
		L">", 
		L">") ;

	wstring markup = pairing.mark_up_source() ;

	wstring expected = L"&gt;" ;
	BOOST_CHECK_EQUAL( expected, markup ) ;
}
BOOST_AUTO_TEST_CASE(test_amp_source)
{
	match_string_pairing pairing ;

	pair_up(pairing, 
		L"&", 
		L"&") ;

	wstring markup = pairing.mark_up_source() ;

	wstring expected = L"&amp;" ;
	BOOST_CHECK_EQUAL( expected, markup ) ;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_calc_score )

	BOOST_AUTO_TEST_CASE( MixedEnds_score )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abba", 
			L"xbbx") ;

		BOOST_CHECK_CLOSE( 0.5, calc_score(pairing.get()), 0.00001 ) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_narrow_num )

	BOOST_AUTO_TEST_CASE( NoMatches )
	{
		BOOST_CHECK_EQUAL( L'a', narrow_num( L'a' ) ) ;
		BOOST_CHECK_EQUAL( L'a', narrow_num( L'a' ) ) ;
		BOOST_CHECK_EQUAL( 0, narrow_num( L'\0' ) ) ;
		BOOST_CHECK_EQUAL( L'b', narrow_num( L'b' ) ) ;
		BOOST_CHECK_EQUAL( L'z', narrow_num( L'z' ) ) ;
		BOOST_CHECK_EQUAL( L'!', narrow_num( L'!' ) ) ;
		BOOST_CHECK_EQUAL( L'@', narrow_num( L'@' ) ) ;
		BOOST_CHECK_EQUAL( L'\\', narrow_num( L'\\' ) ) ;
	}
	BOOST_AUTO_TEST_CASE( MatchesNarrow )
	{
		BOOST_CHECK_EQUAL( L'0', narrow_num( L'0' ) ) ;
		BOOST_CHECK_EQUAL( L'2', narrow_num( L'2' ) ) ;
		BOOST_CHECK_EQUAL( L'4', narrow_num( L'4' ) ) ;
		BOOST_CHECK_EQUAL( L'6', narrow_num( L'6' ) ) ;
		BOOST_CHECK_EQUAL( L'8', narrow_num( L'8' ) ) ;
		BOOST_CHECK_EQUAL( L'9', narrow_num( L'9' ) ) ;
	}
#pragma warning( disable:4566 ) // #pragma deprecated

	BOOST_AUTO_TEST_CASE( MatchesWide )
	{
		BOOST_CHECK_EQUAL( L'0', narrow_num( L'０' ) ) ;
		BOOST_CHECK_EQUAL( L'1', narrow_num( L'１' ) ) ;
		BOOST_CHECK_EQUAL( L'2', narrow_num( L'２' ) ) ;
		BOOST_CHECK_EQUAL( L'3', narrow_num( L'３' ) ) ;
		BOOST_CHECK_EQUAL( L'4', narrow_num( L'４' ) ) ;
		BOOST_CHECK_EQUAL( L'5', narrow_num( L'５' ) ) ;
		BOOST_CHECK_EQUAL( L'6', narrow_num( L'６' ) ) ;
		BOOST_CHECK_EQUAL( L'7', narrow_num( L'７' ) ) ;
		BOOST_CHECK_EQUAL( L'8', narrow_num( L'８' ) ) ;
		BOOST_CHECK_EQUAL( L'9', narrow_num( L'９' ) ) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif