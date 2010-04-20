#include "StdAfx.h"
#include ".\sourceandhtmledit.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCSourceAndHtmlEdit )
	BOOST_AUTO_TEST_CASE(instantiate)
	{
		BOOST_CHECK_NO_THROW(CSourceAndHtmlEdit()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif