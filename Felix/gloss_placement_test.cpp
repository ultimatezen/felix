#include "stdafx.h"
#include "gloss_placement.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( gloss_placement_tests )

	using namespace mem_engine ;

	// find_hole
	BOOST_AUTO_TEST_CASE(empty_lhs)
	{
		gloss_placement placement ;
		wstring lhs = L"" ;
		wstring rhs = L"abc" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(! placement.find_hole(lhs, rhs, holes)) ;
	}
	BOOST_AUTO_TEST_CASE(empty_rhs)
	{
		gloss_placement placement ;
		wstring lhs = L"abc" ;
		wstring rhs = L"" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(! placement.find_hole(lhs, rhs, holes)) ;
	}

	// find_hole - start
	BOOST_AUTO_TEST_CASE(start_0)
	{
		gloss_placement placement ;
		wstring lhs = L"xxx" ;
		wstring rhs = L"yyy" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 0u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 0u) ;
	}
	BOOST_AUTO_TEST_CASE(start_1)
	{
		gloss_placement placement ;
		wstring lhs = L"axxx" ;
		wstring rhs = L"ayyy" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 1u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 1u) ;
	}
	BOOST_AUTO_TEST_CASE(start_2)
	{
		gloss_placement placement ;
		wstring lhs = L"abxxx" ;
		wstring rhs = L"abyyy" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 2u) ;
	}
	// find_hole - len
	BOOST_AUTO_TEST_CASE(start_0_len_3)
	{
		gloss_placement placement ;
		wstring lhs = L"xxx" ;
		wstring rhs = L"yyy" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 3u) ;
	}
	BOOST_AUTO_TEST_CASE(start_3_len_0)
	{
		gloss_placement placement ;
		wstring lhs = L"aaa" ;
		wstring rhs = L"aaa" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(! placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 3u) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 0u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 0u) ;
	}
	BOOST_AUTO_TEST_CASE(start_0_len_3_end_match)
	{
		gloss_placement placement ;
		wstring lhs = L"xxxaa" ;
		wstring rhs = L"yyyaa" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 3u) ;
	}
	BOOST_AUTO_TEST_CASE(start_1_len_1)
	{
		gloss_placement placement ;
		wstring lhs = L"axbb" ;
		wstring rhs = L"aybb" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 1u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 1u) ;
	}
	BOOST_AUTO_TEST_CASE(start_2_len_2)
	{
		gloss_placement placement ;
		wstring lhs = L"abxxcd" ;
		wstring rhs = L"abyycd" ;
		gloss_placement::hole_pair_t holes ;
		BOOST_CHECK(placement.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 2u) ;
	}
	// str_hole
	BOOST_AUTO_TEST_CASE(str_hole)
	{
		gloss_placement placement ;
		wstring expected = L"12" ;
		wstring text = L"xxx12xxx" ;
		gloss_placement::hole_t h(3, 2) ;
		BOOST_CHECK_EQUAL(placement.str_hole(text, h), expected) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_holes )

	using namespace mem_engine ;
	BOOST_AUTO_TEST_CASE(hole_init_empty)
	{
		gloss_placement::hole_t hole ;
		BOOST_CHECK(hole.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(zero_len_empty)
	{
		gloss_placement::hole_t hole(3u, 0u) ;
		BOOST_CHECK(hole.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(len_2_not_empty)
	{
		gloss_placement::hole_t hole(3u, 1u) ;
		BOOST_CHECK(! hole.empty()) ;
	}

	BOOST_AUTO_TEST_CASE(hole_pair_init_empty)
	{
		gloss_placement::hole_pair_t holepair ;
		BOOST_CHECK(holepair.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(pair_one_empty)
	{
		gloss_placement::hole_t hole1(3u, 0u) ;
		gloss_placement::hole_t hole2(3u, 3u) ;
		gloss_placement::hole_pair_t holepair(hole1, hole2) ;
		BOOST_CHECK(holepair.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(pair_neither_empty)
	{
		gloss_placement::hole_t hole1(3u, 1u) ;
		gloss_placement::hole_t hole2(3u, 3u) ;
		gloss_placement::hole_pair_t holepair(hole1, hole2) ;
		BOOST_CHECK(! holepair.empty()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif