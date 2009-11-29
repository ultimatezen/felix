#include "stdafx.h"
#include "WordControllerWord.h"

#include "easyunit/testharness.h"

#ifdef DEBUG

namespace easyunit
{
	TEST(WordControllerTest, Instantiate)
	{
		try
		{
			CWordControllerWord controller ;
			ASSERT_TRUE(true) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate WordController class in harness" ) ;
		}
	}

	TEST(WordControllerTest, is_ignorable)
	{
		CWordControllerWord controller ;

		wstring text ;

		text = L"34" ;
		ASSERT_TRUE( controller.is_ignorable(text, true, SKIP_IF_J ) ) ;
		ASSERT_TRUE( ! controller.is_ignorable(text, false, SKIP_IF_J ) ) ;

		// SKIP_IF_J
		text = L"–ó•¶" ;
		ASSERT_TRUE( controller.is_ignorable(text, true, SKIP_IF_J ) ) ;
		text = L"spam" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, true, SKIP_IF_J ) ) ;
		text = L"–ó•¶" ;
		ASSERT_TRUE( controller.is_ignorable(text, false, SKIP_IF_J ) ) ;
		text = L"spam" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, false, SKIP_IF_J ) ) ;

		// SKIP_UNLESS_J
		text = L"–ó•¶" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, true, SKIP_UNLESS_J ) ) ;
		text = L"spam" ;
		ASSERT_TRUE( controller.is_ignorable(text, true, SKIP_UNLESS_J ) ) ;
		text = L"–ó•¶" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, false, SKIP_UNLESS_J ) ) ;
		text = L"spam" ;
		ASSERT_TRUE( controller.is_ignorable(text, false, SKIP_UNLESS_J ) ) ;

		// NO_SKIP
		text = L"–ó•¶" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, true, NO_SKIP ) ) ;
		text = L"spam" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, true, NO_SKIP ) ) ;
		text = L"–ó•¶" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, false, NO_SKIP ) ) ;
		text = L"spam" ;
		ASSERT_TRUE( ! controller.is_ignorable(text, false, NO_SKIP ) ) ;
	}


}
#endif