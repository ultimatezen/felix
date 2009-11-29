#include "stdafx.h"
#include ".\extrastringsview.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCExtraStringsView, instantiate)
	{
		CExtraStringsView dialog ;
		ASSERT_TRUE(true) ;
	}
	TEST( TestCExtraStringsView, eliminate_whitespace_and_punctuation )
	{
		CExtraStringsView dlg ;
		wstring setting = L"Hello, world!" ;
		wstring key ;
		dlg.eliminate_whitespace_and_punctuation( setting, key ) ;
		ASSERT_EQUALS( L"Hello__world_", key ) ;
	}

}

#endif