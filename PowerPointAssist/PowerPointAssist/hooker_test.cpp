#include "stdafx.h"
#include "Hooker.h"
#include "KeyboardDriver.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

namespace easyunit
{

	TEST(HookerTests, shift_key_is_pressed)
	{

		ASSERT_TRUE( ! shift_key_is_pressed() ) ;

	}

}

#endif // #ifdef _DEBUG
