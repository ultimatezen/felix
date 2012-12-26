#include "stdafx.h"
#include "gloss_placement.h"

#ifdef UNIT_TEST

#include "mem_tests.h"

using namespace mem_engine ;
using namespace placement ;
namespace gp = placement ;


#define MAKE_A_HOLE(X) 	hole_pair_t X ; \
	hole_finder finder ;\
	BOOST_CHECK(finder.find_hole(pairings, holes)) ;

#include <boost/test/unit_test.hpp>


//////////////////////////////////////////////////////////////////////////
// gloss_placer
//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( test_gloss_placer )

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

		gp::create_new_pairings(pairings, holes.lhs) ;

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

		MAKE_A_HOLE(holes) ;
		BOOST_CHECK(placer.place(pairings, segs, holes)) ;
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


		hole_pair_t holes ;
		hole_finder finder ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK(! placer.place(pairings, segs, holes)) ;

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

		wstring trans = L"End up with query" ;
		std::pair<wstring, wstring> segs(trans, trans) ;


		hole_pair_t holes ;
		hole_finder finder ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK(! placer.place(pairings, segs, holes)) ;
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

		wstring trans = L"End up with trans" ;
		std::pair<wstring, wstring> segs(trans, trans) ;


		hole_pair_t holes ;
		hole_finder finder ;
		BOOST_CHECK(finder.find_hole(pairings, holes)) ;
		BOOST_CHECK(! placer.place(pairings, segs, holes)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif