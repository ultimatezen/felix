#include "StdAfx.h"
#include ".\sourceandhtmledit.h"
#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCSourceAndHtmlEdit, instantiate)
	{
		CSourceAndHtmlEdit dialog ;
		ASSERT_TRUE(TRUE) ;
	}
}

#endif