#include "StdAfx.h"
#include "numberfmt.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCNumberFormat, zero )
	{
		CNumberFmt fmt ;
		CStringA actual = fmt.Format(0) ;
		CStringA expected = "0" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCNumberFormat, format_1_234 )
	{
		CNumberFmt fmt ;
		CStringA actual = fmt.Format(1234) ;
		CStringA expected = "1,234" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCNumberFormat, format_1_001_001 )
	{
		CNumberFmt fmt ;
		CStringA actual = fmt.Format(1001001) ;
		CStringA expected = "1,001,001" ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
}

#endif // #ifdef _DEBUG