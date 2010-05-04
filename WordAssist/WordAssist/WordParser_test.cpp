#include "stdafx.h"
#include "WordParser.h"
#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestMemoryFunctions )

	BOOST_AUTO_TEST_CASE( is_whitespace_char)
	{
		WordSelection selection ;
		WordParser parser(selection) ;

		BOOST_CHECK( parser.is_whitespace_char(L" ")) ;
		BOOST_CHECK( parser.is_whitespace_char(L"\naha")) ;
		BOOST_CHECK( parser.is_whitespace_char(L"\r")) ;
		BOOST_CHECK( parser.is_whitespace_char(L"\t")) ;

		BOOST_CHECK( ! parser.is_whitespace_char(NULL)) ;
		BOOST_CHECK( ! parser.is_whitespace_char(L"a")) ;
		BOOST_CHECK( ! parser.is_whitespace_char(L"/")) ;
	}
	BOOST_AUTO_TEST_CASE( formatting_enabled )
	{
		WordParser parser ;
		parser.m_parse_font_face = FALSE;
		parser.m_parse_font_color = FALSE ;
		parser.m_parse_font_bold = FALSE ;
		parser.m_parse_font_italic = FALSE ;
		parser.m_parse_font_underline = TRUE ;
		parser.m_parse_font_super_sub = FALSE ;
		BOOST_CHECK( parser.formatting_enabled() ) ;
	}
	// table_selected
	BOOST_AUTO_TEST_CASE( table_selected_false )
	{
		_bstr_t text = L"spam" ;
		WordParser parser ;
		BOOST_CHECK( ! parser.table_selected(text) ) ;
	}
	BOOST_AUTO_TEST_CASE( table_selected_false_emtpy )
	{
		_bstr_t text = L"" ;
		WordParser parser ;
		BOOST_CHECK( ! parser.table_selected(text) ) ;
	}
	BOOST_AUTO_TEST_CASE( table_selected_true )
	{
		_bstr_t text = L"foo\7" ;
		WordParser parser ;
		BOOST_CHECK( parser.table_selected(text) ) ;
	}
	// is_right_quote_selected
	BOOST_AUTO_TEST_CASE( is_right_quote_selected_false )
	{
		_bstr_t text = L"spam" ;
		WordParser parser ;
		BOOST_CHECK( ! parser.is_right_quote_selected(text) ) ;
	}
	BOOST_AUTO_TEST_CASE( is_right_quote_selected_false_emtpy )
	{
		_bstr_t text = L"" ;
		WordParser parser ;
		BOOST_CHECK( ! parser.is_right_quote_selected(text) ) ;
	}
	BOOST_AUTO_TEST_CASE( is_right_quote_selected_true_straight )
	{
		_bstr_t text = L"\"" ;
		WordParser parser ;
		BOOST_CHECK( parser.is_right_quote_selected(text) ) ;
	}
	BOOST_AUTO_TEST_CASE( is_right_quote_selected_true_single )
	{
		_bstr_t text = L"\x2019" ;
		WordParser parser ;
		BOOST_CHECK( parser.is_right_quote_selected(text) ) ;
	}
	BOOST_AUTO_TEST_CASE( is_right_quote_selected_true_double )
	{
		_bstr_t text = L"\x201D" ;
		WordParser parser ;
		BOOST_CHECK( parser.is_right_quote_selected(text) ) ;
	}
	// make_bold_setting
	BOOST_AUTO_TEST_CASE( make_bold_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_bold_established()) ;
		parser.make_bold_setting(MSOffice::msoTrue, props) ;
		BOOST_CHECK(props.get_bold()) ;
		BOOST_CHECK(props.is_bold_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_bold_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_bold_established()) ;
		parser.make_bold_setting(MSOffice::msoFalse, props) ;
		BOOST_CHECK(!props.get_bold()) ;
		BOOST_CHECK(props.is_bold_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_bold_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_bold_established()) ;
		parser.make_bold_setting(MSOffice::msoTriStateMixed, props) ;
		BOOST_CHECK(!props.get_bold()) ;
		BOOST_CHECK(!props.is_bold_established()) ;
	}
	// make_italic_setting
	BOOST_AUTO_TEST_CASE( make_italic_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_italic_established()) ;
		parser.make_italic_setting(MSOffice::msoTrue, props) ;
		BOOST_CHECK(props.get_italic()) ;
		BOOST_CHECK(props.is_italic_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_italic_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_italic_established()) ;
		parser.make_italic_setting(MSOffice::msoFalse, props) ;
		BOOST_CHECK(!props.get_italic()) ;
		BOOST_CHECK(props.is_italic_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_italic_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_italic_established()) ;
		parser.make_italic_setting(MSOffice::msoTriStateMixed, props) ;
		BOOST_CHECK(!props.get_italic()) ;
		BOOST_CHECK(!props.is_italic_established()) ;
	}
	// make_subscript_setting
	BOOST_AUTO_TEST_CASE( make_subscript_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_subscript_established()) ;
		parser.make_subscript_setting(MSOffice::msoTrue, props) ;
		BOOST_CHECK(props.get_subscript()) ;
		BOOST_CHECK(props.is_subscript_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_subscript_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_subscript_established()) ;
		parser.make_subscript_setting(MSOffice::msoFalse, props) ;
		BOOST_CHECK(!props.get_subscript()) ;
		BOOST_CHECK(props.is_subscript_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_subscript_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_subscript_established()) ;
		parser.make_subscript_setting(MSOffice::msoTriStateMixed, props) ;
		BOOST_CHECK(!props.get_subscript()) ;
		BOOST_CHECK(!props.is_subscript_established()) ;
	}
	// make_superscript_setting
	BOOST_AUTO_TEST_CASE( make_superscript_setting_true )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_superscript_established()) ;
		parser.make_superscript_setting(MSOffice::msoTrue, props) ;
		BOOST_CHECK(props.get_superscript()) ;
		BOOST_CHECK(props.is_superscript_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_superscript_setting_false )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_superscript_established()) ;
		parser.make_superscript_setting(MSOffice::msoFalse, props) ;
		BOOST_CHECK(!props.get_superscript()) ;
		BOOST_CHECK(props.is_superscript_established()) ;
	}
	BOOST_AUTO_TEST_CASE( make_superscript_setting_msoTriStateMixed )
	{
		WordParser parser ;
		font_properties props ;
		BOOST_CHECK(!props.is_superscript_established()) ;
		parser.make_superscript_setting(MSOffice::msoTriStateMixed, props) ;
		BOOST_CHECK(!props.get_superscript()) ;
		BOOST_CHECK(!props.is_superscript_established()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif