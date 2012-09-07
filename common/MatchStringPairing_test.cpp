#include "StdAfx.h"
#include "MatchStringPairing.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

using namespace mem_engine ;
using namespace placement ;

void pair_up(match_string_pairing &pairing, wstring source, wstring query)
{
	for (int i = source.size() - 1 ; i >= 0 ; --i)
	{
		wchar_t s = source[i] ;
		wchar_t q = query[i] ;

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
		match_string_pairing pairing ;

		wstring PotentialNum = L"\\TD00003" ;
		BOOST_CHECK( ! pairing.is_num_rep( PotentialNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( is_num_rep_dash )
	{
		match_string_pairing pairing ;
		wstring PotentialNum = L"30-03" ;
		BOOST_CHECK( pairing.is_num_rep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( is_num_rep_with_letter )
	{
		match_string_pairing pairing ;
		wstring PotentialNum = L"0003-a-0004" ;
		BOOST_CHECK( ! pairing.is_num_rep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( is_num_rep_dashes )
	{
		match_string_pairing pairing ;
		wstring PotentialNum = L"-00990-" ;
		BOOST_CHECK( pairing.is_num_rep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( is_num_rep_paren_with_commas )
	{
		match_string_pairing pairing ;
		wstring PotentialNum = L"0003(1,7)0004" ;
		BOOST_CHECK( ! pairing.is_num_rep( PotentialNum ) ) ;
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
		match_string_pairing pairing ;
		BOOST_CHECK( ! pairing.is_substitution( trans, SourceNum, 0, QueryNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( IsSubstitution_non_num_query )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"3" ;
		wstring QueryNum = L"moo" ;
		match_string_pairing pairing ;
		BOOST_CHECK( ! pairing.is_substitution( trans, SourceNum, 0, QueryNum ) ) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_Placement )


	BOOST_AUTO_TEST_CASE( DoublePlacement )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;
		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'2', L'3' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 3.0f, pairing.calc_score(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"2a1", L"2a1" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		CStringA expected = "3a2" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.mark_up_source().c_str())) ;

		expected = "<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.mark_up_query().c_str())) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.000001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TwoBytePlacement )
	{
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'ÇR', L'2' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 2.0f, pairing.calc_score(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"3a", L"3a" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"placement\">2</span>a" ;
		actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.mark_up_source().c_str())) ;

		expected = "<span class=\"placement\">2</span>a" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.mark_up_query().c_str())) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.000001 ) ;
	}
	BOOST_AUTO_TEST_CASE( BothDoubleByte )
	{
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'ÇR', L'ÇQ' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 2.0f, pairing.calc_score(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"3a", L"3a" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_EQUAL( pairing.mark_up_source(), L"<span class=\"placement\">ÇQ</span>a" ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL( pairing.mark_up_source(), L"<span class=\"placement\">ÇQ</span>a" ) ;

		expected = "<span class=\"placement\">ÇQ</span>a" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(expected, CStringA(pairing.mark_up_query().c_str())) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.000001 ) ;
	}

	BOOST_AUTO_TEST_CASE( TrivialPlacementFalse )
	{
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( placement_false_commas )
	{
		match_string_pairing pairing ;

		pair_up(pairing, L"I have, apples", L"I have 3 apples") ;

		std::pair< wstring, wstring > trans( L"I have 5 apples", L"I have 5 apples" ) ;

		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( trival_placement_bad_entities )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"<a> & <b>1", L"<a> & <b>1" ) ;

		BOOST_CHECK(pairing.place_numbers( trans )) ;
		wstring expected = L"&lt;a&gt; &amp; &lt;b&gt;2" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = L"&lt;a&gt; &amp; &lt;b&gt;<span class=\"placement\">2</span>" ;
		actual = trans.second ;
		BOOST_CHECK_EQUAL(expected, actual) ;

	}
	BOOST_AUTO_TEST_CASE( TrivialPlacementMatchedNums )
	{
		match_string_pairing pairing ;

		pairing.match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"1", L"1" ) ;

		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialPlacementTrue )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"1", L"1" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementNoTransMatch )
	{
		match_string_pairing pairing ;

		pairing.match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementNoTransNoMatch )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumNoMatch )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;
		pairing.match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a1", L"a1" ) ;

		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatch )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;
		pairing.match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		CStringA expected = "a12" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		trans.first = L"1111" ;
		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;

		trans.first = L"11a11" ;
		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;

	}

	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchLonger1 )
	{
		match_string_pairing pairing ;

		pair_up(pairing, L"aa11aa", L"ab12ab") ;

		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		CStringA expected = "a12" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchLonger2 )
	{
		match_string_pairing pairing ;

		pair_up(pairing, L"aa11aa", L"ab12ab") ;

		std::pair< wstring, wstring > trans( L"1111", L"1111" ) ;
		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;

		trans.first = L"11a11" ;
		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;
	}

	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchEpsilons1 )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"aa*11aa", 
			L"ab32*a*") ;

		// a11
		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;

		CStringA expected = "a32" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "a<span class=\"nomatch\">a</span><span class=\"placement\">32</span>a<span class=\"nomatch\">a</span>" ;
		actual = pairing.mark_up_source().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "a<span class=\"nomatch\">b</span><span class=\"placement\">32</span>a" ;
		actual = pairing.mark_up_query().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

	}


	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchEpsilons2 )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"aa*11aa", 
			L"ab32*a*") ;

		// 1111
		std::pair< wstring, wstring > trans( L"1111", L"1111" ) ;
		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;

		// 11a11
		trans.first = L"11a11" ;
		BOOST_CHECK( ! pairing.place_numbers( trans ) ) ;

	}

	BOOST_AUTO_TEST_CASE( WithPunct )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"a12,3a", 
			L"a45,6a") ;

		std::pair< wstring, wstring > trans( L"c12,3c", L"c12,3c" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		wstring expected = L"c45,6c" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( WithJpn )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"a2Ç©3a", 
			L"a5Ç©6a") ;

		std::pair< wstring, wstring > trans( L"c2a3c", L"c2a3c" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;
		wstring expected = L"c5a6c" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.00001 ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_Markup )

	BOOST_AUTO_TEST_CASE( TrivialMatch )
	{
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;

		BOOST_CHECK_EQUAL( pairing.mark_up_source(), L"a") ;
		BOOST_CHECK_EQUAL( pairing.mark_up_query(), L"a") ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialMatchWithBrackets_source )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"<a>", 
			L"<a>") ;

		wstring markup = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL(L"&lt;a&gt;", markup) ;
		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialMatchWithBrackets_query )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"<a>", 
			L"<a>") ;

		wstring markup = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL(L"&lt;a&gt;", markup) ;
		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.00001 ) ;
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

		BOOST_CHECK_CLOSE( (double)0.0f, pairing.calc_score(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( LargerMatch )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"abc") ;

		BOOST_CHECK_EQUAL( pairing.mark_up_source(), L"abc") ;
		BOOST_CHECK_EQUAL( pairing.mark_up_query(), L"abc") ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.calc_score(), 0.00001 ) ;
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
		BOOST_CHECK_CLOSE( (double)0.0f, pairing.calc_score(), 0.00001 ) ;
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
		BOOST_CHECK_CLOSE( (double)0.0f, pairing.calc_score(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( Mixed_Score )
	{
		match_string_pairing pairing ;

		pair_up(pairing, 
			L"abc", 
			L"xbx") ;

		double expected_score = 1.0 / 3.0 ;
		BOOST_CHECK_CLOSE(expected_score, pairing.calc_score(), 0.001) ;
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

		BOOST_CHECK_CLOSE( 0.5, pairing.calc_score(), 0.00001 ) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing_backslashes )


	//////////////////////////////////////////////////////////////////////////
	// tests for bug #274 (placement with backslash
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( backslash_error_extra_string_trans )
	{
		match_string_pairing pairing ;

		// source: \850,000 Åiê≈çû\892,500Åj
		// query:  \1,300,000 Åiê≈çû\1,365,000Åj
		// trans: \850,000 (\892,500 with tax)

		wstring source = L"\\**850,000 Åiê≈çû\\**892,500Åj" ;
		wstring query  = L"\\1,300,000 Åiê≈çû\\1,365,000Åj" ;

		BOOST_CHECK_EQUAL(source.size(), query.size()) ;

		pair_up(pairing, source, query) ;

		// a11
		std::pair< wstring, wstring > trans( L"\\850,000 (\\892,500 with tax)", 
										     L"\\850,000 (\\892,500 with tax)" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;

		wstring expected = L"\\1,300,000 (\\1,365,000 with tax)" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = L"\\<span class=\"placement\">1,300,000</span> (\\<span class=\"placement\">1,365,000</span> with tax)" ;
		actual = trans.second ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( backslash_error_extra_string_source )
	{
		match_string_pairing pairing ;

		// source: \850,000 Åiê≈çû\892,500Åj
		// query:  \1,300,000 Åiê≈çû\1,365,000Åj
		// trans: \850,000 (\892,500 with tax)

		wstring source = L"\\**850,000 Åiê≈çû\\**892,500Åj" ;
		wstring query  = L"\\1,300,000 Åiê≈çû\\1,365,000Åj" ;

		pair_up(pairing, source, query) ;

		// a11
		std::pair< wstring, wstring > trans( L"\\850,000 (\\892,500 with tax)", 
			L"\\850,000 (\\892,500 with tax)" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;

		wstring expected = L"\\<span class=\"placement\">1,300,000</span> Åiê≈çû\\<span class=\"placement\">1,365,000</span>Åj" ;
		wstring actual = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( backslash_error_extra_string_query )
	{

		match_string_pairing pairing ;

		// source: \850,000 Åiê≈çû\892,500Åj
		// query:  \1,300,000 Åiê≈çû\1,365,000Åj
		// trans: \850,000 (\892,500 with tax)

		wstring source = L"\\**850,000 Åiê≈çû\\**892,500Åj" ;
		wstring query  = L"\\1,300,000 Åiê≈çû\\1,365,000Åj" ;

		pair_up(pairing, source, query) ;

		// a11
		std::pair< wstring, wstring > trans( L"\\850,000 (\\892,500 with tax)", 
			L"\\850,000 (\\892,500 with tax)" ) ;

		BOOST_CHECK( pairing.place_numbers( trans ) ) ;

		wstring expected = L"\\<span class=\"placement\">1,300,000</span> Åiê≈çû\\<span class=\"placement\">1,365,000</span>Åj" ;
		wstring actual = pairing.mark_up_query() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
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
	BOOST_AUTO_TEST_CASE( MatchesWide )
	{
		BOOST_CHECK_EQUAL( L'0', narrow_num( L'ÇO' ) ) ;
		BOOST_CHECK_EQUAL( L'1', narrow_num( L'ÇP' ) ) ;
		BOOST_CHECK_EQUAL( L'2', narrow_num( L'ÇQ' ) ) ;
		BOOST_CHECK_EQUAL( L'3', narrow_num( L'ÇR' ) ) ;
		BOOST_CHECK_EQUAL( L'4', narrow_num( L'ÇS' ) ) ;
		BOOST_CHECK_EQUAL( L'5', narrow_num( L'ÇT' ) ) ;
		BOOST_CHECK_EQUAL( L'6', narrow_num( L'ÇU' ) ) ;
		BOOST_CHECK_EQUAL( L'7', narrow_num( L'ÇV' ) ) ;
		BOOST_CHECK_EQUAL( L'8', narrow_num( L'ÇW' ) ) ;
		BOOST_CHECK_EQUAL( L'9', narrow_num( L'ÇX' ) ) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif