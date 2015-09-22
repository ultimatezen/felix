#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"


#ifdef UNIT_TEST
#include "easyunit/testharness.h"

namespace easyunit
{
	typedef CComPtr< CComObject< CConnect > > connPtr ;
	TEST( CConnectTest, Instantiate )
	{
		try
		{
			connPtr conn ;
			HRESULT hr = CComObject< CConnect >::CreateInstance( &conn ) ;
			ASSERT_TRUE( SUCCEEDED( hr ) ) ;
			ASSERT_TRUE( conn ) ;
		}
		catch (...)
		{
			FAIL_M( "Failed to instantiate CConnect class in harness" ) ;
		}
	}
}
#endif
