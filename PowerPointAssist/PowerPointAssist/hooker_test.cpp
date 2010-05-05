#include "stdafx.h"
#include "Hooker.h"
#include "KeyboardDriver.h"

#ifdef UNIT_TEST

namespace easyunit
{

	TEST(HookerTests, shift_key_is_pressed)
	{

		ASSERT_TRUE( ! shift_key_is_pressed() ) ;

	}

}

#endif // #ifdef _DEBUG
