// font_properties.cpp: implementation of the font_properties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "font_properties.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_font_properties )

BOOST_AUTO_TEST_CASE( Constructor )
{
	font_properties props ;
	BOOST_CHECK( props.get_bold() == false ) ;
	BOOST_CHECK( props.get_font_color() == RGB(0,0,0) ) ;
	BOOST_CHECK( props.get_font_face() == L"" ) ;
	BOOST_CHECK( props.get_font_size() == -1 ) ;
	BOOST_CHECK( props.get_italic() == false ) ;
	BOOST_CHECK( props.get_subscript() == false ) ;
	BOOST_CHECK( props.get_superscript() == false ) ;
	BOOST_CHECK( props.get_underline() == false ) ;

	BOOST_CHECK( props.is_bold_established() == false ) ;
	BOOST_CHECK( props.is_font_color_established() == false ) ;
	BOOST_CHECK( props.is_font_face_established() == false ) ;
	BOOST_CHECK( props.is_font_size_established() == false ) ;
	BOOST_CHECK( props.is_italic_established() == false ) ;
	BOOST_CHECK( props.is_subscript_established() == false ) ;
	BOOST_CHECK( props.is_superscript_established() == false ) ;

}

BOOST_AUTO_TEST_CASE( Bold )
{
	font_properties props ;
	BOOST_CHECK( props.get_bold() == false ) ;

	BOOST_CHECK( props.is_bold_established() == false ) ;

	props.set_bold( true ) ;

	BOOST_CHECK( props.get_bold() == true ) ;
	BOOST_CHECK( props.is_bold_established() == true ) ;

	props.set_bold( false ) ;

	BOOST_CHECK( props.get_bold() == false ) ;
	BOOST_CHECK( props.is_bold_established() == true ) ;

	font_properties props2 ;

	props2.set_bold( false ) ;

	BOOST_CHECK( props2.get_bold() == false ) ;
	BOOST_CHECK( props2.is_bold_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( FontColor )
{
	font_properties props ;
	BOOST_CHECK( props.get_font_color() == RGB(0,0,0) ) ;

	BOOST_CHECK( props.is_font_color_established() == false ) ;

	CColorRef color( RGB( 255, 0, 255 ) ) ;
	props.set_font_color( color ) ;

	BOOST_CHECK( props.get_font_color() == RGB( 255, 0, 255 ) ) ;
	BOOST_CHECK( props.is_font_color_established() == true ) ;

}


BOOST_AUTO_TEST_CASE( FontFace )
{
	font_properties props ;
	BOOST_CHECK( props.get_font_face() == L"" ) ;

	BOOST_CHECK( props.is_font_face_established() == false ) ;

	props.set_font_face( L"Times New Roman" ) ;

	BOOST_CHECK( props.get_font_face() == L"Times New Roman" ) ;
	BOOST_CHECK( props.is_font_face_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( FontSize )
{
	font_properties props ;
	BOOST_CHECK( props.get_font_size() == -1 ) ;

	BOOST_CHECK( props.is_font_size_established() == false ) ;

	props.set_font_size( 12 ) ;

	BOOST_CHECK( props.get_font_size() == 12 ) ;
	BOOST_CHECK( props.is_font_size_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( Italic )
{
	font_properties props ;
	BOOST_CHECK( props.get_italic() == false ) ;

	BOOST_CHECK( props.is_italic_established() == false ) ;

	props.set_italic( true ) ;

	BOOST_CHECK( props.get_italic() == true ) ;
	BOOST_CHECK( props.is_italic_established() == true ) ;

	props.set_italic( false ) ;

	BOOST_CHECK( props.get_italic() == false ) ;
	BOOST_CHECK( props.is_italic_established() == true ) ;

	font_properties props2 ;

	props2.set_italic( false ) ;

	BOOST_CHECK( props2.get_italic() == false ) ;
	BOOST_CHECK( props2.is_italic_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( Subscript )
{
	font_properties props ;
	BOOST_CHECK( props.get_subscript() == false ) ;

	BOOST_CHECK( props.is_subscript_established() == false ) ;

	props.set_subscript( true ) ;

	BOOST_CHECK( props.get_subscript() == true ) ;
	BOOST_CHECK( props.is_subscript_established() == true ) ;

	props.set_subscript( false ) ;

	BOOST_CHECK( props.get_subscript() == false ) ;
	BOOST_CHECK( props.is_subscript_established() == true ) ;

	font_properties props2 ;

	props2.set_subscript( false ) ;

	BOOST_CHECK( props2.get_subscript() == false ) ;
	BOOST_CHECK( props2.is_subscript_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( Superscript )
{
	font_properties props ;
	BOOST_CHECK( props.get_superscript() == false ) ;

	BOOST_CHECK( props.is_superscript_established() == false ) ;

	props.set_superscript( true ) ;

	BOOST_CHECK( props.get_superscript() == true ) ;
	BOOST_CHECK( props.is_superscript_established() == true ) ;

	props.set_superscript( false ) ;

	BOOST_CHECK( props.get_superscript() == false ) ;
	BOOST_CHECK( props.is_superscript_established() == true ) ;

	font_properties props2 ;

	props2.set_superscript( false ) ;

	BOOST_CHECK( props2.get_superscript() == false ) ;
	BOOST_CHECK( props2.is_superscript_established() == true ) ;
}

BOOST_AUTO_TEST_CASE( Underline )
{
	font_properties props ;
	BOOST_CHECK( props.get_underline() == false ) ;

	BOOST_CHECK( props.is_underline_established() == false ) ;

	props.set_underline( true ) ;

	BOOST_CHECK( props.get_underline() == true ) ;
	BOOST_CHECK( props.is_underline_established() == true ) ;

	props.set_underline( false ) ;

	BOOST_CHECK( props.get_underline() == false ) ;
	BOOST_CHECK( props.is_underline_established() == true ) ;

	font_properties props2 ;

	props2.set_underline( false ) ;

	BOOST_CHECK( props2.get_underline() == false ) ;
	BOOST_CHECK( props2.is_underline_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( CopyConstructor )
{
	font_properties props ;
	BOOST_CHECK( props.get_bold() == false ) ;
	BOOST_CHECK( props.get_font_color() == RGB(0,0,0) ) ;
	BOOST_CHECK( props.get_font_face() == L"" ) ;
	BOOST_CHECK( props.get_font_size() == -1 ) ;
	BOOST_CHECK( props.get_italic() == false ) ;
	BOOST_CHECK( props.get_subscript() == false ) ;
	BOOST_CHECK( props.get_superscript() == false ) ;
	BOOST_CHECK( props.get_underline() == false ) ;

	BOOST_CHECK( props.is_bold_established() == false ) ;
	BOOST_CHECK( props.is_font_color_established() == false ) ;
	BOOST_CHECK( props.is_font_face_established() == false ) ;
	BOOST_CHECK( props.is_font_size_established() == false ) ;
	BOOST_CHECK( props.is_italic_established() == false ) ;
	BOOST_CHECK( props.is_subscript_established() == false ) ;
	BOOST_CHECK( props.is_superscript_established() == false ) ;


	props.set_bold( true ) ;

	BOOST_CHECK( props.get_bold() == true ) ;
	BOOST_CHECK( props.is_bold_established() == true ) ;


	CColorRef color( RGB( 255, 0, 255 ) ) ;
	props.set_font_color( color ) ;

	BOOST_CHECK( props.get_font_color() == RGB( 255, 0, 255 ) ) ;
	BOOST_CHECK( props.is_font_color_established() == true ) ;


	props.set_font_face( L"Times New Roman" ) ;

	BOOST_CHECK( props.get_font_face() == L"Times New Roman" ) ;
	BOOST_CHECK( props.is_font_face_established() == true ) ;


	props.set_font_size( 12 ) ;

	BOOST_CHECK( props.get_font_size() == 12 ) ;
	BOOST_CHECK( props.is_font_size_established() == true ) ;


	props.set_italic( true ) ;

	BOOST_CHECK( props.get_italic() == true ) ;
	BOOST_CHECK( props.is_italic_established() == true ) ;


	props.set_subscript( true ) ;

	BOOST_CHECK( props.get_subscript() == true ) ;
	BOOST_CHECK( props.is_subscript_established() == true ) ;


	props.set_superscript( false ) ;

	BOOST_CHECK( props.get_superscript() == false ) ;
	BOOST_CHECK( props.is_superscript_established() == true ) ;


	props.set_underline( true ) ;

	BOOST_CHECK( props.get_underline() == true ) ;
	BOOST_CHECK( props.is_underline_established() == true ) ;

	font_properties props2( props ) ;


	BOOST_CHECK( props2.get_bold() == true ) ;
	BOOST_CHECK( props2.is_bold_established() == true ) ;


	BOOST_CHECK( props2.get_font_color() == RGB( 255, 0, 255 ) ) ;
	BOOST_CHECK( props2.is_font_color_established() == true ) ;


	BOOST_CHECK( props2.get_font_face() == L"Times New Roman" ) ;
	BOOST_CHECK( props2.is_font_face_established() == true ) ;


	BOOST_CHECK( props2.get_font_size() == 12 ) ;
	BOOST_CHECK( props2.is_font_size_established() == true ) ;


	BOOST_CHECK( props2.get_italic() == true ) ;
	BOOST_CHECK( props2.is_italic_established() == true ) ;


	BOOST_CHECK( props2.get_subscript() == true ) ;
	BOOST_CHECK( props2.is_subscript_established() == true ) ;


	BOOST_CHECK( props2.get_superscript() == false ) ;
	BOOST_CHECK( props2.is_superscript_established() == true ) ;


	BOOST_CHECK( props2.get_underline() == true ) ;
	BOOST_CHECK( props2.is_underline_established() == true ) ;

}

BOOST_AUTO_TEST_CASE( AssignmentOperator )
{
	font_properties props ;
	BOOST_CHECK( props.get_bold() == false ) ;
	BOOST_CHECK( props.get_font_color() == RGB(0,0,0) ) ;
	BOOST_CHECK( props.get_font_face() == L"" ) ;
	BOOST_CHECK( props.get_font_size() == -1 ) ;
	BOOST_CHECK( props.get_italic() == false ) ;
	BOOST_CHECK( props.get_subscript() == false ) ;
	BOOST_CHECK( props.get_superscript() == false ) ;
	BOOST_CHECK( props.get_underline() == false ) ;

	BOOST_CHECK( props.is_bold_established() == false ) ;
	BOOST_CHECK( props.is_font_color_established() == false ) ;
	BOOST_CHECK( props.is_font_face_established() == false ) ;
	BOOST_CHECK( props.is_font_size_established() == false ) ;
	BOOST_CHECK( props.is_italic_established() == false ) ;
	BOOST_CHECK( props.is_subscript_established() == false ) ;
	BOOST_CHECK( props.is_superscript_established() == false ) ;


	props.set_bold( true ) ;

	BOOST_CHECK( props.get_bold() == true ) ;
	BOOST_CHECK( props.is_bold_established() == true ) ;


	CColorRef color( RGB( 255, 0, 255 ) ) ;
	props.set_font_color( color ) ;

	BOOST_CHECK( props.get_font_color() == RGB( 255, 0, 255 ) ) ;
	BOOST_CHECK( props.is_font_color_established() == true ) ;


	props.set_font_face( L"Times New Roman" ) ;

	BOOST_CHECK( props.get_font_face() == L"Times New Roman" ) ;
	BOOST_CHECK( props.is_font_face_established() == true ) ;


	props.set_font_size( 12 ) ;

	BOOST_CHECK( props.get_font_size() == 12 ) ;
	BOOST_CHECK( props.is_font_size_established() == true ) ;


	props.set_italic( true ) ;

	BOOST_CHECK( props.get_italic() == true ) ;
	BOOST_CHECK( props.is_italic_established() == true ) ;


	props.set_subscript( true ) ;

	BOOST_CHECK( props.get_subscript() == true ) ;
	BOOST_CHECK( props.is_subscript_established() == true ) ;


	props.set_superscript( false ) ;

	BOOST_CHECK( props.get_superscript() == false ) ;
	BOOST_CHECK( props.is_superscript_established() == true ) ;


	props.set_underline( true ) ;

	BOOST_CHECK( props.get_underline() == true ) ;
	BOOST_CHECK( props.is_underline_established() == true ) ;

	font_properties props2 ;
	props2 = props ;


	BOOST_CHECK( props2.get_bold() == true ) ;
	BOOST_CHECK( props2.is_bold_established() == true ) ;


	BOOST_CHECK( props2.get_font_color() == RGB( 255, 0, 255 ) ) ;
	BOOST_CHECK( props2.is_font_color_established() == true ) ;


	BOOST_CHECK( props2.get_font_face() == L"Times New Roman" ) ;
	BOOST_CHECK( props2.is_font_face_established() == true ) ;


	BOOST_CHECK( props2.get_font_size() == 12 ) ;
	BOOST_CHECK( props2.is_font_size_established() == true ) ;


	BOOST_CHECK( props2.get_italic() == true ) ;
	BOOST_CHECK( props2.is_italic_established() == true ) ;


	BOOST_CHECK( props2.get_subscript() == true ) ;
	BOOST_CHECK( props2.is_subscript_established() == true ) ;


	BOOST_CHECK( props2.get_superscript() == false ) ;
	BOOST_CHECK( props2.is_superscript_established() == true ) ;


	BOOST_CHECK( props2.get_underline() == true ) ;
	BOOST_CHECK( props2.is_underline_established() == true ) ;

}

BOOST_AUTO_TEST_SUITE_END()

