#include "stdafx.h"

#include "AppStrings.h"


#ifdef UNIT_TEST


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( CAppStringsTests )

	BOOST_AUTO_TEST_CASE(test_Initialize)
	{
		BOOST_CHECK_NO_THROW(CAppStrings()) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif