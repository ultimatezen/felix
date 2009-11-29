#include "StdAfx.h"
#include "ClipboardBackup.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG

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