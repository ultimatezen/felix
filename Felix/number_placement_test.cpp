#include "stdafx.h"
#include "number_placement.h"

#ifdef UNIT_TEST

#include "mem_tests.h"

using namespace mem_engine ;
using namespace placement ;
namespace gp = placement ;

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( number_placement_tests )

	// find_hole
	BOOST_AUTO_TEST_CASE(init)
	{
		placement::number_placer placer ;
		placer ;
		BOOST_CHECK(true) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif