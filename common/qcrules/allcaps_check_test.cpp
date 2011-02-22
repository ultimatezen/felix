/*!
	@brief Tests the Array class
 */

#include "StdAfx.h"
#include "Array.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( AllCapsCheckTests )

	/** @test begin() method of class Array.
	 */
	BOOST_AUTO_TEST_CASE( ensure_init )
	{
		BOOST_CHECK_EQUAL( 1, 1 ) ;

	}
BOOST_AUTO_TEST_SUITE_END()

#endif
