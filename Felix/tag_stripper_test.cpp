#include "StdAfx.h"
#include "tag_stripper.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_symbol_map )

	// symbol_map
	BOOST_AUTO_TEST_CASE( erase_lt )
	{
		symbol_map symbols ;
		BOOST_CHECK(symbols.exists(L"lt")) ;
		symbols.erase(L"lt") ;
		BOOST_CHECK(!symbols.exists(L"lt")) ;
	}
	BOOST_AUTO_TEST_CASE( erase_non_existent )
	{
		symbol_map symbols ;
		BOOST_CHECK(!symbols.erase(L"side of bacon")) ;
	}
	BOOST_AUTO_TEST_CASE( get_val_gt )
	{
		symbol_map symbols ;
		BOOST_CHECK_EQUAL((int)L'>', (int)symbols.get_val(L"gt")) ;
	}
	BOOST_AUTO_TEST_CASE( get_val_existent )
	{
		symbol_map symbols ;
		BOOST_CHECK_EQUAL(0u, symbols.get_val(L"side of bacon")) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_convert_num_entity )
	// strip_tags
	BOOST_AUTO_TEST_CASE( test_x41 )
	{
		wstring chunk = L"#x41" ;
		textstream_reader<wchar_t> reader ;
		BOOST_CHECK_EQUAL( convert_num_entity(reader, chunk), L'A' ) ;
	}
	BOOST_AUTO_TEST_CASE( test_97 )
	{
		wstring chunk = L"#97" ;
		textstream_reader<wchar_t> reader ;
		BOOST_CHECK_EQUAL( convert_num_entity(reader, chunk), L'a' ) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_tag_stripper )
	BOOST_AUTO_TEST_CASE( test_handle_ampersand_hash )
	{
		symbol_map symbols ;
		wstring text = L"&#65;" ;
		wc_reader reader ;
		reader.set_buffer(text.c_str()) ;
		wstring chunk ;
		wstring stripped_text = handle_ampersand(reader, symbols) ;
		BOOST_CHECK_EQUAL( stripped_text, L"A" ) ;
		BOOST_CHECK(reader.empty()) ;
	}
	BOOST_AUTO_TEST_CASE( test_handle_ampersand_amp )
	{
		symbol_map symbols ;
		wstring text = L"&amp;" ;
		wc_reader reader ;
		reader.set_buffer(text.c_str()) ;
		wstring chunk ;
		wstring stripped_text = handle_ampersand(reader, symbols) ;
		BOOST_CHECK_EQUAL( stripped_text, L"&" ) ;
		BOOST_CHECK(reader.empty()) ;
	}
	// strip_tags
	BOOST_AUTO_TEST_CASE( test_foobar )
	{
		wstring rich = L"<b>foo</b><font size=3><i>bar</i></font> " ;
		wstring stripped = strip_tags( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"foobar " ) ;
	}
	BOOST_AUTO_TEST_CASE( test_br_tag )
	{
		wstring rich = L"foo<br />bar" ;
		wstring stripped = strip_tags( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"foo\nbar" ) ;
	}
	BOOST_AUTO_TEST_CASE( test_hex_numerical_entity )
	{
		wstring rich = L"&#x20;" ;
		wstring stripped = strip_tags( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L" " ) ;
	}
	BOOST_AUTO_TEST_CASE( test_bare_amersand )
	{
		wstring rich = L" & " ;
		wstring stripped = strip_tags( rich ) ;
		BOOST_CHECK_EQUAL( stripped, rich ) ;
	}
	BOOST_AUTO_TEST_CASE( test_nonexistent_symbol )
	{
		wstring rich = L"&blarg;" ;
		wstring stripped = strip_tags( rich ) ;
		BOOST_CHECK_EQUAL( stripped, rich ) ;
	}
	BOOST_AUTO_TEST_CASE( test_complex)
	{
		wstring rich = L"<TD>Here <FONT face=\"Monotype Corsiva\"><EM>is <U>some </U>test</EM> text "
			L"with</FONT> <U><FONT color=#ff0000>lots of "
			L"<STRONG>formatting</STRONG></FONT></U>!</TD>" ;
		wstring stripped = strip_tags(rich) ;
		BOOST_CHECK_EQUAL( stripped, wstring(L"Here is some test text with lots of formatting!") ) ; 
	}
	BOOST_AUTO_TEST_CASE( test_entity)
	{
		wstring rich = L"<STRONG>&amp;</STRONG>" ;
		wstring stripped = strip_tags(rich) ;
		BOOST_CHECK_EQUAL( stripped, wstring(L"&") ) ; 
	}

	BOOST_AUTO_TEST_CASE( test_variations)
	{
		wstring rich = L"<LI><STRONG><U><EM><FONT color=#0080ff>Variations on the theme...</FONT></EM></U></STRONG>" ;
		wstring stripped = strip_tags(rich) ;
		BOOST_CHECK_EQUAL( stripped, wstring(L"Variations on the theme...") ) ; 
	}
	BOOST_AUTO_TEST_CASE( vertical_tab )
	{
		wstring rich = L"<b>Here</b> is a &#11; (vertical tab)" ;
		wstring stripped = strip_tags( rich ) ;
		BOOST_CHECK_EQUAL( stripped, wstring(L"Here is a ") + wchar_t(11) + wstring(L" (vertical tab)") ) ;
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( test_strip_tags_only )
	// strip_tags
	BOOST_AUTO_TEST_CASE( test_foobar )
	{
		wstring rich = L"<b>foo</b><font size=3><i>bar</i></font> " ;
		wstring stripped = strip_tags_only( rich ) ;
		BOOST_CHECK_EQUAL( stripped, L"foobar " ) ;
	}
	BOOST_AUTO_TEST_CASE( test_complex)
	{
		wstring rich = L"<TD>Here <FONT face=\"Monotype Corsiva\"><EM>is <U>some </U>test</EM> text "
			L"with</FONT> <U><FONT color=#ff0000>lots of "
			L"<STRONG>formatting</STRONG></FONT></U>!</TD>" ;
		wstring stripped = strip_tags_only(rich) ;
		BOOST_CHECK_EQUAL( stripped, wstring(L"Here is some test text with lots of formatting!") ) ; 
	}
	BOOST_AUTO_TEST_CASE( test_entity)
	{
		wstring rich = L"<STRONG>&amp;</STRONG>" ;
		wstring stripped = strip_tags_only(rich) ;
		BOOST_CHECK_EQUAL( stripped, wstring(L"&amp;") ) ; 
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
