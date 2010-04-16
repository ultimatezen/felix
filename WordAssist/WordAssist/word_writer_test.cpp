#include "stdafx.h"
#include "word_writer.h"

#ifdef UNIT_TEST
#include "easyunit/testharness.h"

namespace easyunit
{

	TEST( word_writer_test, Instantiate )
	{
		try
		{
			WordSelection selection ;
			word_writer writer(selection) ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate word_writer class in test harness" ) ;
		}
	}
}
#endif
