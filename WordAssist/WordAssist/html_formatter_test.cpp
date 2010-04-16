#include "stdafx.h"
#include "html_formatter.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_html_formatter )

BOOST_AUTO_TEST_CASE( Bold_init )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_bold() == false ) ;
	BOOST_CHECK ( formatter.is_bold_specified() == false ) ; 
}
BOOST_AUTO_TEST_CASE( Bold_true )
{
	html_formatter formatter ;
	formatter.set_bold( true ) ;

	BOOST_CHECK( formatter.get_bold() == true ) ;
	BOOST_CHECK ( formatter.is_bold_specified() == true ) ; 

}
BOOST_AUTO_TEST_CASE( Bold_false )
{
	html_formatter formatter ;
	formatter.set_bold( true ) ;
	formatter.set_bold( false ) ;

	BOOST_CHECK( formatter.get_bold() == false ) ;
	BOOST_CHECK ( formatter.is_bold_specified() == true ) ; 
}

BOOST_AUTO_TEST_CASE( ForeColor )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_fore_color() == RGB(0,0,0) ) ;
	BOOST_CHECK ( formatter.is_fore_color_specified() == false ) ; 

	CColorRef color( 255, 100, 255 ) ;
	formatter.set_fore_color( color ) ;

	BOOST_CHECK( formatter.get_fore_color() == RGB( 255, 100, 255 ) ) ;
	BOOST_CHECK ( formatter.is_fore_color_specified() == true ) ; 
}

BOOST_AUTO_TEST_CASE( HeadingLevel_none )
{
	wstring html_text( L"Some text" ) ;

	html_formatter formatter ;

	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_text() == L"Some text" ) ; 

}
BOOST_AUTO_TEST_CASE( HeadingLevel_init )
{
	wstring html_text( L"Some text" ) ;

	html_formatter formatter ;

	formatter.add_text( html_text ) ;
	formatter.init() ;
	BOOST_CHECK ( formatter.get_text() == L"" ) ; 
}

BOOST_AUTO_TEST_CASE( HeadingLevel_p )
{
	wstring html_text( L"Some text" ) ;
	html_formatter formatter ;

	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_paragraph_text() == L"<P>Some text</P>" ) ; 
}

BOOST_AUTO_TEST_CASE( HeadingLevel_1 )
{
	wstring html_text( L"Some text" ) ;
	html_formatter formatter ;

	formatter.set_heading_level( 1u ) ;
	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_text() == L"Some text" ) ; 

	formatter.init() ;
	formatter.set_heading_level( 1u ) ;
	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_paragraph_text() == L"<H1>Some text</H1>" ) ; 
}

BOOST_AUTO_TEST_CASE( HeadingLevel_3 )
{
	wstring html_text( L"Some text" ) ;
	html_formatter formatter ;

	formatter.set_heading_level( 3u ) ;
	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_text() == L"Some text" ) ; 

	formatter.init() ;
	formatter.set_heading_level( 3u ) ;
	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_paragraph_text() == L"<H3>Some text</H3>" ) ; 
}

BOOST_AUTO_TEST_CASE( HeadingLevel_0 )
{
	wstring html_text( L"Some text" ) ;
	html_formatter formatter ;

	formatter.set_heading_level( 0u ) ;
	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_text() == L"Some text" ) ; 

	formatter.init() ;
	formatter.set_heading_level( 0u ) ;
	formatter.add_text( html_text ) ;
	BOOST_CHECK ( formatter.get_paragraph_text() == L"<P>Some text</P>" ) ; 
}

BOOST_AUTO_TEST_CASE( Italic_init )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_italic() == false ) ;
	BOOST_CHECK ( formatter.is_italic_specified() == false ) ; 
}
BOOST_AUTO_TEST_CASE( Italic_true )
{
	html_formatter formatter ;
	formatter.set_italic( true ) ;

	BOOST_CHECK( formatter.get_italic() == true ) ;
	BOOST_CHECK ( formatter.is_italic_specified() == true ) ; 

}
BOOST_AUTO_TEST_CASE( Italic_false )
{
	html_formatter formatter ;
	formatter.set_italic( true ) ;
	formatter.set_italic( false ) ;

	BOOST_CHECK( formatter.get_italic() == false ) ;
	BOOST_CHECK ( formatter.is_italic_specified() == true ) ; 
}

BOOST_AUTO_TEST_CASE( FontSize )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_font_size() == 4 ) ;
	BOOST_CHECK ( formatter.is_font_size_specified() == false ) ; 

	formatter.set_size( 12 ) ;

	BOOST_CHECK( formatter.get_font_size() == formatter.point_size_to_html_size( 12 ) ) ;
	BOOST_CHECK ( formatter.is_font_size_specified() == true ) ; 
}

BOOST_AUTO_TEST_CASE( Superscript )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_superscript() == false ) ;

	formatter.set_superscript( true ) ;

	BOOST_CHECK ( formatter.get_superscript() == true ) ; 
	BOOST_CHECK ( formatter.get_subscript() == false ) ;

	formatter.set_subscript( true ) ;

	BOOST_CHECK ( formatter.get_superscript() == false ) ; 
	BOOST_CHECK ( formatter.get_subscript() == true ) ;
}


BOOST_AUTO_TEST_CASE( Subscript )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_subscript() == false ) ;

	formatter.set_subscript( true ) ;

	BOOST_CHECK ( formatter.get_superscript() == false ) ; 
	BOOST_CHECK ( formatter.get_subscript() == true ) ;

	formatter.set_superscript( true ) ;

	BOOST_CHECK ( formatter.get_subscript() == false ) ;
	BOOST_CHECK ( formatter.get_superscript() == true ) ; 

}
BOOST_AUTO_TEST_CASE( Underline )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_underline() == false ) ;
	BOOST_CHECK ( formatter.is_underline_specified() == false ) ; 

	formatter.set_underline( true ) ;

	BOOST_CHECK( formatter.get_underline() == true ) ;
	BOOST_CHECK ( formatter.is_underline_specified() == true ) ; 

	html_formatter formatter2 ;

	formatter2.set_underline( false ) ;

	BOOST_CHECK( formatter2.get_underline() == false ) ;
	BOOST_CHECK ( formatter2.is_underline_specified() == true ) ; 
}
BOOST_AUTO_TEST_CASE( VertAlign )
{
	html_formatter formatter ;

	BOOST_CHECK( formatter.get_v_align() == html_processing::VA_BASELINE ) ;

	formatter.set_v_alignment( html_processing::VA_SUBSCRIPT ) ;

	BOOST_CHECK( formatter.get_v_align() == html_processing::VA_SUBSCRIPT ) ;

	formatter.set_v_alignment( html_processing::VA_SUPERSCRIPT ) ;

	BOOST_CHECK( formatter.get_v_align() == html_processing::VA_SUPERSCRIPT ) ;

	formatter.set_v_alignment( html_processing::VA_BASELINE ) ;

	BOOST_CHECK( formatter.get_v_align() == html_processing::VA_BASELINE ) ;
}


/** implementation of black-box test for clear_fore_color. */
BOOST_AUTO_TEST_CASE( ClearForeColor )
{
	html_formatter formatter ;

	BOOST_CHECK ( formatter.get_fore_color() == RGB( 0, 0, 0 ) ) ; 

	CColorRef color( RGB( 255, 10, 155 ) ) ;
	formatter.set_fore_color( color ) ;

	BOOST_CHECK ( formatter.get_fore_color() == RGB( 255, 10, 155 ) ) ; 

	color = RGB( 100, 55, 100 ) ;
	formatter.set_fore_color( color ) ;

	BOOST_CHECK ( formatter.get_fore_color() == RGB( 100, 55, 100 ) ) ; 

	formatter.clear_fore_color() ;

	BOOST_CHECK ( formatter.get_fore_color() == RGB( 0, 0, 0 ) ) ; 
}

BOOST_AUTO_TEST_SUITE_END()
