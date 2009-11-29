#include "stdafx.h"
#include "easyunit/testharness.h"
#include "DispatchWrapper.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( CDispatchWrapperTest, Create )
	{
		CDispatchWrapper wrapper(L"SimpleServer.Test") ;
	}
	TEST( CDispatchWrapperTest, Greet )
	{
		CDispatchWrapper wrapper(L"SimpleServer.Test") ;

		CComVariant name(L"Ryan") ;
		CComVariant result = wrapper.method(L"Greet", name) ;

		ASSERT_EQUALS( CComBSTR(result.bstrVal), CComBSTR(L"Hello, Ryan")) ;
	}

	TEST( CDispatchWrapperTest, NonExistentFunction )
	{
		CDispatchWrapper wrapper(L"SimpleServer.Test") ;
		try
		{
			CComVariant result = wrapper.method(L"Spam") ;
			FAIL_M( "Should have thrown when trying to call nonexistent function") ;
		}
		catch (_com_error& e)
		{
			e ;
			ASSERT_TRUE( true ) ;
		}
	}
	TEST( CDispatchWrapperTest, SplitInt )
	{
		CDispatchWrapper wrapper(L"SimpleServer.Test") ;
		try
		{
			CComVariant item(3) ;
			CComVariant result = wrapper.method(L"Split", item) ;
			FAIL_M( "Should have thrown when trying to split an int") ;
		}
		catch (_com_error& e)
		{
			e ;
			ASSERT_TRUE( true ) ;
		}
	}
	TEST( CDispatchWrapperTest, Split )
	{
		CDispatchWrapper wrapper(L"SimpleServer.Test") ;
		CComVariant sentence = L"this is a sentence" ;
		IDispatchPtr disp = wrapper.method(L"Split", sentence).punkVal ;
		CDispatchWrapper words(disp) ;

		CComVariant count = words.prop_get(L"Count") ;
		ASSERT_EQUALS(count.iVal, 4) ;

		CComVariant word0 = words.method(L"Item", CComVariant(0)) ;
		ASSERT_EQUALS(wstring(word0.bstrVal), L"this") ;
		CComVariant word1 = words.method(L"Item", CComVariant(1)) ;
		ASSERT_EQUALS(wstring(word1.bstrVal), L"is") ;
		CComVariant word2 = words.method(L"Item", CComVariant(2)) ;
		ASSERT_EQUALS(wstring(word2.bstrVal), L"a") ;
		CComVariant word3 = words.method(L"Item", CComVariant(3)) ;
		ASSERT_EQUALS(wstring(word3.bstrVal), L"sentence") ;
	}
}

#endif