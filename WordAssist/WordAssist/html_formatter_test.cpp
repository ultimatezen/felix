#include "stdafx.h"
#include "html_formatter.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	TEST( html_formatter, Bold )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_bold() == false ) ;
		ASSERT_TRUE ( formatter.is_bold_specified() == false ) ; 

		formatter.set_bold( true ) ;

		ASSERT_TRUE( formatter.get_bold() == true ) ;
		ASSERT_TRUE ( formatter.is_bold_specified() == true ) ; 

		html_formatter formatter2 ;

		formatter2.set_bold( false ) ;

		ASSERT_TRUE( formatter2.get_bold() == false ) ;
		ASSERT_TRUE ( formatter2.is_bold_specified() == true ) ; 
	}

	TEST( html_formatter, ForeColor )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_fore_color() == RGB(0,0,0) ) ;
		ASSERT_TRUE ( formatter.is_fore_color_specified() == false ) ; 

		CColorRef color( 255, 100, 255 ) ;
		formatter.set_fore_color( color ) ;

		ASSERT_TRUE( formatter.get_fore_color() == RGB( 255, 100, 255 ) ) ;
		ASSERT_TRUE ( formatter.is_fore_color_specified() == true ) ; 
	}

	/** implementation of black-box test for set_heading_level. */

	TEST( html_formatter, HeadingLevel )
	{
		wstring html_text( L"Some text" ) ;

		html_formatter formatter ;

		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_text() == L"Some text" ) ; 

		formatter.init() ;
		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_paragraph_text() == L"<P>Some text</P>" ) ; 

		formatter.init() ;
		formatter.set_heading_level( 1u ) ;
		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_text() == L"Some text" ) ; 

		formatter.init() ;
		formatter.set_heading_level( 1u ) ;
		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_paragraph_text() == L"<H1>Some text</H1>" ) ; 

		formatter.init() ;
		formatter.set_heading_level( 3u ) ;
		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_text() == L"Some text" ) ; 

		formatter.init() ;
		formatter.set_heading_level( 3u ) ;
		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_paragraph_text() == L"<H3>Some text</H3>" ) ; 

		formatter.init() ;
		formatter.set_heading_level( 0u ) ;
		formatter.add_text( html_text ) ;
		ASSERT_TRUE ( formatter.get_paragraph_text() == L"<P>Some text</P>" ) ; 

	}
	TEST( html_formatter, Italic )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_italic() == false ) ;
		ASSERT_TRUE ( formatter.is_italic_specified() == false ) ; 

		formatter.set_italic( true ) ;

		ASSERT_TRUE( formatter.get_italic() == true ) ;
		ASSERT_TRUE ( formatter.is_italic_specified() == true ) ; 

		html_formatter formatter2 ;

		formatter2.set_italic( false ) ;

		ASSERT_TRUE( formatter2.get_italic() == false ) ;
		ASSERT_TRUE ( formatter2.is_italic_specified() == true ) ; 
	}

	TEST( html_formatter, FontSize )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_font_size() == 4 ) ;
		ASSERT_TRUE ( formatter.is_font_size_specified() == false ) ; 

		formatter.set_size( 12 ) ;

		ASSERT_TRUE( formatter.get_font_size() == formatter.point_size_to_html_size( 12 ) ) ;
		ASSERT_TRUE ( formatter.is_font_size_specified() == true ) ; 
	}


	TEST( html_formatter, Superscript )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_superscript() == false ) ;

		formatter.set_superscript( true ) ;

		ASSERT_TRUE ( formatter.get_superscript() == true ) ; 
		ASSERT_TRUE ( formatter.get_subscript() == false ) ;

		formatter.set_subscript( true ) ;

		ASSERT_TRUE ( formatter.get_superscript() == false ) ; 
		ASSERT_TRUE ( formatter.get_subscript() == true ) ;

	}

	TEST( html_formatter, Subscript )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_subscript() == false ) ;

		formatter.set_subscript( true ) ;

		ASSERT_TRUE ( formatter.get_superscript() == false ) ; 
		ASSERT_TRUE ( formatter.get_subscript() == true ) ;

		formatter.set_superscript( true ) ;

		ASSERT_TRUE ( formatter.get_subscript() == false ) ;
		ASSERT_TRUE ( formatter.get_superscript() == true ) ; 

	}

	TEST( html_formatter, Underline )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_underline() == false ) ;
		ASSERT_TRUE ( formatter.is_underline_specified() == false ) ; 

		formatter.set_underline( true ) ;

		ASSERT_TRUE( formatter.get_underline() == true ) ;
		ASSERT_TRUE ( formatter.is_underline_specified() == true ) ; 

		html_formatter formatter2 ;

		formatter2.set_underline( false ) ;

		ASSERT_TRUE( formatter2.get_underline() == false ) ;
		ASSERT_TRUE ( formatter2.is_underline_specified() == true ) ; 
	}
	TEST( html_formatter, VertAlign )
	{
		html_formatter formatter ;

		ASSERT_TRUE( formatter.get_v_align() == html_processing::VA_BASELINE ) ;

		formatter.set_v_alignment( html_processing::VA_SUBSCRIPT ) ;

		ASSERT_TRUE( formatter.get_v_align() == html_processing::VA_SUBSCRIPT ) ;

		formatter.set_v_alignment( html_processing::VA_SUPERSCRIPT ) ;

		ASSERT_TRUE( formatter.get_v_align() == html_processing::VA_SUPERSCRIPT ) ;

		formatter.set_v_alignment( html_processing::VA_BASELINE ) ;

		ASSERT_TRUE( formatter.get_v_align() == html_processing::VA_BASELINE ) ;
	}


	/** implementation of black-box test for clear_fore_color. */
	TEST( html_formatter, ClearForeColor )
	{
		html_formatter formatter ;

		ASSERT_TRUE ( formatter.get_fore_color() == RGB( 0, 0, 0 ) ) ; 

		CColorRef color( RGB( 255, 10, 155 ) ) ;
		formatter.set_fore_color( color ) ;

		ASSERT_TRUE ( formatter.get_fore_color() == RGB( 255, 10, 155 ) ) ; 

		color = RGB( 100, 55, 100 ) ;
		formatter.set_fore_color( color ) ;

		ASSERT_TRUE ( formatter.get_fore_color() == RGB( 100, 55, 100 ) ) ; 

		formatter.clear_fore_color() ;

		ASSERT_TRUE ( formatter.get_fore_color() == RGB( 0, 0, 0 ) ) ; 
	}
}
#endif
