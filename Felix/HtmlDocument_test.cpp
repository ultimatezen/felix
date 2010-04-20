#include "StdAfx.h"
#include "HtmlDocument.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestWebBrowser )

	using namespace html ;
	BOOST_AUTO_TEST_CASE( instantiate)
	{
		browser_ptr bptr ;
		CHtmlWebBrowser browser(bptr) ;
		BOOST_CHECK(! browser.is_instantiated()) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef _DEBUG
