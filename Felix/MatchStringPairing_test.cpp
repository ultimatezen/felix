#include "StdAfx.h"
#include "MatchStringPairing.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{

	TEST( CMatchStringPairing, IsNumRep_tag )
	{
		CMatchStringPairing pairing ;

		wstring PotentialNum = L"\\TD00003" ;
		ASSERT_TRUE( ! pairing.IsNumRep( PotentialNum ) ) ;
	}
	TEST( CMatchStringPairing, IsNumRep_dash )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"30-03" ;
		ASSERT_TRUE( pairing.IsNumRep( PotentialNum ) ) ;

	}
	TEST( CMatchStringPairing, IsNumRep_with_letter )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"0003-a-0004" ;
		ASSERT_TRUE( ! pairing.IsNumRep( PotentialNum ) ) ;

	}
	TEST( CMatchStringPairing, IsNumRep_dashes )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"-00990-" ;
		ASSERT_TRUE( pairing.IsNumRep( PotentialNum ) ) ;

	}
	TEST( CMatchStringPairing, IsNumRep_paren_with_commas )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"0003(1,7)0004" ;
		ASSERT_TRUE( ! pairing.IsNumRep( PotentialNum ) ) ;
	}
	TEST( TestCMatchStringPairing, IsSubstitution_non_num_source )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"cow" ;
		wstring QueryNum = L"3" ;
		CMatchStringPairing pairing ;
		ASSERT_TRUE( ! pairing.IsSubstitution( trans, SourceNum, 0, QueryNum ) ) ;
	}
	TEST( TestCMatchStringPairing, IsSubstitution_non_num_query )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"3" ;
		wstring QueryNum = L"moo" ;
		CMatchStringPairing pairing ;
		ASSERT_TRUE( ! pairing.IsSubstitution( trans, SourceNum, 0, QueryNum ) ) ;
	}


	TEST( CMatchStringPairing, DoublePlacement )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'2', L'3' ) ;
		ASSERT_EQUALS_DELTA( 1.0f / 3.0f, pairing.CalcScore(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"2a1", L"2a1" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "3a2" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		// same value when called twice
		ASSERT_EQUALS_V(actual, CStringA(pairing.MarkupSource().c_str())) ;

		expected = "<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		// same value when called twice
		ASSERT_EQUALS_V(actual, CStringA(pairing.MarkupQuery().c_str())) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.000001 ) ;
	}
	TEST( CMatchStringPairing, TwoBytePlacement )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'‚R', L'2' ) ;
		ASSERT_EQUALS_DELTA( 1.0f / 2.0f, pairing.CalcScore(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"3a", L"3a" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "<span class=\"placement\">2</span>a" ;
		actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		// same value when called twice
		ASSERT_EQUALS_V(actual, CStringA(pairing.MarkupSource().c_str())) ;

		expected = "<span class=\"placement\">2</span>a" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		// same value when called twice
		ASSERT_EQUALS_V(actual, CStringA(pairing.MarkupQuery().c_str())) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.000001 ) ;
	}
	TEST( CMatchStringPairing, BothDoubleByte )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'‚R', L'‚Q' ) ;
		ASSERT_EQUALS_DELTA( 1.0f / 2.0f, pairing.CalcScore(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"3a", L"3a" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		ASSERT_EQUALS( pairing.MarkupSource(), L"<span class=\"placement\">‚Q</span>a" ) ;

		// same value when called twice
		ASSERT_EQUALS( pairing.MarkupSource(), L"<span class=\"placement\">‚Q</span>a" ) ;

		expected = "<span class=\"placement\">‚Q</span>a" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		// same value when called twice
		ASSERT_EQUALS_V(expected, CStringA(pairing.MarkupQuery().c_str())) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.000001 ) ;
	}

	TEST( CMatchStringPairing, TrivialPlacementFalse )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;
	}
	TEST( CMatchStringPairing, trival_placement_bad_entities )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"<a> & <b>1", L"<a> & <b>1" ) ;

		ASSERT_TRUE(pairing.PlaceNumbers( trans )) ;
		SimpleString expected = "&lt;a&gt; &amp; &lt;b&gt;2" ;
		SimpleString actual = (LPCSTR)CStringA(trans.first.c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

		expected = "&lt;a&gt; &amp; &lt;b&gt;<span class=\"placement\">2</span>" ;
		actual = (LPCSTR)CStringA(trans.second.c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;

	}
	TEST( CMatchStringPairing, TrivialPlacementMatchedNums )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"1", L"1" ) ;

		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;
	}
	TEST( CMatchStringPairing, TrivialPlacementTrue )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"1", L"1" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
	}
	TEST( CMatchStringPairing, PlacementNoTransMatch )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;
	}
	TEST( CMatchStringPairing, PlacementNoTransNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;
	}
	TEST( CMatchStringPairing, PlacementLongNumNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a1", L"a1" ) ;

		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;
	}
	TEST( CMatchStringPairing, PlacementLongNumMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "a12" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		trans.first = L"1111" ;
		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;

		trans.first = L"11a11" ;
		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;

	}

	TEST( CMatchStringPairing, PlacementLongNumMatchLonger1 )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;
		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "a12" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( CMatchStringPairing, PlacementLongNumMatchLonger2 )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;
		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"1111", L"1111" ) ;
		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;

		trans.first = L"11a11" ;
		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;
	}

	TEST( CMatchStringPairing, PlacementLongNumMatchEpsilons1 )
	{
		CMatchStringPairing pairing ;

		pairing.SourceToEpsilon( L'a' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.SourceToEpsilon( L'1' ) ;
		pairing.NoMatch( L'1', L'2' ) ;
		pairing.QueryToEpsilon( L'3' ) ;
		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		// a11
		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;

		CStringA expected = "a32" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "a<span class=\"nomatch\">a</span><span class=\"placement\">32</span>a<span class=\"nomatch\">a</span>" ;
		actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "a<span class=\"nomatch\">b</span><span class=\"placement\">32</span>a" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

	}
	TEST( CMatchStringPairing, PlacementLongNumMatchEpsilons2 )
	{
		CMatchStringPairing pairing ;

		pairing.SourceToEpsilon( L'a' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.SourceToEpsilon( L'1' ) ;
		pairing.NoMatch( L'1', L'2' ) ;
		pairing.QueryToEpsilon( L'3' ) ;
		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		// 1111
		std::pair< wstring, wstring > trans( L"1111", L"1111" ) ;
		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;

		// 11a11
		trans.first = L"11a11" ;
		ASSERT_TRUE( ! pairing.PlaceNumbers( trans ) ) ;

	}

	TEST( CMatchStringPairing, WithPunct )
	{
		CMatchStringPairing pairing ;

		pairing.Match  ( L'a', L'a' ) ;
		pairing.NoMatch( L'3', L'6' ) ;
		pairing.Match  ( L',', L',' ) ;
		pairing.NoMatch( L'2', L'5' ) ;
		pairing.NoMatch( L'1', L'4' ) ;
		pairing.Match  ( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"c12,3c", L"c12,3c" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "c45,6c" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, WithJpn )
	{
		CMatchStringPairing pairing ;

		pairing.Match  ( L'a', L'a' ) ;
		pairing.NoMatch( L'3', L'6' ) ;
		pairing.Match  ( L'‚©', L'‚©' ) ;
		pairing.NoMatch( L'2', L'5' ) ;
		pairing.Match  ( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"c2a3c", L"c2a3c" ) ;

		ASSERT_TRUE( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "c5a6c" ;
		CStringA actual = trans.first.c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.00001 ) ;
	}

	TEST( CMatchStringPairing, TrivialMatch )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;

		ASSERT_EQUALS_M( pairing.MarkupSource(), L"a", "Expected the string [a]" ) ;
		ASSERT_EQUALS_M( pairing.MarkupQuery(), L"a", "Expected the string [a]" ) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, TrivialMatchWithBrackets_source )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'>', L'>' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.Match( L'<', L'<' ) ;

		string markup = string2string(pairing.MarkupSource()) ;
		ASSERT_EQUALS_V("&lt;a&gt;", SimpleString(markup.c_str())) ;
		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, TrivialMatchWithBrackets_trans )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'>', L'>' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.Match( L'<', L'<' ) ;

		string markup = string2string(pairing.MarkupQuery()) ;
		ASSERT_EQUALS_V("&lt;a&gt;", SimpleString(markup.c_str())) ;
		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, TrivialNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'a', L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		ASSERT_EQUALS_DELTA( 0.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, LargerMatch )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'c', L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		ASSERT_EQUALS_M( pairing.MarkupSource(), L"abc", "Expected the string [abc]" ) ;
		ASSERT_EQUALS_M( pairing.MarkupQuery(), L"abc", "Expected the string [abc]" ) ;

		ASSERT_EQUALS_DELTA( 1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, LargerNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'c', L'c' ) ;
		pairing.NoMatch( L'b', L'b' ) ;
		pairing.NoMatch( L'a', L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">abc</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		ASSERT_EQUALS_DELTA( 0.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, Mixed )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'c', L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.NoMatch( L'a', L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>b<span class=\"nomatch\">c</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected.Format( "%f", 1.0f / 3.0f ) ;
		actual.Format( "%f", pairing.CalcScore()) ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( CMatchStringPairing, MixedEnds )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'c', L'c' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'c', L'c' ) ;

		CStringA expected = "<span class=\"nomatch\">c</span>aa<span class=\"nomatch\">c</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		ASSERT_EQUALS_DELTA( 0.5f, pairing.CalcScore(), 0.00001 ) ;
	}
	TEST( CMatchStringPairing, MixedMiddle )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'c', L'c' ) ;
		pairing.NoMatch( L'c', L'c' ) ;
		pairing.Match( L'a', L'a' ) ;

		CStringA expected = "a<span class=\"nomatch\">cc</span>a" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( CMatchStringPairing, Epsilons )
	{
		CMatchStringPairing pairing ;

		pairing.QueryToEpsilon( L'c' ) ;
		pairing.NoMatch( L'b', L'b' ) ;
		pairing.SourceToEpsilon( L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">ab</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "<span class=\"nomatch\">bc</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( CMatchStringPairing, EpsilonsMixed )
	{
		CMatchStringPairing pairing ;

		pairing.QueryToEpsilon( L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.SourceToEpsilon( L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>b" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "b<span class=\"nomatch\">c</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
	TEST( CMatchStringPairing, EpsilonsMixedEnds )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'b', L'b' ) ;
		pairing.QueryToEpsilon( L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.SourceToEpsilon( L'a' ) ;
		pairing.NoMatch( L'b', L'b' ) ;

		CStringA expected = "<span class=\"nomatch\">ba</span>b<span class=\"nomatch\">b</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;

		expected = "<span class=\"nomatch\">b</span>b<span class=\"nomatch\">cb</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		ASSERT_EQUALS_V( expected, actual ) ;
	}
}

#endif