#include "StdAfx.h"
#include ".\ColorRef.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( CColorRefTestCases, TestModule )
	{
		CColorRef color ;
		ASSERT_TRUE( color.is_black() ) ;
	}
	TEST( CColorRefTestCases, as_wstring_black )
	{
		CColorRef color ;
		SimpleString actual = CStringA(color.as_wstring().c_str()) ;
		ASSERT_EQUALS_V( actual, "#000000" ) ;
	}
	TEST( CColorRefTestCases, byte_2_wstring_0 )
	{
		CColorRef color ;
		SimpleString actual = CStringA(color.byte_2_wstring(0).c_str()) ;
		ASSERT_EQUALS_V( actual, "00" ) ;
	}
	TEST( CColorRefTestCases, constructor_bytes_red )
	{
		CColorRef color(255, 0, 0) ;
		SimpleString actual = CStringA(color.as_wstring().c_str()) ;
		ASSERT_EQUALS_V( "#ff0000", actual ) ;
	}
	TEST( CColorRefTestCases, constructor_copy_red )
	{
		CColorRef source(255, 0, 0) ;
		CColorRef color(source) ;
		SimpleString actual = CStringA(color.as_wstring().c_str()) ;
		ASSERT_EQUALS_V( "#ff0000", actual ) ;
	}
	TEST( CColorRefTestCases, constructor_bytes_gray )
	{
		CColorRef color(0x10, 0x10, 0x10) ;
		SimpleString actual = CStringA(color.as_wstring().c_str()) ;
		ASSERT_EQUALS_V( "#101010", actual ) ;
	}
	TEST( CColorRefTestCases, is_black )
	{
		CColorRef color ;
		ASSERT_TRUE( color.is_black() ) ;
	}
	TEST( CColorRefTestCases, is_white_true )
	{
		CColorRef color ;
		color.set_red_value(0xff) ;
		color.set_green_value(0xff) ;
		color.set_blue_value(0xff) ;
		ASSERT_TRUE( color.is_white() ) ;
	}
	TEST( CColorRefTestCases, is_white_false )
	{
		CColorRef color ;
		color.set_red_value(0x00) ;
		color.set_green_value(0xff) ;
		color.set_blue_value(0xff) ;
		ASSERT_TRUE( ! color.is_white() ) ;
	}
	TEST( CColorRefTestCases, as_colorref )
	{
		CColorRef color ;
		color.set_red_value(0x80) ;
		color.set_green_value(0x40) ;
		color.set_blue_value(0x20) ;
		COLORREF actual = color.as_colorref() ;
		COLORREF expected = RGB(0x80, 0x40, 0x20) ;
		ASSERT_EQUALS_V( (int)expected, (int)actual ) ;
	}
}


#endif // #ifdef _DEBUG
