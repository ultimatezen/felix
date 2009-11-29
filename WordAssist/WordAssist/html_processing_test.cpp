#include "stdafx.h"
#include "html_processing.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	TEST( html_processing_test, Instantiate )
	{
		try
		{
			html_processing processing ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate html_processing class in test harness" ) ;
		}
	}
}
#endif
