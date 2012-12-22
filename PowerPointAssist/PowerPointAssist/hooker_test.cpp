#include "stdafx.h"
#include "Hooker.h"
#include "KeyboardDriver.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( HookerTests )


	BOOST_AUTO_TEST_CASE(test_shift_key_is_pressed)
	{

		BOOST_CHECK( ! shift_key_is_pressed() ) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
