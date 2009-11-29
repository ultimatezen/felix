#include "StdAfx.h"
#include "logical.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	// any
	TEST( test_logical, any_true )
	{
		bool arg1=false, arg2=false, arg3=true, arg4=false;
		ASSERT_TRUE(logical::any(arg1, arg2, arg3, arg4)) ;
	}
	TEST( test_logical, any_false )
	{
		bool arg1=false, arg2=false, arg3=false, arg4=false;
		ASSERT_TRUE(! logical::any(arg1, arg2, arg3, arg4)) ;
	}
	TEST( test_logical, any_true_6)
	{
		ASSERT_TRUE( logical::any(false, false, false, false, false, true)) ;
	}
	// all
	TEST( test_logical, all_true )
	{
		bool arg1=true, arg2=true, arg3=true, arg4=true;
		ASSERT_TRUE(logical::all(arg1, arg2, arg3, arg4)) ;
	}
	TEST( test_logical, all_false )
	{
		bool arg1=true, arg2=true, arg3=false, arg4=true;
		ASSERT_TRUE(! logical::all(arg1, arg2, arg3, arg4)) ;
	}
	TEST( test_logical, all_true_6 )
	{
		ASSERT_TRUE(logical::all(true, true, true, true, true, true)) ;
	}
}


#endif // #ifdef _DEBUG