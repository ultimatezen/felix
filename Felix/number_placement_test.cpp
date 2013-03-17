#include "stdafx.h"
#include "number_placement.h"

#ifdef UNIT_TEST

#include "mem_tests.h"

using namespace mem_engine ;
using namespace placement ;
namespace gp = placement ;

void pair_up_num(gp::pairings_t &pairings, wstring source, wstring query)
{
	for (size_t i = 0 ; i < source.size() ; ++i)
	{
		const wchar_t s = source[i] ;
		const wchar_t q = query[i] ;

		if (s == L'*')
		{
			pairings.push_back(gp::pairing_t(0, NOMATCH, q)) ;
		}
		else if (q == L'*')
		{
			pairings.push_back(gp::pairing_t(s, NOMATCH, 0)) ;
		}
		else if ( s == q )
		{
			pairings.push_back(gp::pairing_t(s, MATCH, q)) ;
		}
		else
		{
			pairings.push_back(gp::pairing_t(s, NOMATCH, q)) ;
		}
	}
}
void pair_up_num(match_string_pairing &pairing, wstring source, wstring query)
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


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( number_placement_tests )

	// get num
	BOOST_AUTO_TEST_CASE(get_num_source)
	{
		gp::number_placer placer ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', NOMATCH, L'y')) ;
		pairings.push_back(gp::pairing_t(L'1', NOMATCH, L'2')) ;
		pairings.push_back(gp::pairing_t(L'1', NOMATCH, L'2')) ;
		pairings.push_back(gp::pairing_t(L'z', NOMATCH, L'z')) ;

		wstring expected = L"11" ;
		std::set<size_t> positions ;
		size_t pos = 2 ;
		wstring actual = placer.get_num(pairings, pos, gp::SOURCE, positions) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(2u, positions.size()) ;
	}

	BOOST_AUTO_TEST_CASE(get_num_query)
	{
		gp::number_placer placer ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', NOMATCH, L'y')) ;
		pairings.push_back(gp::pairing_t(L'1', NOMATCH, L'2')) ;
		pairings.push_back(gp::pairing_t(L'1', NOMATCH, L'2')) ;
		pairings.push_back(gp::pairing_t(L'z', NOMATCH, L'z')) ;

		wstring expected = L"22" ;
		std::set<size_t> positions ;
		size_t pos = 2 ;
		wstring actual = placer.get_num(pairings, pos, gp::QUERY, positions) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		BOOST_CHECK_EQUAL(2u, positions.size()) ;
	}

	BOOST_AUTO_TEST_CASE(narrow_num_single_byte)
	{
		gp::number_placer placer ;

		wstring num = L"1234.56" ;
		wstring actual = placer.narrow_num_str(num) ;
		wstring expected = L"1234.56" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE(narrow_num_double_byte)
	{
		gp::number_placer placer ;

		wstring num = L"１２３４.５６" ;
		wstring actual = placer.narrow_num_str(num) ;
		wstring expected = L"1234.56" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_number_placement_tests_Placement )

	BOOST_AUTO_TEST_CASE( DoublePlacement )
	{
		gp::number_placer placer ;

		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;
		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'2', L'3' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 3.0f, calc_score(pairing.get()), 0.000001 ) ;

		trans_pair trans( L"2a1", L"2a1" ) ;

		BOOST_CHECK( placer.place(pairing.get(), trans)) ;
		wstring expected = L"3a2" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = L"<span class=\"placement\">3</span>a<span class=\"placement\">2</span>" ;
		actual = trans.second ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

	}
	BOOST_AUTO_TEST_CASE( TwoBytePlacement )
	{
		gp::number_placer placer ;
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'３', L'2' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 2.0f, calc_score(pairing.get()), 0.000001 ) ;

		trans_pair trans( L"3a", L"3a" ) ;

		BOOST_CHECK( placer.place(pairing.get(), trans)) ;
		wstring expected = L"2a" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = L"<span class=\"placement\">2</span>a" ;
		actual = trans.second ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( BothDoubleByte )
	{
		gp::number_placer placer ;
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;
		pairing.no_match( L'３', L'２' ) ;
		BOOST_CHECK_CLOSE( (double)1.0f / 2.0f, calc_score(pairing.get()), 0.000001 ) ;

		trans_pair trans( L"3a", L"3a" ) ;

		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( placer.place(parings, trans)) ;
		CStringA expected = "2a" ;
		CStringA actual = trans.first.c_str() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_EQUAL( trans.second, L"<span class=\"placement\">2</span>a" ) ;
		BOOST_CHECK_EQUAL( mark_up(parings, SOURCE), L"<span class=\"placement\">２</span>a" ) ;
		BOOST_CHECK_EQUAL( mark_up(parings, SOURCE), L"<span class=\"placement\">２</span>a" ) ;
	}

	BOOST_AUTO_TEST_CASE( TrivialPlacementFalse )
	{
		gp::number_placer placer ;
		match_string_pairing pairing ;

		pairing.match( L'a', L'a' ) ;

		trans_pair trans( L"a", L"a" ) ;

		BOOST_CHECK( ! placer.place(pairing.get(), trans) ) ;
	}
	BOOST_AUTO_TEST_CASE( placement_false_commas )
	{
		gp::number_placer placer ;
		match_string_pairing pairing ;

		pair_up_num(pairing, L"I have,  apples", L"I have 3 apples") ;

		trans_pair trans( L"I have 5 apples", L"I have 5 apples" ) ;

		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( ! placer.place(parings, trans)) ;
	}
	BOOST_AUTO_TEST_CASE( trival_placement_bad_entities )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;

		trans_pair trans( L"<a> & <b>1", L"<a> & <b>1" ) ;

		gp::number_placer placer ;
		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( placer.place(parings, trans)) ;
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

		trans_pair trans( L"1", L"1" ) ;

		gp::number_placer placer ;
		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( ! placer.place(parings, trans)) ;
	}
	BOOST_AUTO_TEST_CASE( TrivialPlacementTrue )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;

		trans_pair trans( L"1", L"1" ) ;

		gp::number_placer placer ;
		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( placer.place(parings, trans)) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementNoTransMatch )
	{
		match_string_pairing pairing ;

		pairing.match( L'1', L'1' ) ;

		trans_pair trans( L"a", L"a" ) ;

		gp::number_placer placer ;
		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( ! placer.place(parings, trans)) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementNoTransNoMatch )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;

		trans_pair trans( L"a", L"a" ) ;

		gp::number_placer placer ;
		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( ! placer.place(parings, trans)) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumNoMatch )
	{
		match_string_pairing pairing ;

		pairing.no_match( L'1', L'2' ) ;
		pairing.match( L'1', L'1' ) ;

		trans_pair trans( L"a1", L"a1" ) ;

		gp::number_placer placer ;
		pairings_t &parings = pairing.get() ;
		BOOST_CHECK( ! placer.place(parings, trans)) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatch )
	{
		pairings_t pairs ;

		pair_up_num(pairs, L"11", L"12") ; 

		trans_pair trans( L"a11", L"a11" ) ;

		gp::number_placer placer ;
		BOOST_CHECK( placer.place(pairs, trans)) ;
		wstring expected = L"a12" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatch_no_match_when_found_twice )
	{
		pairings_t pairs ;

		pair_up_num(pairs, L"11", L"21") ; 

		trans_pair trans( L"11a11", L"11a11" ) ;

		gp::number_placer placer ;

		BOOST_CHECK( ! placer.place(pairs, trans)) ;

		trans.first = L"1111" ;
		BOOST_CHECK( ! placer.place(pairs, trans)) ;
	}

	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchLonger1 )
	{
		pairings_t pairs ;
		pair_up_num(pairs, L"aa11aa", L"ab12ab") ;

		trans_pair trans( L"a11", L"a11" ) ;

		gp::number_placer placer ;
		BOOST_CHECK( placer.place(pairs, trans)) ;
		wstring expected = L"a12" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}
	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchLonger2 )
	{
		pairings_t pairs ;

		pair_up_num(pairs, L"aa11aa", L"ab12ab") ;

		trans_pair trans( L"1111", L"1111" ) ;
		gp::number_placer placer ;
		BOOST_CHECK( ! placer.place(pairs, trans)) ;

		trans.first = L"11a11" ;
		BOOST_CHECK( ! placer.place(pairs, trans)) ;
	}

	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchEpsilons1 )
	{
		pairings_t pairs ;

		pair_up_num(pairs, 
			L"aa*11aa", 
			L"ab32*a*") ;

		// a11
		trans_pair trans( L"a11", L"a11" ) ;

		gp::number_placer placer ;
		BOOST_CHECK( placer.place(pairs, trans)) ;

		wstring expected = L"a32" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = L"a<span class=\"nomatch\">a</span><span class=\"placement\">32</span>a<span class=\"nomatch\">a</span>" ;
		actual = mark_up(pairs, SOURCE) ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		expected = L"a<span class=\"nomatch\">b</span><span class=\"placement\">32</span>a" ;
		actual = mark_up(pairs, QUERY) ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

	}


	BOOST_AUTO_TEST_CASE( PlacementLongNumMatchEpsilons2 )
	{
		pairings_t pairs ;

		pair_up_num(pairs, 
			L"aa*11aa", 
			L"ab32*a*") ;

		// 1111
		trans_pair trans( L"1111", L"1111" ) ;
		gp::number_placer placer ;
		BOOST_CHECK( ! placer.place(pairs, trans)) ;

		// 11a11
		trans.first = L"11a11" ;
		BOOST_CHECK( ! placer.place(pairs, trans)) ;

	}

	BOOST_AUTO_TEST_CASE( WithPunct )
	{
		pairings_t pairs ;

		pair_up_num(pairs, 
			L"a12,3a", 
			L"a45,6a") ;

		trans_pair trans( L"c12,3c", L"c12,3c" ) ;

		gp::number_placer placer ;
		BOOST_CHECK( placer.place(pairs, trans)) ;
		wstring expected = L"c45,6c" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairs), 0.00001 ) ;
	}
	BOOST_AUTO_TEST_CASE( WithJpn )
	{
		pairings_t pairs ;

		pair_up_num(pairs, 
			L"a2‚©3a", 
			L"a5‚©6a") ;

		trans_pair trans( L"c2a3c", L"c2a3c" ) ;

		gp::number_placer placer ;
		BOOST_CHECK( placer.place(pairs, trans)) ;
		wstring expected = L"c5a6c" ;
		wstring actual = trans.first ;
		BOOST_CHECK_EQUAL( expected, actual ) ;

		BOOST_CHECK_CLOSE( (double)1.0f, calc_score(pairs), 0.00001 ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_number_placement_tests_backslashes )


	//////////////////////////////////////////////////////////////////////////
	// tests for bug #274 (placement with backslash
	//////////////////////////////////////////////////////////////////////////
	BOOST_AUTO_TEST_CASE( backslash_error_extra_string_trans )
	{
		match_string_pairing pairing ;

		// source: \850,000 iÅž\892,500j
		// query:  \1,300,000 iÅž\1,365,000j
		// trans: \850,000 (\892,500 with tax)

		wstring source = L"\\**850,000 (税込み\\**892,500)" ;
		wstring query  = L"\\1,300,000 (税込み\\1,365,000)" ;

		BOOST_CHECK_EQUAL(source.size(), query.size()) ;

		pair_up_num(pairing, source, query) ;

		// a11
		trans_pair trans( L"\\850,000 (\\892,500 with tax)", 
			L"\\850,000 (\\892,500 with tax)" ) ;

		pairings_t &pairs = pairing.get() ;
		gp::number_placer placer ;

		BOOST_CHECK( placer.place(pairs, trans) ) ;

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

		wstring source = L"\\**850,000（税込み\\**892,500）" ;
		wstring query  = L"\\1,300,000（税込み\\1,365,000）" ;

		pair_up_num(pairing, source, query) ;

		// a11
		trans_pair trans( L"\\850,000 (\\892,500 with tax)", 
			L"\\850,000 (\\892,500 with tax)" ) ;

		pairings_t &pairs = pairing.get() ;
		gp::number_placer placer ;

		BOOST_CHECK( placer.place(pairs, trans) ) ;

		wstring expected = L"\\<span class=\"placement\">1,300,000</span>（税込み\\<span class=\"placement\">1,365,000</span>）" ;
		wstring actual = pairing.mark_up_source() ;
		BOOST_CHECK_EQUAL( expected, actual ) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif