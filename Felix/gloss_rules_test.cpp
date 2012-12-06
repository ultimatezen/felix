#include "stdafx.h"
#include "gloss_rules.h"

#ifdef UNIT_TEST

using namespace placement ;

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( gloss_rules_tests )

	BOOST_AUTO_TEST_CASE( create_regex_rule )
	{
		BOOST_CHECK_NO_THROW(placement::regex_rule()) ;
	}
	BOOST_AUTO_TEST_CASE( create_regex_rules )
	{
		BOOST_CHECK_NO_THROW(placement::regex_rules()) ;
	}

BOOST_AUTO_TEST_SUITE_END()


#endif