#include "StdAfx.h"
#include ".\system_message.h"


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( test_system_message )


	BOOST_AUTO_TEST_CASE( testint_arg_a)
	{
		CStringA actual = int_arg_a(10) ;
		string expected = "10" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( int_arg_0)
	{
		CStringA actual = int_arg(0) ;
		string expected = "0" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( int_arg_1_001)
	{
		CStringA actual = int_arg(1001) ;
		string expected = "1,001" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( system_message_a_IDS_OPEN_TOOLBAR)
	{
		CStringA actual = system_message_a(IDS_OPEN_TOOLBAR) ;
		string expected = "Open a memory" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( system_message_IDS_OPEN_TOOLBAR)
	{
		CStringA actual = system_message(IDS_OPEN_TOOLBAR) ;
		string expected = "Open a memory" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( system_message_IDS_CLEARED_MEMORY)
	{
		CStringA actual = system_message(IDS_CLEARED_MEMORY, L"foo") ;
		string expected = "Cleared foo" ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
	BOOST_AUTO_TEST_CASE( system_message_IDS_CURRENT_SIZE)
	{
		CStringA actual = system_message(IDS_CURRENT_SIZE, L"foo", int_arg(1234)) ;
		string expected = "foo size is now 1,234." ;
		BOOST_CHECK_EQUAL(expected, string(actual)) ;
	}
BOOST_AUTO_TEST_SUITE_END()