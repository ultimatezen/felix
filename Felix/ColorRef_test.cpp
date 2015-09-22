#include "StdAfx.h"
#include "ColorRef.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CColorRefTestCases )

	BOOST_AUTO_TEST_CASE( test_default_color_is_black )
	{
		CColorRef color ;
		BOOST_CHECK( color.is_black() ) ;
	}
	// as_wstring
	BOOST_AUTO_TEST_CASE( test_as_wstring_black )
	{
		CColorRef color ;
		string actual = CStringA(color.as_wstring().c_str()) ;
		BOOST_CHECK_EQUAL( actual, "#000000" ) ;
	}
	// byte_2_wstring
	BOOST_AUTO_TEST_CASE( test_byte_2_wstring_0 )
	{
		CColorRef color ;
		string actual = CStringA(color.byte_2_wstring(0).c_str()) ;
		BOOST_CHECK_EQUAL( actual, "00" ) ;
	}
	// ctor
	BOOST_AUTO_TEST_CASE( constructor_bytes_red )
	{
		CColorRef color(255, 0, 0) ;
		string actual = CStringA(color.as_wstring().c_str()) ;
		BOOST_CHECK_EQUAL( "#ff0000", actual ) ;
	}
	BOOST_AUTO_TEST_CASE( constructor_copy_red )
	{
		CColorRef source(255, 0, 0) ;
		CColorRef color(source) ;
		string actual = CStringA(color.as_wstring().c_str()) ;
		BOOST_CHECK_EQUAL( "#ff0000", actual ) ;
	}
	BOOST_AUTO_TEST_CASE( constructor_bytes_gray )
	{
		CColorRef color(0x10, 0x10, 0x10) ;
		string actual = CStringA(color.as_wstring().c_str()) ;
		BOOST_CHECK_EQUAL( "#101010", actual ) ;
	}
	BOOST_AUTO_TEST_CASE( constructor_colorref_turquoise)
	{
		CColorRef color(COLOR_TURQUOISE) ;
		string actual = CStringA(color.as_wstring().c_str()) ;
		BOOST_CHECK_EQUAL( "#008080", actual ) ;
	}
	// from_string
	BOOST_AUTO_TEST_CASE( ctor_string_empty )
	{
		wstring colorstring ;
		CColorRef color(colorstring) ;
		BOOST_CHECK( color.is_black() ) ;
	}

	// is_black
	BOOST_AUTO_TEST_CASE( is_black )
	{
		CColorRef color ;
		BOOST_CHECK( color.is_black() ) ;
	}
	BOOST_AUTO_TEST_CASE( is_black_false_red )
	{
		CColorRef color(0x10, 0, 0) ;
		BOOST_CHECK( ! color.is_black() ) ;
	}
	BOOST_AUTO_TEST_CASE( is_black_false_green )
	{
		CColorRef color(0, 0x10, 0) ;
		BOOST_CHECK( ! color.is_black() ) ;
	}
	BOOST_AUTO_TEST_CASE( is_black_false_blue )
	{
		CColorRef color(0, 0, 0x10) ;
		BOOST_CHECK( ! color.is_black() ) ;
	}
	// is_white
	BOOST_AUTO_TEST_CASE( is_white_true )
	{
		CColorRef color ;
		color.set_red_value(0xff) ;
		color.set_green_value(0xff) ;
		color.set_blue_value(0xff) ;
		BOOST_CHECK( color.is_white() ) ;
	}
	BOOST_AUTO_TEST_CASE( is_white_false_red )
	{
		CColorRef color(0x0, 0xff, 0xff) ;
		BOOST_CHECK( ! color.is_white() ) ;
	}
	BOOST_AUTO_TEST_CASE( is_white_false_green )
	{
		CColorRef color(0xff, 0x0, 0xff) ;
		BOOST_CHECK( ! color.is_white() ) ;
	}
	BOOST_AUTO_TEST_CASE( is_white_false_blue )
	{
		CColorRef color(0xff, 0xff, 0x0) ;
		BOOST_CHECK( ! color.is_white() ) ;
	}
	// as_colorref
	BOOST_AUTO_TEST_CASE( as_colorref )
	{
		CColorRef color ;
		color.set_red_value(0x80) ;
		color.set_green_value(0x40) ;
		color.set_blue_value(0x20) ;
		COLORREF actual = color.as_colorref() ;
		COLORREF expected = RGB(0x80, 0x40, 0x20) ;
		BOOST_CHECK_EQUAL( (int)expected, (int)actual ) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif
