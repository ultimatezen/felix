#include "StdAfx.h"
#include "MatchStringPairing.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_CMatchStringPairing )

	BOOST_AUTO_TEST_CASE( IsNumRep_tag )
	{
		CMatchStringPairing pairing ;

		wstring PotentialNum = L"\\TD00003" ;
		BOOST_CHECK( ! pairing.IsNumRep( PotentialNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( IsNumRep_dash )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"30-03" ;
		BOOST_CHECK( pairing.IsNumRep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( IsNumRep_with_letter )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"0003-a-0004" ;
		BOOST_CHECK( ! pairing.IsNumRep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( IsNumRep_dashes )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"-00990-" ;
		BOOST_CHECK( pairing.IsNumRep( PotentialNum ) ) ;

	}
	BOOST_AUTO_TEST_CASE( IsNumRep_paren_with_commas )
	{
		CMatchStringPairing pairing ;
		wstring PotentialNum = L"0003(1,7)0004" ;
		BOOST_CHECK( ! pairing.IsNumRep( PotentialNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( IsSubstitution_non_num_source )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"cow" ;
		wstring QueryNum = L"3" ;
		CMatchStringPairing pairing ;
		BOOST_CHECK( ! pairing.IsSubstitution( trans, SourceNum, 0, QueryNum ) ) ;
	}
	BOOST_AUTO_TEST_CASE( IsSubstitution_non_num_query )
	{
		wstring first = L"spam" ;
		wstring second = L"egg" ;
		std::pair<wstring, wstring> trans(first, second) ;
		wstring SourceNum = L"3" ;
		wstring QueryNum = L"moo" ;
		CMatchStringPairing pairing ;
		BOOST_CHECK( ! pairing.IsSubstitution( trans, SourceNum, 0, QueryNum ) ) ;
	}


	BOOST_AUTO_TEST_CASE( DoublePlacement )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'2', L'3' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 3.0f, pairing.CalcScore(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"2a1", L"2a1" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "3a2" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.MarkupSource().c_str())) ;

		expected = "<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.MarkupQuery().c_str())) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.000001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TwoBytePlacement )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'‚R', L'2' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 2.0f, pairing.CalcScore(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"3a", L"3a" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"placement\">2</span>a" ;
		actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.MarkupSource().c_str())) ;

		expected = "<span class=\"placement\">2</span>a" ;
		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(actual, CStringA(pairing.MarkupQuery().c_str())) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.000001 ) ;
	}
	BOOST_AUTO_TEST_CASE( BothDoubleByte )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'‚R', L'‚Q' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 2.0f, pairing.CalcScore(), 0.000001 ) ;

		std::pair< wstring, wstring > trans( L"3a", L"3a" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_EQUAL( pairing.MarkupSource(), L"<span class=\"placement\">‚Q</span>a" ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL( pairing.MarkupSource(), L"<span class=\"placement\">‚Q</span>a" ) ;

		expected = "<span class=\"placement\">‚Q</span>a" ;
		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		// same value when called twice
		BOOST_CHECK_EQUAL(expected, CStringA(pairing.MarkupQuery().c_str())) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.000001 ) ;
	}

	BOOST_AUTO_TEST_CASE( TrivialPlacementFalse )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( trival_placement_bad_entities )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"<a> & <b>1", L"<a> & <b>1" ) ;

		BOOST_CHECK(pairing.PlaceNumbers( trans )) ;
		string expected = "&lt;a&gt; &amp; &lt;b&gt;2" ;
		string actual = (LPCSTR)CStringA(trans.first.c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

		expected = "&lt;a&gt; &amp; &lt;b&gt;<span class=\"placement\">2</span>" ;
		actual = (LPCSTR)CStringA(trans.second.c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;

	}
	BOOST_AUTO_TEST_CASE( TrivialPlacementMatchedNums )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"1", L"1" ) ;

		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialPlacementTrue )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"1", L"1" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementNoTransMatch )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementNoTransNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;

		std::pair< wstring, wstring > trans( L"a", L"a" ) ;

		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a1", L"a1" ) ;

		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;

		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "a12" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		trans.first = L"1111" ;
		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;

		trans.first = L"11a11" ;
		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;

	}

	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchLonger1 )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;
		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"a11", L"a11" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "a12" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchLonger2 )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'1', L'2' ) ;
		pairing.Match( L'1', L'1' ) ;
		pairing.NoMatch( L'a', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"1111", L"1111" ) ;
		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;

		trans.first = L"11a11" ;
		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;
	}

	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchEpsilons1 )
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

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;

		CStringA expected = "a32" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "a<span class=\"nomatch\">a</span><span class=\"placement\">32</span>a<span class=\"nomatch\">a</span>" ;
		actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "a<span class=\"nomatch\">b</span><span class=\"placement\">32</span>a" ;
		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchEpsilons2 )
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
		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;

		// 11a11
		trans.first = L"11a11" ;
		BOOST_CHECK( ! pairing.PlaceNumbers( trans ) ) ;

	}

	BOOST_AUTO_TEST_CASE( WithPunct )
	{
		CMatchStringPairing pairing ;

		pairing.Match  ( L'a', L'a' ) ;
		pairing.NoMatch( L'3', L'6' ) ;
		pairing.Match  ( L',', L',' ) ;
		pairing.NoMatch( L'2', L'5' ) ;
		pairing.NoMatch( L'1', L'4' ) ;
		pairing.Match  ( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"c12,3c", L"c12,3c" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "c45,6c" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( WithJpn )
	{
		CMatchStringPairing pairing ;

		pairing.Match  ( L'a', L'a' ) ;
		pairing.NoMatch( L'3', L'6' ) ;
		pairing.Match  ( L'‚©', L'‚©' ) ;
		pairing.NoMatch( L'2', L'5' ) ;
		pairing.Match  ( L'a', L'a' ) ;

		std::pair< wstring, wstring > trans( L"c2a3c", L"c2a3c" ) ;

		BOOST_CHECK( pairing.PlaceNumbers( trans ) ) ;
		CStringA expected = "c5a6c" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.00001 ) ;
	}

	BOOST_AUTO_TEST_CASE( TrivialMatch )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;

		BOOST_CHECK_EQUAL( pairing.MarkupSource(), L"a") ;
		BOOST_CHECK_EQUAL( pairing.MarkupQuery(), L"a") ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialMatchWithBrackets_source )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'>', L'>' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.Match( L'<', L'<' ) ;

		string markup = string2string(pairing.MarkupSource()) ;
		BOOST_CHECK_EQUAL("&lt;a&gt;", string(markup.c_str())) ;
		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialMatchWithBrackets_trans )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'>', L'>' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.Match( L'<', L'<' ) ;

		string markup = string2string(pairing.MarkupQuery()) ;
		BOOST_CHECK_EQUAL("&lt;a&gt;", string(markup.c_str())) ;
		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'a', L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)0.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( LargerMatch )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'c', L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.Match( L'a', L'a' ) ;

		BOOST_CHECK_EQUAL( pairing.MarkupSource(), L"abc") ;
		BOOST_CHECK_EQUAL( pairing.MarkupQuery(), L"abc") ;

		BOOST_CHECK_CLOSE( (double)1.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( LargerNoMatch )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'c', L'c' ) ;
		pairing.NoMatch( L'b', L'b' ) ;
		pairing.NoMatch( L'a', L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">abc</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)0.0f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( Mixed )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'c', L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.NoMatch( L'a', L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>b<span class=\"nomatch\">c</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected.Format( "%f", 1.0f / 3.0f ) ;
		actual.Format( "%f", pairing.CalcScore()) ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( MixedEnds )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'c', L'c' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'c', L'c' ) ;

		CStringA expected = "<span class=\"nomatch\">c</span>aa<span class=\"nomatch\">c</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)0.5f, pairing.CalcScore(), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( MixedMiddle )
	{
		CMatchStringPairing pairing ;

		pairing.Match( L'a', L'a' ) ;
		pairing.NoMatch( L'c', L'c' ) ;
		pairing.NoMatch( L'c', L'c' ) ;
		pairing.Match( L'a', L'a' ) ;

		CStringA expected = "a<span class=\"nomatch\">cc</span>a" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( Epsilons )
	{
		CMatchStringPairing pairing ;

		pairing.QueryToEpsilon( L'c' ) ;
		pairing.NoMatch( L'b', L'b' ) ;
		pairing.SourceToEpsilon( L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">ab</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"nomatch\">bc</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( EpsilonsMixed )
	{
		CMatchStringPairing pairing ;

		pairing.QueryToEpsilon( L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.SourceToEpsilon( L'a' ) ;

		CStringA expected = "<span class=\"nomatch\">a</span>b" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "b<span class=\"nomatch\">c</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( EpsilonsMixedEnds )
	{
		CMatchStringPairing pairing ;

		pairing.NoMatch( L'b', L'b' ) ;
		pairing.QueryToEpsilon( L'c' ) ;
		pairing.Match( L'b', L'b' ) ;
		pairing.SourceToEpsilon( L'a' ) ;
		pairing.NoMatch( L'b', L'b' ) ;

		CStringA expected = "<span class=\"nomatch\">ba</span>b<span class=\"nomatch\">b</span>" ;

		CStringA actual = pairing.MarkupSource().c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = "<span class=\"nomatch\">b</span>b<span class=\"nomatch\">cb</span>" ;
		actual = pairing.MarkupQuery().c_str() ;
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
	BOOST_CHECK_EQUAL( L'0', narrow_num( L'‚O' ) ) ;
	BOOST_CHECK_EQUAL( L'1', narrow_num( L'‚P' ) ) ;
	BOOST_CHECK_EQUAL( L'2', narrow_num( L'‚Q' ) ) ;
	BOOST_CHECK_EQUAL( L'3', narrow_num( L'‚R' ) ) ;
	BOOST_CHECK_EQUAL( L'4', narrow_num( L'‚S' ) ) ;
	BOOST_CHECK_EQUAL( L'5', narrow_num( L'‚T' ) ) ;
	BOOST_CHECK_EQUAL( L'6', narrow_num( L'‚U' ) ) ;
	BOOST_CHECK_EQUAL( L'7', narrow_num( L'‚V' ) ) ;
	BOOST_CHECK_EQUAL( L'8', narrow_num( L'‚W' ) ) ;
	BOOST_CHECK_EQUAL( L'9', narrow_num( L'‚X' ) ) ;
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_replace_first )

BOOST_AUTO_TEST_CASE( foo_foo )
{
	wstring src, to, from ;
	CStringA expected, actual ;

	// foo -> foo
	src = L"foo" ;
	from = L"baz" ;
	to = L"bar" ;
	replace_first( src, from, to ) ;
	expected = "foo" ;
	actual = src.c_str() ;
	BOOST_CHECK_EQUAL( expected, actual ) ;

}

BOOST_AUTO_TEST_CASE( foo_bar )
{
	wstring src, to, from ;
	CStringA expected, actual ;
	// foo -> bar
	src = L"foo" ;
	from = L"foo" ;
	to = L"bar" ;
	replace_first( src, from, to ) ;
	expected = "bar" ;
	actual = src.c_str() ;
	BOOST_CHECK_EQUAL( expected, actual ) ;
}

BOOST_AUTO_TEST_CASE( foo_foo_2_bar_foo )
{
	wstring src, to, from ;
	// foo foo -> bar foo
	src = L"foo foo" ;
	from = L"foo" ;
	to = L"bar" ;
	replace_first( src, from, to ) ;
	CStringA expected = "bar foo" ;
	CStringA actual = src.c_str() ;
	BOOST_CHECK_EQUAL( expected, actual ) ;
}

BOOST_AUTO_TEST_CASE( modern_major_general )
{
	wstring src, to, from ;
	CStringA expected, actual ;

	// "I am the very model of a modern major general" -> "I ate the very model of a modern major general"
	src = L"I am the very model of a modern major general" ;
	from = L"m" ;
	to = L"te" ;
	replace_first( src, from, to ) ;
	expected = "I ate the very model of a modern major general" ;
	actual = src.c_str() ;
	BOOST_CHECK_EQUAL( expected, actual ) ;

}

BOOST_AUTO_TEST_SUITE_END()
