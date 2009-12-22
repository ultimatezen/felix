#include "stdafx.h"
#include "FelixMemDocUIHandler.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( CFelixMemDocUIHandlerTest, FinalConstruct )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		ASSERT_TRUE_M(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)), "Creation should succeed");
		ASSERT_TRUE_M(SUCCEEDED(handler->FinalConstruct()), "Final construct should succeed") ;
	}
	TEST( CFelixMemDocUIHandlerTest, ShowUI )
	{
		CComObject<CFelixMemDocUIHandler> *handler = NULL;
		ASSERT_TRUE_M(SUCCEEDED(CComObject<CFelixMemDocUIHandler>::CreateInstance (&handler)), "Creation should succeed");
		HRESULT retval = S_OK ;
		ASSERT_TRUE_M(E_NOTIMPL == handler->ShowUI(0, NULL, NULL, NULL, NULL, &retval), "ShowUI should return E_NOTIMPL") ;
	}
}
#endif
