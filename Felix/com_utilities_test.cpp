#include "stdafx.h"
#include "comutilities.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( ComUtilitiesTest )

	BOOST_AUTO_TEST_CASE( test_out_ptr )
	{
		boost::scoped_ptr<int> i(new int(0)) ;
		int *val = i.get() ;
		void **data = out_ptr(&val) ;
		BOOST_CHECK((int*)*data == val) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
