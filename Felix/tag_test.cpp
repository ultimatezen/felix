#include "StdAfx.h"
#include "tag.h"
#include "tag_name_holder.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_tag_name_holder )

	BOOST_AUTO_TEST_CASE( is_query_tag_true )
	{
		tag_name_holder &tags = tag_name_holder::instance() ;
		wstring name = L"query" ;
		BOOST_CHECK(tags.is_query_tag(name)) ;
	}

	BOOST_AUTO_TEST_CASE( is_query_tag_false )
	{
		tag_name_holder &tags = tag_name_holder::instance() ;
		wstring name = L"foo" ;
		BOOST_CHECK(! tags.is_query_tag(name)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCTag )

	BOOST_AUTO_TEST_CASE( empty_attribute )
	{
		CTag<char> tag ;
		string raw = "<b>" ;
		tag.parse_tag(raw) ;
		string key = "style" ;
		BOOST_CHECK_EQUAL("", tag.get_attribute(key)) ;
	}
	BOOST_AUTO_TEST_CASE( get_tag )
	{
		CTag<char> tag ;
		string raw = "<b foo=\"bar\">" ;
		tag.parse_tag(raw) ;
		BOOST_CHECK_EQUAL("b", tag.get_tag()) ;
	}
	BOOST_AUTO_TEST_CASE( get_tag_w )
	{
		CTag<wchar_t> tag ;
		wstring raw = L"<b foo=\"bar\">" ;
		tag.parse_tag(raw) ;
		BOOST_CHECK_EQUAL(L"b", tag.get_tag()) ;
	}
	BOOST_AUTO_TEST_CASE( is_end_tag_false )
	{
		CTag<char> tag ;
		string raw = "<b foo=\"bar\">" ;
		tag.parse_tag(raw) ;
		BOOST_CHECK(! tag.is_end_tag()) ;
	}	
	BOOST_AUTO_TEST_CASE( is_end_tag_true )
	{
		CTag<char> tag ;
		string raw = "</b>" ;
		tag.parse_tag(raw) ;
		BOOST_CHECK(tag.is_end_tag()) ;
		BOOST_CHECK_EQUAL("b", tag.get_tag()) ;
	}	
	BOOST_AUTO_TEST_CASE( double_quote_attr )
	{
		CTag<char> tag ;
		string raw = "<b foo=\"bar\">" ;
		tag.parse_tag(raw) ;
		string key = "foo" ;
		BOOST_CHECK_EQUAL("bar", tag.get_attribute(key)) ;
	}
	BOOST_AUTO_TEST_CASE( single_quote_attr )
	{
		CTag<char> tag ;
		string raw = "<b foo='bar'>" ;
		tag.parse_tag(raw) ;
		string key = "foo" ;
		BOOST_CHECK_EQUAL("bar", tag.get_attribute(key)) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif
