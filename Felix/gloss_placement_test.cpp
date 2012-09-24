#include "stdafx.h"
#include "gloss_placement.h"

#ifdef UNIT_TEST

#include "mem_tests.h"

using namespace mem_engine ;
using namespace placement ;
namespace gp = placement ;


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( gloss_placement_tests )


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


BOOST_AUTO_TEST_SUITE( gloss_placement_tests_pairing )

	using namespace mem_engine ;
	namespace gp = placement ;

	// find_hole
	BOOST_AUTO_TEST_CASE(empty_pairing)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(! finder.find_hole(pairings, holes)) ;
	}
	BOOST_AUTO_TEST_CASE(empty_hole)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(! finder.find_hole(pairings, holes)) ;
		BOOST_CHECK(holes.empty()) ;
	}
	// find_hole - start
	BOOST_AUTO_TEST_CASE(start_0)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 0u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 0u) ;
	}
	BOOST_AUTO_TEST_CASE(start_1)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 1u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 1u) ;
	}
	BOOST_AUTO_TEST_CASE(start_2)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 2u) ;
	}
	// find_hole - len
	BOOST_AUTO_TEST_CASE(start_0_len_3)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'a', NOMATCH, L'x')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 3u) ;
	}
	BOOST_AUTO_TEST_CASE(start_3_len_0)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'c', MATCH, L'c')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(! finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 3u) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 0u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 0u) ;
	}
	BOOST_AUTO_TEST_CASE(start_0_len_3_end_match)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 3u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 3u) ;
	}
	BOOST_AUTO_TEST_CASE(start_1_len_1)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 1u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 1u) ;
	}
	BOOST_AUTO_TEST_CASE(start_2_len_2)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK_EQUAL(holes.lhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.start, 2u) ;
		BOOST_CHECK_EQUAL(holes.lhs.len, 2u) ;
		BOOST_CHECK_EQUAL(holes.rhs.len, 2u) ;
	}

BOOST_AUTO_TEST_SUITE_END()


//////////////////////////////////////////////////////////////////////////
// Holes
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_holes )

	using namespace mem_engine ;
	namespace gp = placement ;
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

	// get_str
	BOOST_AUTO_TEST_CASE(get_str_wstr)
	{
		wstring expected = L"12" ;
		wstring text = L"xxx12xxx" ;
		placement::hole_t h(3, 2) ;
		BOOST_CHECK_EQUAL(h.get_str(text), expected) ;
	}

	// placement
	BOOST_AUTO_TEST_CASE(get_str_placement)
	{
		wstring expected = L"234" ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'1', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'2', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'3', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'4', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'5', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'6', MATCH, L'b')) ;
		placement::hole_t h(1, 3) ;
		BOOST_CHECK_EQUAL(h.get_str_source(pairings), expected) ;
	}
	BOOST_AUTO_TEST_CASE(get_str_placement_nulls)
	{
		wstring expected = L"24" ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'1', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'2', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(0, NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'4', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'5', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'6', MATCH, L'b')) ;
		placement::hole_t h(1, 3) ;
		BOOST_CHECK_EQUAL(h.get_str_source(pairings), expected) ;
	}

	BOOST_AUTO_TEST_CASE(get_str_placement_query)
	{
		wstring expected = L"axx" ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'1', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'2', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'3', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'4', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'5', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'6', MATCH, L'b')) ;
		placement::hole_t h(1, 3) ;
		BOOST_CHECK_EQUAL(h.get_str_query(pairings), expected) ;
	}
	BOOST_AUTO_TEST_CASE(get_str_placement_nulls_query)
	{
		wstring expected = L"x" ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'1', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'2', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(0, NOMATCH, 0)) ;
		pairings.push_back(gp::pairing_t(L'4', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'5', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'6', MATCH, L'b')) ;
		placement::hole_t h(2, 2) ;
		BOOST_CHECK_EQUAL(h.get_str_query(pairings), expected) ;
	}

BOOST_AUTO_TEST_SUITE_END()


