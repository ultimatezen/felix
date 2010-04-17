// Test_Felix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BOOST_TEST_MODULE FelixTests
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE( foo )

BOOST_AUTO_TEST_CASE( init )
{
	BOOST_CHECK ( 1 == 1 ) ; 
}


BOOST_AUTO_TEST_SUITE_END()
