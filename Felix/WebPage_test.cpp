#include "StdAfx.h"
#include "WebPage.h"
#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	using namespace html ;
	TEST( CWebPage, instantiate)
	{
		CWebPage page ;
		ASSERT_TRUE(true) ;
	}
	TEST( CWebPage, GetSystemErrorMessage)
	{
		wstring err = GetSystemErrorMessage(2) ;
		SimpleString actual(string2string(err, CP_UTF8).c_str()) ;
		wstring exp_err(L"指定されたファイルが見つかりません。\r\n") ;
		SimpleString expected(string2string(exp_err, CP_UTF8).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( CWebPage, GetNextToken)
	{
		CString strDelim(" \n\r\t"), 
			strSrc("spam\n");

		CStringA actual = GetNextToken(strSrc, strDelim) ;
		SimpleString expected = "spam" ;
		ASSERT_EQUALS_V(expected, SimpleString(actual)) ;
	}

}

#endif