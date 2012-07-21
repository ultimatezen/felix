#include "stdafx.h"
#include "gloss_placement.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( gloss_placement_tests )

	using namespace mem_engine ;
	namespace gp = placement ;

	// find_hole
	BOOST_AUTO_TEST_CASE(empty_lhs)
	{
		gp::hole_finder finder ;
		wstring lhs = L"" ;
		wstring rhs = L"abc" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(! finder.find_hole(lhs, rhs, holes)) ;
	}
	BOOST_AUTO_TEST_CASE(empty_rhs)
	{
		gp::hole_finder finder ;
		wstring lhs = L"abc" ;
		wstring rhs = L"" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(! finder.find_hole(lhs, rhs, holes)) ;
	}

	// find_hole - start
	BOOST_AUTO_TEST_CASE(start_0)
	{
		gp::hole_finder finder ;
		wstring lhs = L"xxx" ;
		wstring rhs = L"yyy" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 0u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 0u) ;
	}
	BOOST_AUTO_TEST_CASE(start_1)
	{
		gp::hole_finder finder ;
		wstring lhs = L"axxx" ;
		wstring rhs = L"ayyy" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 1u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 1u) ;
	}
	BOOST_AUTO_TEST_CASE(start_2)
	{
		gp::hole_finder finder ;
		wstring lhs = L"abxxx" ;
		wstring rhs = L"abyyy" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 2u) ;
	}
	// find_hole - len
	BOOST_AUTO_TEST_CASE(start_0_len_3)
	{
		gp::hole_finder finder ;
		wstring lhs = L"xxx" ;
		wstring rhs = L"yyy" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 3u) ;
	}
	BOOST_AUTO_TEST_CASE(start_3_len_0)
	{
		gp::hole_finder finder ;
		wstring lhs = L"aaa" ;
		wstring rhs = L"aaa" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(! finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 3u) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 0u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 0u) ;
	}
	BOOST_AUTO_TEST_CASE(start_0_len_3_end_match)
	{
		gp::hole_finder finder ;
		wstring lhs = L"xxxaa" ;
		wstring rhs = L"yyyaa" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 3u) ;
	}
	BOOST_AUTO_TEST_CASE(start_1_len_1)
	{
		gp::hole_finder finder ;
		wstring lhs = L"axbb" ;
		wstring rhs = L"aybb" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 1u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 1u) ;
	}
	BOOST_AUTO_TEST_CASE(start_2_len_2)
	{
		gp::hole_finder finder ;
		wstring lhs = L"abxxcd" ;
		wstring rhs = L"abyycd" ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(lhs, rhs, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 2u) ;
	}
	// str_hole
	BOOST_AUTO_TEST_CASE(str_hole)
	{
		wstring expected = L"12" ;
		wstring text = L"xxx12xxx" ;
		placement::hole_t h(3, 2) ;
		BOOST_CHECK_EQUAL(h.str_hole(text), expected) ;
	}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_holes )

	using namespace mem_engine ;
	BOOST_AUTO_TEST_CASE(hole_init_empty)
	{
		placement::hole_t hole ;
		BOOST_CHECK(hole.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(zero_len_empty)
	{
		placement::hole_t hole(3u, 0u) ;
		BOOST_CHECK(hole.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(len_2_not_empty)
	{
		placement::hole_t hole(3u, 1u) ;
		BOOST_CHECK(! hole.empty()) ;
	}

	BOOST_AUTO_TEST_CASE(hole_pair_init_empty)
	{
		placement::hole_pair_t holepair ;
		BOOST_CHECK(holepair.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(pair_one_empty)
	{
		placement::hole_t hole1(3u, 0u) ;
		placement::hole_t hole2(3u, 3u) ;
		placement::hole_pair_t holepair(hole1, hole2) ;
		BOOST_CHECK(holepair.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(pair_neither_empty)
	{
		placement::hole_t hole1(3u, 1u) ;
		placement::hole_t hole2(3u, 3u) ;
		placement::hole_pair_t holepair(hole1, hole2) ;
		BOOST_CHECK(! holepair.empty()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif