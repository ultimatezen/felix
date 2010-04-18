#include "stdafx.h"
#include "word_writer.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestMemoryFunctions )

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
BOOST_AUTO_TEST_SUITE_END()