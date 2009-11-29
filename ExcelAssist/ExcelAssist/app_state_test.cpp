#include "stdafx.h"
#include "easyunit/testharness.h"
#include "app_state.h"

#ifdef _DEBUG


namespace easyunit
{
	TEST( test_app_state, toolbar_visible)
	{
		app_state properties ;
		ASSERT_TRUE(properties.get_toolbar_visible()) ;
		properties.set_toolbar_visible(FALSE) ;
		ASSERT_TRUE(! properties.get_toolbar_visible()) ;
	}

}
#endif
