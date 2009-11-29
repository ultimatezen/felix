#include "StdAfx.h"
#include ".\system_message.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST(test_system_message, int_arg_a)
	{
		CStringA actual = int_arg_a(10) ;
		SimpleString expected = "10" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_system_message, int_arg_0)
	{
		CStringA actual = int_arg(0) ;
		SimpleString expected = "0" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_system_message, int_arg_1_001)
	{
		CStringA actual = int_arg(1001) ;
		SimpleString expected = "1,001" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_system_message, system_message_a_IDS_OPEN_TOOLBAR)
	{
		CStringA actual = system_message_a(IDS_OPEN_TOOLBAR) ;
		SimpleString expected = "Open a memory" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_system_message, system_message_IDS_OPEN_TOOLBAR)
	{
		CStringA actual = system_message(IDS_OPEN_TOOLBAR) ;
		SimpleString expected = "Open a memory" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_system_message, system_message_IDS_CLEARED_MEMORY)
	{
		CStringA actual = system_message(IDS_CLEARED_MEMORY, L"foo") ;
		SimpleString expected = "Cleared foo" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
	TEST(test_system_message, system_message_IDS_CURRENT_SIZE)
	{
		CStringA actual = system_message(IDS_CURRENT_SIZE, L"foo", int_arg(1234)) ;
		SimpleString expected = "foo size is now 1,234." ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}
}



#endif