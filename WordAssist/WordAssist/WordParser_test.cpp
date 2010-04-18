#include "stdafx.h"
#include "WordParser.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestMemoryFunctions )

TEST( WordParserTest, is_whitespace_char)
	{
		WordSelection selection ;
		WordParser parser(selection) ;

		ASSERT_TRUE( parser.is_whitespace_char(L" ")) ;
		ASSERT_TRUE( parser.is_whitespace_char(L"\naha")) ;
		ASSERT_TRUE( parser.is_whitespace_char(L"\r")) ;
		ASSERT_TRUE( parser.is_whitespace_char(L"\t")) ;

		ASSERT_TRUE( ! parser.is_whitespace_char(NULL)) ;
		ASSERT_TRUE( ! parser.is_whitespace_char(L"a")) ;
		ASSERT_TRUE( ! parser.is_whitespace_char(L"/")) ;
	}
	TEST( WordParserTest, formatting_enabled )
	{
		WordParser parser ;
		parser.m_parse_font_face = FALSE;
		parser.m_parse_font_color = FALSE ;
		parser.m_parse_font_bold = FALSE ;
		parser.m_parse_font_italic = FALSE ;
		parser.m_parse_font_underline = TRUE ;
		parser.m_parse_font_super_sub = FALSE ;
		ASSERT_TRUE( parser.formatting_enabled() ) ;
	}
	// table_selected
	TEST( WordParserTest, table_selected_false )
	{
		_bstr_t text = L"spam" ;
		WordParser parser ;
		ASSERT_TRUE( ! parser.table_selected(text) ) ;
	}
	TEST( WordParserTest, table_selected_false_emtpy )
	{
		_bstr_t text = L"" ;
		WordParser parser ;
		ASSERT_TRUE( ! parser.table_selected(text) ) ;
	}
	TEST( WordParserTest, table_selected_true )
	{
		_bstr_t text = L"foo\7" ;
		WordParser parser ;
		ASSERT_TRUE( parser.table_selected(text) ) ;
	}
	// is_right_quote_selected
	TEST( WordParserTest, is_right_quote_selected_false )
	{
		_bstr_t text = L"spam" ;
		WordParser parser ;
		ASSERT_TRUE( ! parser.is_right_quote_selected(text) ) ;
	}
	TEST( WordParserTest, is_right_quote_selected_false_emtpy )
	{
		_bstr_t text = L"" ;
		WordParser parser ;
		ASSERT_TRUE( ! parser.is_right_quote_selected(text) ) ;
	}
	TEST( WordParserTest, is_right_quote_selected_true_straight )
	{
		_bstr_t text = L"\"" ;
		WordParser parser ;
		ASSERT_TRUE( parser.is_right_quote_selected(text) ) ;
	}
	TEST( WordParserTest, is_right_quote_selected_true_single )
	{
		_bstr_t text = L"\x2019" ;
		WordParser parser ;
		ASSERT_TRUE( parser.is_right_quote_selected(text) ) ;
	}
	TEST( WordParserTest, is_right_quote_selected_true_double )
	{
		_bstr_t text = L"\x201D" ;
		WordParser parser ;
		ASSERT_TRUE( parser.is_right_quote_selected(text) ) ;
	}
	// make_bold_setting
	TEST( WordParserTest, make_bold_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_bold_established()) ;
		parser.make_bold_setting(MSOffice::msoTrue, props) ;
		ASSERT_TRUE(props.get_bold()) ;
		ASSERT_TRUE(props.is_bold_established()) ;
	}
	TEST( WordParserTest, make_bold_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_bold_established()) ;
		parser.make_bold_setting(MSOffice::msoFalse, props) ;
		ASSERT_TRUE(!props.get_bold()) ;
		ASSERT_TRUE(props.is_bold_established()) ;
	}
	TEST( WordParserTest, make_bold_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_bold_established()) ;
		parser.make_bold_setting(MSOffice::msoTriStateMixed, props) ;
		ASSERT_TRUE(!props.get_bold()) ;
		ASSERT_TRUE(!props.is_bold_established()) ;
	}
	// make_italic_setting
	TEST( WordParserTest, make_italic_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_italic_established()) ;
		parser.make_italic_setting(MSOffice::msoTrue, props) ;
		ASSERT_TRUE(props.get_italic()) ;
		ASSERT_TRUE(props.is_italic_established()) ;
	}
	TEST( WordParserTest, make_italic_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_italic_established()) ;
		parser.make_italic_setting(MSOffice::msoFalse, props) ;
		ASSERT_TRUE(!props.get_italic()) ;
		ASSERT_TRUE(props.is_italic_established()) ;
	}
	TEST( WordParserTest, make_italic_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_italic_established()) ;
		parser.make_italic_setting(MSOffice::msoTriStateMixed, props) ;
		ASSERT_TRUE(!props.get_italic()) ;
		ASSERT_TRUE(!props.is_italic_established()) ;
	}
	// make_subscript_setting
	TEST( WordParserTest, make_subscript_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_subscript_established()) ;
		parser.make_subscript_setting(MSOffice::msoTrue, props) ;
		ASSERT_TRUE(props.get_subscript()) ;
		ASSERT_TRUE(props.is_subscript_established()) ;
	}
	TEST( WordParserTest, make_subscript_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_subscript_established()) ;
		parser.make_subscript_setting(MSOffice::msoFalse, props) ;
		ASSERT_TRUE(!props.get_subscript()) ;
		ASSERT_TRUE(props.is_subscript_established()) ;
	}
	TEST( WordParserTest, make_subscript_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_subscript_established()) ;
		parser.make_subscript_setting(MSOffice::msoTriStateMixed, props) ;
		ASSERT_TRUE(!props.get_subscript()) ;
		ASSERT_TRUE(!props.is_subscript_established()) ;
	}
	// make_superscript_setting
	TEST( WordParserTest, make_superscript_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_superscript_established()) ;
		parser.make_superscript_setting(MSOffice::msoTrue, props) ;
		ASSERT_TRUE(props.get_superscript()) ;
		ASSERT_TRUE(props.is_superscript_established()) ;
	}
	TEST( WordParserTest, make_superscript_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_superscript_established()) ;
		parser.make_superscript_setting(MSOffice::msoFalse, props) ;
		ASSERT_TRUE(!props.get_superscript()) ;
		ASSERT_TRUE(props.is_superscript_established()) ;
	}
	TEST( WordParserTest, make_superscript_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		ASSERT_TRUE(!props.is_superscript_established()) ;
		parser.make_superscript_setting(MSOffice::msoTriStateMixed, props) ;
		ASSERT_TRUE(!props.get_superscript()) ;
		ASSERT_TRUE(!props.is_superscript_established()) ;
	}

BOOST_AUTO_TEST_SUITE_END()
