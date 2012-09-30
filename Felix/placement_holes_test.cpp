#include "stdafx.h"
#include "placement_holes.h"

#ifdef UNIT_TEST

#include "mem_tests.h"

using namespace mem_engine ;
using namespace placement ;
namespace gp = placement ;

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( placement_holes_test )

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

BOOST_AUTO_TEST_SUITE_END()

#endif