//////////////////////////////////////////////////////////////////////////
// test_gloss
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_gloss )

	using namespace mem_engine ;
	namespace gp = placement ;

	BOOST_AUTO_TEST_CASE(init_empty)
	{
		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		placer.get_matches(matches, text) ;
		BOOST_CHECK(matches.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(get_matches_none)
	{
		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "bar", "bar") ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		placer.get_matches(matches, text) ;
		BOOST_CHECK(matches.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(get_matches_one)
	{
		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "foo", "bar") ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		BOOST_CHECK_EQUAL(1u, placer.get_matches(matches, text)) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_matches_two)
	{
		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "foo", "bar") ;
		add_record(mem, "foo", "gaz") ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		placer.get_matches(matches, text) ;
		BOOST_CHECK_EQUAL(2u, matches.size()) ;
	}
	// get_trans_subset
	BOOST_AUTO_TEST_CASE(get_trans_subset_one_one)
	{
		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "foo", "bar") ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		placer.get_matches(matches, text) ;
		BOOST_CHECK_EQUAL(1u, placer.get_trans_subset(matches, L"bar")) ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_trans_subset_two_one)
	{
		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "foo", "bar") ;
		add_record(mem, "foo", "gaz") ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		placer.get_matches(matches, text) ;
		placer.get_trans_subset(matches, L"bar") ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_trans_subset_two_one_two_hits)
	{
		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "foo", "bar") ;
		add_record(mem, "foo", "bar bar") ;
		gp::gloss_placer placer(memories) ;

		search_match_container matches ;
		wstring text = L"foo" ;
		BOOST_CHECK_EQUAL(2u, placer.get_matches(matches, text)) ;
		placer.get_trans_subset(matches, L"bar") ;
		BOOST_CHECK_EQUAL(1u, matches.size()) ;
	}
	// num_hits

	BOOST_AUTO_TEST_CASE(num_hits_0)
	{
		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		wstring needle = L"aaa" ;
		wstring haystack = L"bbb" ;

		BOOST_CHECK_EQUAL(0u, placer.num_hits(needle, haystack)) ;
	}
	BOOST_AUTO_TEST_CASE(num_hits_1)
	{
		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		wstring needle = L"aaa" ;
		wstring haystack = L"bbb aaa" ;

		BOOST_CHECK_EQUAL(1u, placer.num_hits(needle, haystack)) ;
	}
	BOOST_AUTO_TEST_CASE(num_hits_2)
	{
		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		wstring needle = L"aaa" ;
		wstring haystack = L"bbb aaa ccc aaa" ;

		BOOST_CHECK_EQUAL(2u, placer.num_hits(needle, haystack)) ;
	}

	// replace_trans_term

	BOOST_AUTO_TEST_CASE(replace_trans_term_start)
	{
		const wstring qword = L"after" ;
		const wstring trans_plain = L"before" ;
		wstring trans = L"before xxx" ;
		std::pair<wstring, wstring> segs(trans, trans) ;
		const wstring expected = L"<span class=\"placement\">after</span> xxx" ;

		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		placer.replace_trans_term(qword, trans_plain, segs) ;

		BOOST_CHECK_EQUAL(segs.second, expected) ;
	}

	BOOST_AUTO_TEST_CASE(replace_trans_term_middle)
	{
		const wstring qword = L"after" ;
		const wstring trans_plain = L"before" ;
		wstring trans = L"yyy before xxx" ;
		std::pair<wstring, wstring> segs(trans, trans) ;
		const wstring expected = L"yyy <span class=\"placement\">after</span> xxx" ;

		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		placer.replace_trans_term(qword, trans_plain, segs) ;

		BOOST_CHECK_EQUAL(segs.second, expected) ;
	}

	BOOST_AUTO_TEST_CASE(replace_trans_term_end)
	{
		const wstring qword = L"after" ;
		const wstring trans_plain = L"before" ;
		wstring trans = L"xxx before" ;
		std::pair<wstring, wstring> segs(trans, trans) ;
		const wstring expected = L"xxx <span class=\"placement\">after</span>" ;

		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		placer.replace_trans_term(qword, trans_plain, segs) ;

		BOOST_CHECK_EQUAL(segs.second, expected) ;
	}

	BOOST_AUTO_TEST_CASE(replace_trans_term_entities)
	{
		const wstring qword = L"after" ;
		const wstring trans_plain = L"before" ;
		wstring trans = L"before x < y" ;
		std::pair<wstring, wstring> segs(trans, trans) ;
		const wstring expected = L"<span class=\"placement\">after</span> x &lt; y" ;

		memory_list memories ;
		gp::gloss_placer placer(memories) ;

		placer.replace_trans_term(qword, trans_plain, segs) ;

		BOOST_CHECK_EQUAL(segs.second, expected) ;
	}

	// create_new_pairings
	BOOST_AUTO_TEST_CASE(create_new_pairings_xx_yy)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;

		memory_list memories ;
		gp::gloss_placer placer(memories) ;
		placer.create_new_pairings(pairings, holes) ;

		std::vector<pairing_t> pairvec ;
		pairvec.assign(pairings.begin(), pairings.end()) ;

		BOOST_CHECK_EQUAL(pairvec[0].source(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[1].source(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[2].source(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[3].source(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[4].source(), L'b') ;
		BOOST_CHECK_EQUAL(pairvec[5].source(), L'b') ;

		BOOST_CHECK_EQUAL(pairvec[0].query(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[1].query(), L'a') ;
		BOOST_CHECK_EQUAL(pairvec[2].query(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[3].query(), L'x') ;
		BOOST_CHECK_EQUAL(pairvec[4].query(), L'b') ;
		BOOST_CHECK_EQUAL(pairvec[5].query(), L'b') ;

		BOOST_CHECK_EQUAL(pairvec[2].match_type(), PLACEMENT) ;
	}

	// is_valid_placement
	BOOST_AUTO_TEST_CASE(get_trans_subset_one_one_true)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "yy", "source") ;
		add_record(mem, "xx", "query") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with source" ;

		search_match_container q_matches ;
		search_match_container s_matches ;
		BOOST_CHECK(placer.is_valid_placement(holes, pairings, trans, q_matches, s_matches)) ;
		BOOST_CHECK_EQUAL(1u, s_matches.size()) ;
		BOOST_CHECK_EQUAL(1u, q_matches.size()) ;
	}
	BOOST_AUTO_TEST_CASE(get_trans_subset_one_one_false)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "yy", "source") ;
		add_record(mem, "xx", "query") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with blah" ;

		search_match_container q_matches ;
		search_match_container s_matches ;
		BOOST_CHECK(!placer.is_valid_placement(holes, pairings, trans, q_matches, s_matches)) ;
	}
	BOOST_AUTO_TEST_CASE(get_trans_subset_zero_one_false)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "yy", "source") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with source" ;

		search_match_container q_matches ;
		search_match_container s_matches ;
		BOOST_CHECK(! placer.is_valid_placement(holes, pairings, trans, q_matches, s_matches)) ;
	}
	BOOST_AUTO_TEST_CASE(get_trans_subset_one_zero_false)
	{
		gp::hole_finder finder ;
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		placement::hole_pair_t holes ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "xx", "query") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with source" ;

		search_match_container q_matches ;
		search_match_container s_matches ;
		BOOST_CHECK(! placer.is_valid_placement(holes, pairings, trans, q_matches, s_matches)) ;
	}

	// place
	BOOST_AUTO_TEST_CASE(place_true)
	{
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "yy", "source") ;
		add_record(mem, "xx", "query") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with source" ;
		std::pair<wstring, wstring> segs(trans, trans) ;

		BOOST_CHECK(placer.place(pairings, segs)) ;
		wstring expected = L"End up with <span class=\"placement\">query</span>" ;
		BOOST_CHECK_EQUAL(segs.second, expected) ;
	}
	BOOST_AUTO_TEST_CASE(place_false_source)
	{
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "xx", "query") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with source" ;
		std::pair<wstring, wstring> segs(trans, trans) ;

		BOOST_CHECK(! placer.place(pairings, segs)) ;
	}
	BOOST_AUTO_TEST_CASE(place_false_query)
	{
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "yy", "source") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with source" ;
		std::pair<wstring, wstring> segs(trans, trans) ;

		BOOST_CHECK(! placer.place(pairings, segs)) ;
	}
	BOOST_AUTO_TEST_CASE(place_false_trans)
	{
		gp::pairings_t pairings ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'a', MATCH, L'a')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'y', NOMATCH, L'x')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;
		pairings.push_back(gp::pairing_t(L'b', MATCH, L'b')) ;

		memory_list memories ;
		memory_pointer mem(new memory_local(app_props::get_props())) ;
		memories.push_back(mem) ;
		add_record(mem, "yy", "source") ;
		add_record(mem, "xx", "query") ;
		gp::gloss_placer placer(memories) ;

		wstring trans = L"End up with blah" ;
		std::pair<wstring, wstring> segs(trans, trans) ;

		BOOST_CHECK(! placer.place(pairings, segs)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif