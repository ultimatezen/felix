#include "StdAfx.h"
#include "pagination.h"

#include <boost/test/unit_test.hpp>
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
BOOST_AUTO_TEST_SUITE_END()