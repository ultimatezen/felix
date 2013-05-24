#include "stdafx.h"
#include "html_processing.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_html_processing )

BOOST_AUTO_TEST_CASE( test_init )
{
	html_processing processing ;
	BOOST_CHECK ( ! processing.is_underline_specified() ) ; 
	BOOST_CHECK ( ! processing.is_italic_specified() ) ; 
	BOOST_CHECK ( ! processing.is_bold_specified() ) ; 
	BOOST_CHECK ( ! processing.is_font_size_specified() ) ; 
	BOOST_CHECK ( ! processing.is_fore_color_specified() ) ; 
}


BOOST_AUTO_TEST_CASE( test_html_size_to_point_size )
{
	BOOST_CHECK_EQUAL(8u, html_size_to_point_size(1u)) ;
	BOOST_CHECK_EQUAL(10u, html_size_to_point_size(2u)) ;
	BOOST_CHECK_EQUAL(12u, html_size_to_point_size(3u)) ;
	BOOST_CHECK_EQUAL(14u, html_size_to_point_size(4u)) ;
	BOOST_CHECK_EQUAL(18u, html_size_to_point_size(5u)) ;
	BOOST_CHECK_EQUAL(24u, html_size_to_point_size(6u)) ;
	BOOST_CHECK_EQUAL(36u, html_size_to_point_size(7u)) ;
}

BOOST_AUTO_TEST_CASE( test_html_size_to_point_size_out_of_bounds )
{
	BOOST_CHECK_EQUAL(8u, html_size_to_point_size(0u)) ;
	BOOST_CHECK_EQUAL(36u, html_size_to_point_size(8u)) ;
}


BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_8 )
{
	BOOST_CHECK_EQUAL(1u, point_size_to_html_size(8u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_10 )
{
	BOOST_CHECK_EQUAL(2u, point_size_to_html_size(10u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_11 )
{
	BOOST_CHECK_EQUAL(2u, point_size_to_html_size(11u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_12 )
{
	BOOST_CHECK_EQUAL(3u, point_size_to_html_size(12u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_14 )
{
	BOOST_CHECK_EQUAL(4u, point_size_to_html_size(14u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_18 )
{
	BOOST_CHECK_EQUAL(5u, point_size_to_html_size(18u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_19 )
{
	BOOST_CHECK_EQUAL(5u, point_size_to_html_size(19u)) ;
}


BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_24 )
{
	BOOST_CHECK_EQUAL(6u, point_size_to_html_size(24u)) ;
}
BOOST_AUTO_TEST_CASE( test_point_size_to_html_size_36 )
{
	BOOST_CHECK_EQUAL(7u, point_size_to_html_size(36u)) ;
}


BOOST_AUTO_TEST_SUITE_END()
