#include "stdafx.h"
#include "DemoException.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( CDemoExceptionTest, init )
	{
		CDemoException exception ;
		SimpleString msg = (LPCSTR)CStringA(exception.what()) ;
		ASSERT_EQUALS_V(msg, "Demo Exception") ;
	}
}
#endif
