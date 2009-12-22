#include "stdafx.h"
#include "comutilities.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( ComUtilitiesTest, out_ptr )
	{
		boost::scoped_ptr<int> i(new int(0)) ;
		int *val = i.get() ;
		void **data = out_ptr(&val) ;
		ASSERT_EQUALS((int*)*data, val) ;
	}
}
#endif
