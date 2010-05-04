#include "StdAfx.h"
#include "ClipboardBackup.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( CClipboardBackupTest, Initialize )
	{
		try
		{
			CClipboardBackup backup ;
			ASSERT_TRUE( true ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to initialize clipboard object ") ;
		}
	}
}

#endif