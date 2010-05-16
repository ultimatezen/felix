#include "StdAfx.h"
#include "destination_parser.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestWebBrowser )

	BOOST_AUTO_TEST_CASE( no_colon )
	{
		destination_parser parser ;
		wstring destination = L"foo" ;
		LPMSG expected = NULL ;
		BOOST_CHECK_EQUAL(expected, parser.parse(destination)) ;
	}

	BOOST_AUTO_TEST_CASE( category_not_number )
	{
		destination_parser parser ;
		wstring destination = L"foo:bar" ;
		LPMSG expected = NULL ;
		BOOST_CHECK_EQUAL(expected, parser.parse(destination)) ;
	}

	BOOST_AUTO_TEST_CASE( sub_category_not_number )
	{
		destination_parser parser ;
		wstring destination = L"145:bar" ;
		LPMSG expected = NULL ;
		BOOST_CHECK_EQUAL(expected, parser.parse(destination)) ;
	}

	BOOST_AUTO_TEST_CASE( parse_155_103 )
	{
		destination_parser parser ;
		wstring destination = L"155:103" ;
		LPMSG msg = parser.parse(destination) ;
		BOOST_CHECK_EQUAL(155u, msg->wParam) ;
		BOOST_CHECK_EQUAL(103, msg->lParam) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
