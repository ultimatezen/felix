#include "stdafx.h"

#include "AppStrings.h"


#ifdef UNIT_TEST

namespace easyunit
{
	TEST(CAppStringsTests, Initialize)
	{
		try
		{
			CAppStrings app_strings ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Exception initializing CAppStrings" ) ;
		}
	}
	TEST(CAppStringsTests, load_english)
	{
		CAppStrings app_strings ;
		app_strings.load(LANG_ENGLISH) ;
		CStringA get_string((LPCWSTR)app_strings.get_get()) ;
		ASSERT_EQUALS_V("Ge&t  (Alt+DWN ARR)", get_string) ;
	}
}
#endif