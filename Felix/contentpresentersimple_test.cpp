#include "stdafx.h"
#include ".\contentpresentersimple.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

namespace easyunit
{
	TEST( TestCContentPresenterSimple, instantiate)
	{
		CHtmlView view ;
		CContentPresenterSimple presenter(view, _T("foo")) ;
		ASSERT_TRUE(true) ;
	}

}

#endif