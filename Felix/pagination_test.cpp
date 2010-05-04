#include "StdAfx.h"
#include "pagination.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestPaginator )

	// getting basic info
	BOOST_AUTO_TEST_CASE(init_has_next)
	{
		Paginator paginator ;
		BOOST_CHECK(! paginator.has_next()) ;
	}


	BOOST_AUTO_TEST_CASE( default_1)
	{
		Paginator paginator ;
		paginator.set_num_records(1) ;
		BOOST_CHECK_EQUAL(1, (int)paginator.get_num_pages()) ;
		BOOST_CHECK_EQUAL(0, (int)paginator.get_current_page()) ;
		BOOST_CHECK_EQUAL(0, (int)paginator.get_start()) ;
		BOOST_CHECK_EQUAL(1, (int)paginator.get_end()) ;
		BOOST_CHECK(! paginator.has_next()) ;
		BOOST_CHECK(! paginator.has_prev()) ;
	}

	BOOST_AUTO_TEST_CASE( next_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		BOOST_CHECK(! paginator.has_prev()) ;
		BOOST_CHECK(paginator.has_next()) ;
		paginator.next_page() ;
		BOOST_CHECK_EQUAL(3, (int)paginator.get_num_pages()) ;
		BOOST_CHECK_EQUAL(1, (int)paginator.get_current_page()) ;
		BOOST_CHECK_EQUAL(20, (int)paginator.get_start()) ;
		BOOST_CHECK_EQUAL(40, (int)paginator.get_end()) ;
		BOOST_CHECK(paginator.has_prev()) ;
	}
	BOOST_AUTO_TEST_CASE( next_page_30)
	{
		Paginator paginator ;
		paginator.set_num_records(30) ;
		paginator.next_page() ;
		BOOST_CHECK_EQUAL(1, (int)paginator.get_current_page()) ;
		BOOST_CHECK_EQUAL(20, (int)paginator.get_start()) ;
		BOOST_CHECK_EQUAL(30, (int)paginator.get_end()) ;
		BOOST_CHECK(paginator.has_prev()) ;
		BOOST_CHECK(! paginator.has_next()) ;
	}

	BOOST_AUTO_TEST_CASE( prev_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		paginator.next_page() ;
		BOOST_CHECK_EQUAL(1, (int)paginator.get_current_page()) ;
		paginator.prev_page() ;
		BOOST_CHECK_EQUAL(0, (int)paginator.get_current_page()) ;
		BOOST_CHECK_EQUAL(0, (int)paginator.get_start()) ;
		BOOST_CHECK_EQUAL(20, (int)paginator.get_end()) ;
		BOOST_CHECK(! paginator.has_prev()) ;
		BOOST_CHECK(paginator.has_next()) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	// pagination text
BOOST_AUTO_TEST_SUITE( test_get_pagination_text )
	BOOST_AUTO_TEST_CASE( default_1)
	{
		Paginator paginator ;
		paginator.set_num_records(1) ;
		wstring text = get_pagination_text(paginator) ;

		BOOST_CHECK(boost::contains(text, L"<p class=\"paginator\">")) ;
		BOOST_CHECK(boost::contains(text, L"<span class=\"this-page\" title=\"Current page\">1</span>")) ;
		BOOST_CHECK(boost::contains(text, L"&nbsp;&nbsp;&nbsp;&nbsp;1 to 1 of 1 matches.")) ;
		BOOST_CHECK(boost::contains(text, L"</p>")) ;

		BOOST_CHECK(! boost::contains(text, L"/next_page")) ;
		BOOST_CHECK(! boost::contains(text, L"/prev_page")) ;
		BOOST_CHECK(! boost::contains(text, L"2\">2</a>")) ;

	}
	BOOST_AUTO_TEST_CASE( first_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		wstring text = get_pagination_text(paginator) ;

		BOOST_CHECK(boost::contains(text, L"<p class=\"paginator\">")) ;
		BOOST_CHECK(boost::contains(text, L"<span class=\"this-page\" title=\"Current page\">1</span>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/2/goto_page\" title=\"Go to page 2\">2</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/3/goto_page\" title=\"Go to page 3\">3</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/next_page\" title=\"Go to next page\">&gt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/last_page\" title=\"Go to last page\">&gt;&gt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"&nbsp;&nbsp;&nbsp;&nbsp;1 to 20 of 50 matches.")) ;
		BOOST_CHECK(boost::contains(text, L"</p>")) ;

		BOOST_CHECK(!boost::contains(text, L"/prev_page")) ;
		BOOST_CHECK(!boost::contains(text, L"/first_page")) ;
		BOOST_CHECK(! boost::contains(text, L">4</a>")) ;
	}
	BOOST_AUTO_TEST_CASE( next_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		paginator.next_page() ;
		wstring text = get_pagination_text(paginator) ;

		BOOST_CHECK(boost::contains(text, L"<p class=\"paginator\">")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/first_page\" title=\"Go to first page\">&lt;&lt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/prev_page\" title=\"Go to previous page\">&lt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/1/goto_page\" title=\"Go to page 1\">1</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<span class=\"this-page\" title=\"Current page\">2</span>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/3/goto_page\" title=\"Go to page 3\">3</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/next_page\" title=\"Go to next page\">&gt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/last_page\" title=\"Go to last page\">&gt;&gt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"&nbsp;&nbsp;&nbsp;&nbsp;21 to 40 of 50 matches.")) ;
		BOOST_CHECK(boost::contains(text, L"</p>")) ;
	}

	BOOST_AUTO_TEST_CASE( page_20_of_100)
	{
		Paginator paginator ;
		paginator.set_num_records(100u * 20u) ;
		paginator.goto_page(19) ;
		wstring text = get_pagination_text(paginator) ;

		BOOST_CHECK(boost::contains(text, L"<p class=\"paginator\">")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/first_page\" title=\"Go to first page\">&lt;&lt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/prev_page\" title=\"Go to previous page\">&lt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 15\">15</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 16\">16</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 17\">17</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 18\">18</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 19\">19</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<span class=\"this-page\" title=\"Current page\">20</span>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 21\">21</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 22\">22</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 23\">23</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 24\">24</a>")) ;
		BOOST_CHECK(boost::contains(text, L"Go to page 25\">25</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/next_page\" title=\"Go to next page\">&gt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"<a href=\"/last_page\" title=\"Go to last page\">&gt;&gt;</a>")) ;
		BOOST_CHECK(boost::contains(text, L"&nbsp;&nbsp;&nbsp;&nbsp;381 to 400 of 2,000 matches.")) ;
		BOOST_CHECK(boost::contains(text, L"</p>")) ;

		BOOST_CHECK(! boost::contains(text, L"Go to page 14\">14</a>")) ;
		BOOST_CHECK(! boost::contains(text, L"Go to page 26\">26</a>")) ;
	}


BOOST_AUTO_TEST_SUITE_END()

// make_window
BOOST_AUTO_TEST_SUITE( test_make_window )
	
	BOOST_AUTO_TEST_CASE(test_0_5_10)
	{
		window_range winrange = make_window(0, 5, 10) ;
		BOOST_CHECK_EQUAL(winrange.first, 0u) ;
		BOOST_CHECK_EQUAL(winrange.second, 5u) ;
	}
	BOOST_AUTO_TEST_CASE(test_0_10_10)
	{
		window_range winrange = make_window(0, 10, 10) ;
		BOOST_CHECK_EQUAL(winrange.first, 0u) ;
		BOOST_CHECK_EQUAL(winrange.second, 10u) ;
	}
	BOOST_AUTO_TEST_CASE(test_0_15_10)
	{
		window_range winrange = make_window(0, 15, 10) ;
		BOOST_CHECK_EQUAL(winrange.first, 0u) ;
		BOOST_CHECK_EQUAL(winrange.second, 10u) ;
	}
	BOOST_AUTO_TEST_CASE(test_5_15_10)
	{
		window_range winrange = make_window(5, 15, 10) ;
		BOOST_CHECK_EQUAL(winrange.first, 0u) ;
		BOOST_CHECK_EQUAL(winrange.second, 10u) ;
	}
	BOOST_AUTO_TEST_CASE(test_10_15_10)
	{
		window_range winrange = make_window(10, 15, 10) ;
		BOOST_CHECK_EQUAL(winrange.first, 5u) ;
		BOOST_CHECK_EQUAL(winrange.second, 15u) ;
	}
	BOOST_AUTO_TEST_CASE(test_20_23_10)
	{
		window_range winrange = make_window(20, 23, 10) ;
		BOOST_CHECK_EQUAL(winrange.first, 13u) ;
		BOOST_CHECK_EQUAL(winrange.second, 23u) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
