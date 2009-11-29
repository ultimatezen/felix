#include "StdAfx.h"
#include "tag.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCTag, empty_attribute )
	{
		CTag<char> tag ;
		string raw = "<b>" ;
		tag.parse_tag(raw) ;
		SimpleString expected("") ;
		string key = "style" ;
		string actual = tag.get_attribute(key) ;
		ASSERT_EQUALS_V(expected, actual.c_str()) ;
	}

}

#endif