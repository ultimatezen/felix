#include "StdAfx.h"
#include "logical.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( test_logical )


	// any
	BOOST_AUTO_TEST_CASE( any_true )
	{
		bool arg1=false, arg2=false, arg3=true, arg4=false;
		BOOST_CHECK(logical::any(arg1, arg2, arg3, arg4)) ;
	}
	BOOST_AUTO_TEST_CASE( any_false )
	{
		bool arg1=false, arg2=false, arg3=false, arg4=false;
		BOOST_CHECK(! logical::any(arg1, arg2, arg3, arg4)) ;
	}
	BOOST_AUTO_TEST_CASE( any_true_6)
	{
		BOOST_CHECK( logical::any(false, false, false, false, false, true)) ;
	}
	// all
	BOOST_AUTO_TEST_CASE( all_true )
	{
		bool arg1=true, arg2=true, arg3=true, arg4=true;
		BOOST_CHECK(logical::all(arg1, arg2, arg3, arg4)) ;
	}
	BOOST_AUTO_TEST_CASE( all_false )
	{
		bool arg1=true, arg2=true, arg3=false, arg4=true;
		BOOST_CHECK(! logical::all(arg1, arg2, arg3, arg4)) ;
	}
	BOOST_AUTO_TEST_CASE( all_true_6 )
	{
		BOOST_CHECK(logical::all(true, true, true, true, true, true)) ;
	}
BOOST_AUTO_TEST_SUITE_END()


#endif // #ifdef _DEBUG