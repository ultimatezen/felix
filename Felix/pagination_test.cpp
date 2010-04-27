#include "StdAfx.h"
#include "pagination.h"

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestPaginator )

	// getting basic info
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

		BOOST_CHECK(text.find(L"Previous") != wstring::npos) ;
		BOOST_CHECK(text.find(L"Next") != wstring::npos) ;
		BOOST_CHECK(text.find(L"/next_page") == wstring::npos) ;
		BOOST_CHECK(text.find(L"/prev_page") == wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( first_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		wstring text = get_pagination_text(paginator) ;

		BOOST_CHECK(text.find(L"Previous") != wstring::npos) ;
		BOOST_CHECK(text.find(L"Next") != wstring::npos) ;
		BOOST_CHECK(text.find(L"/next_page") != wstring::npos) ;
		BOOST_CHECK(text.find(L"/prev_page") == wstring::npos) ;
	}
	BOOST_AUTO_TEST_CASE( next_page_50)
	{
		Paginator paginator ;
		paginator.set_num_records(50) ;
		paginator.next_page() ;
		wstring text = get_pagination_text(paginator) ;

		BOOST_CHECK(text.find(L"Previous") != wstring::npos) ;
		BOOST_CHECK(text.find(L"Next") != wstring::npos) ;
		BOOST_CHECK(text.find(L"/next_page") != wstring::npos) ;
		BOOST_CHECK(text.find(L"/prev_page") != wstring::npos) ;
	}
BOOST_AUTO_TEST_SUITE_END()