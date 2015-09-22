#include "stdafx.h"
#include "ColorRef.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	TEST( CColorRefTestCase, StringConversions )
	{
		CColorRef color ;
		wstring color_label( L"#FF0088" ) ;
		color.from_string(color_label) ;

		ASSERT_EQUALS ( 0xFF, color.get_red_value() ) ; 
		ASSERT_EQUALS ( 0x00, color.get_green_value() ) ; 
		ASSERT_EQUALS ( 0x88, color.get_blue_value() ) ;

		ASSERT_EQUALS ( RGB( 0xFF, 0x00, 0x88 ), color.as_colorref() ) ;

		color = RGB( 0xAA, 0x11, 0xFC ) ;

		ASSERT_EQUALS ( true, str::equal_nocase( L"#AA11FC", color.as_wstring() ) ) ; 
	}

	TEST( CColorRefTestCase, Invert )
	{
		CColorRef color ;
		ASSERT_EQUALS ( RGB( 0, 0, 0), color.as_colorref() ) ;
		color.invert() ;
		ASSERT_EQUALS ( RGB( 255, 255, 255), color.as_colorref() ) ;

		color = RGB( 255, 0, 0 ) ;
		ASSERT_EQUALS ( RGB( 255, 0, 0), color.as_colorref() ) ;
		color.invert() ;
		ASSERT_EQUALS ( RGB( 0, 255, 255), color.as_colorref() ) ;
		color.invert() ;
		ASSERT_EQUALS ( RGB( 255, 0, 0), color.as_colorref() ) ;

		color = RGB( 100, 0, 100 ) ;
		ASSERT_EQUALS ( RGB( 100, 0, 100), color.as_colorref() ) ;
		color.invert() ;
		ASSERT_EQUALS ( RGB( 155, 255, 155), color.as_colorref() ) ;
		color.invert() ;
		ASSERT_EQUALS ( RGB( 100, 0, 100), color.as_colorref() ) ;

	}
}
#endif
