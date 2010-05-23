#include "StdAfx.h"
#include "AppStringTextNodeParser.h"


#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( CAppStringTextNodeParserTest )

	BOOST_AUTO_TEST_CASE(test_EmptyNode )
	{
		wstring empty_file_text ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		BOOST_CHECK_EQUAL( L"", parser.get_node_string(L"text") ) ;
		BOOST_CHECK_EQUAL( L"", parser.get_node_string(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"", parser.get_node_string(L"id") ) ;
		BOOST_CHECK_EQUAL( L"", parser.get_node_string(L"type") ) ;
	}
	BOOST_AUTO_TEST_CASE(test_SimpleNodes )
	{
		wstring empty_file_text(L"<type>menu</type><id>file</id><text>File</text><tooltip>tooltip</tooltip>" ) ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		BOOST_CHECK_EQUAL( L"File", parser.get_node_string(L"text") ) ;
		BOOST_CHECK_EQUAL( L"tooltip", parser.get_node_string(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"file", parser.get_node_string(L"id") ) ;
		BOOST_CHECK_EQUAL( L"menu", parser.get_node_string(L"type") ) ;
	}
	BOOST_AUTO_TEST_CASE(test_ComplexNodes )
	{
		wstring empty_file_text(L"<type>menu</type><id>file</id><text>&lt;&amp;File&gt;\\tShortcut</text><tooltip>tooltip\\nPlus</tooltip>" ) ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		BOOST_CHECK_EQUAL( L"<&File>\tShortcut", parser.get_node_string(L"text") ) ;
		BOOST_CHECK_EQUAL( L"tooltip\nPlus", parser.get_node_string(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"file", parser.get_node_string(L"id") ) ;
		BOOST_CHECK_EQUAL( L"menu", parser.get_node_string(L"type") ) ;
	}
	BOOST_AUTO_TEST_CASE(test_MissingNodes )
	{
		wstring empty_file_text(L"<id>file</id><text>&amp;File</text>" ) ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		BOOST_CHECK_EQUAL( L"&File", parser.get_node_string(L"text") ) ;
		BOOST_CHECK_EQUAL( L"", parser.get_node_string(L"tooltip") ) ;
		BOOST_CHECK_EQUAL( L"file", parser.get_node_string(L"id") ) ;
		BOOST_CHECK_EQUAL( L"", parser.get_node_string(L"type") ) ;
	}


BOOST_AUTO_TEST_SUITE_END()
#endif