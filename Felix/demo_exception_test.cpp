#include "stdafx.h"
#include "DemoException.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( CDemoExceptionTest )

	BOOST_AUTO_TEST_CASE( init )
	{
		CDemoException exception ;
		string msg = (LPCSTR)CStringA(exception.what()) ;
		BOOST_CHECK_EQUAL(msg, "Demo Exception") ;
	}
BOOST_AUTO_TEST_SUITE_END()
#endif
