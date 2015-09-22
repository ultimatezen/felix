#include "stdafx.h"
#include "HtmlDocColorSetter.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCHtmlDocColorSetter )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		BOOST_CHECK_NO_THROW(html::CHtmlDocColorSetter());
	}
BOOST_AUTO_TEST_SUITE_END()
#endif