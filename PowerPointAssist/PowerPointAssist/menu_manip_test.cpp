#include "stdafx.h"
#include "menu_manip.h"

#ifdef _DEBUG

namespace easyunit
{
	TEST( menu_manip_test, item_needs_killing )
	{
		_bstr_t caption = L"Feli&x" ;
		ASSERT_TRUE( item_needs_killing( caption ) ) ;
		caption = L"Felix Interface" ;
		ASSERT_TRUE( item_needs_killing( caption ) ) ;

		caption = L"Menu Bar" ;
		ASSERT_TRUE( ! item_needs_killing( caption ) ) ;
		caption = L"" ;
		ASSERT_TRUE( ! item_needs_killing( caption ) ) ;
	}
}
#endif
