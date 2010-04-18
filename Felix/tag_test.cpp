#include "StdAfx.h"
#include "tag.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestCTag )


	BOOST_AUTO_TEST_CASE( empty_attribute )
	{
		CTag<char> tag ;
		string raw = "<b>" ;
		tag.parse_tag(raw) ;
		string expected("") ;
		string key = "style" ;
		string actual = tag.get_attribute(key) ;
		BOOST_CHECK_EQUAL(expected, actual.c_str()) ;
	}

BOOST_AUTO_TEST_SUITE_END()