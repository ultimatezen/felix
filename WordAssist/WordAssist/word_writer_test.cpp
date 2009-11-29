#include "stdafx.h"
#include "word_writer.h"
#include "easyunit/testharness.h"

#ifdef DEBUG

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
