#include "stdafx.h"
#include "xpmenu/ToolBarXP.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( ToolBarXPTest, handled )
	{
		CToolBarXP toolbar ;
		toolbar.SetMsgHandled(TRUE) ;
		ASSERT_TRUE_M(toolbar.IsMsgHandled(), "Should have been set to true") ;
		toolbar.SetMsgHandled(FALSE) ;
		ASSERT_TRUE_M(! toolbar.IsMsgHandled(), "Should have been set to false") ;
	}
}
#endif
