#include "StdAfx.h"
#include ".\sourceandhtmledit.h"
#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCSourceAndHtmlEdit, instantiate)
	{
		CSourceAndHtmlEdit dialog ;
		SimpleString expected("") ;
		SimpleString actual(CW2A(dialog.m_text)) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
}

#endif