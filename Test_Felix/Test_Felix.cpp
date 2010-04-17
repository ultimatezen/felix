// Test_Felix.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Test_Felix.h"

#define MAX_LOADSTRING 100

#define BOOST_TEST_MODULE ColorRefTests
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE( test_app_state )

BOOST_AUTO_TEST_CASE( initial_values_seg_chars )
{
	BOOST_CHECK_EQUAL(1, 1) ;
}

BOOST_AUTO_TEST_CASE( initial_values_manual_url )
{
	BOOST_CHECK_EQUAL(1, 2) ;
}
BOOST_AUTO_TEST_SUITE_END()


