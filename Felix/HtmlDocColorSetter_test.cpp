#include "stdafx.h"
#include "HtmlDocColorSetter.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCHtmlDocColorSetter, instantiate)
	{
		html::CHtmlDocColorSetter setter ;
		ASSERT_TRUE_M(TRUE, "Should not blow up") ;
	}
}

#endif