#include "StdAfx.h"
#include "HtmlDocument.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	using namespace html ;
	TEST( TestWebBrowser, instantiate)
	{
		browser_ptr bptr ;
		CHtmlWebBrowser browser(bptr) ;
		ASSERT_TRUE(! browser.is_instantiated()) ;
	}

}


#endif // #ifdef _DEBUG
