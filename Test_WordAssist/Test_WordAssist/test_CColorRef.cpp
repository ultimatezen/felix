#include "stdafx.h"
#include "ColorRef.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_CColorRef )


BOOST_AUTO_TEST_CASE( rgb_values )
{
	CColorRef color ;
	wstring color_label( L"#FF0088" ) ;
	color.from_string(color_label) ;

	BOOST_CHECK ( 0xFF == color.get_red_value() ) ; 
	BOOST_CHECK ( 0x00 == color.get_green_value() ) ; 
	BOOST_CHECK ( 0x88 == color.get_blue_value() ) ;

	BOOST_CHECK ( RGB( 0xFF, 0x00, 0x88 ) == color.as_colorref() ) ;
}

BOOST_AUTO_TEST_CASE( as_wstring )
{
	CColorRef color ;
	color = RGB( 0xAA, 0x11, 0xFC ) ;
	BOOST_CHECK ( boost::iequals( L"#AA11FC", color.as_wstring() ) ) ; 
}


BOOST_AUTO_TEST_SUITE_END()
