#include "StdAfx.h"
#include "AppStringTextNodeParser.h"


#ifdef UNIT_TEST

namespace easyunit
{
	TEST( CAppStringTextNodeParserTest, EmptyNode )
	{
		wstring empty_file_text ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		ASSERT_EQUALS( L"", parser.get_node_string(L"text") ) ;
		ASSERT_EQUALS( L"", parser.get_node_string(L"tooltip") ) ;
		ASSERT_EQUALS( L"", parser.get_node_string(L"id") ) ;
		ASSERT_EQUALS( L"", parser.get_node_string(L"type") ) ;
	}
	TEST( CAppStringTextNodeParserTest, SimpleNodes )
	{
		wstring empty_file_text(L"<type>menu</type><id>file</id><text>File</text><tooltip>tooltip</tooltip>" ) ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		ASSERT_EQUALS( L"File", parser.get_node_string(L"text") ) ;
		ASSERT_EQUALS( L"tooltip", parser.get_node_string(L"tooltip") ) ;
		ASSERT_EQUALS( L"file", parser.get_node_string(L"id") ) ;
		ASSERT_EQUALS( L"menu", parser.get_node_string(L"type") ) ;
	}
	TEST( CAppStringTextNodeParserTest, ComplexNodes )
	{
		wstring empty_file_text(L"<type>menu</type><id>file</id><text>&lt;&amp;File&gt;\\tShortcut</text><tooltip>tooltip\\nPlus</tooltip>" ) ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		ASSERT_EQUALS( L"<&File>\tShortcut", parser.get_node_string(L"text") ) ;
		ASSERT_EQUALS( L"tooltip\nPlus", parser.get_node_string(L"tooltip") ) ;
		ASSERT_EQUALS( L"file", parser.get_node_string(L"id") ) ;
		ASSERT_EQUALS( L"menu", parser.get_node_string(L"type") ) ;
	}
	TEST( CAppStringTextNodeParserTest, MissingNodes )
	{
		wstring empty_file_text(L"<id>file</id><text>&amp;File</text>" ) ;
		CAppStringTextNodeParser parser( empty_file_text.c_str() ) ;
		ASSERT_EQUALS( L"&File", parser.get_node_string(L"text") ) ;
		ASSERT_EQUALS( L"", parser.get_node_string(L"tooltip") ) ;
		ASSERT_EQUALS( L"file", parser.get_node_string(L"id") ) ;
		ASSERT_EQUALS( L"", parser.get_node_string(L"type") ) ;
	}


}
#endif