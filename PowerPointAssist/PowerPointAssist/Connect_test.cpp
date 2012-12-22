#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"

#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>

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
	TEST( CConnectTest, OnDisconnection )
	{
			connPtr conn ;
			ASSERT_TRUE(SUCCEEDED(CComObject< CConnect >::CreateInstance( &conn ))) ;
			ASSERT_TRUE( SUCCEEDED( conn->OnDisconnection(AddInDesignerObjects::ext_dm_UserClosed, NULL) ) ) ;
	}

	TEST( CConnectTest, OnAddInsUpdate )
	{
		connPtr conn ;
		ASSERT_TRUE(SUCCEEDED(CComObject< CConnect >::CreateInstance( &conn ))) ;
		ASSERT_TRUE( SUCCEEDED( conn->OnAddInsUpdate(NULL) ) ) ;
	}
	TEST( CConnectTest, OnStartupComplete )
	{
		connPtr conn ;
		ASSERT_TRUE(SUCCEEDED(CComObject< CConnect >::CreateInstance( &conn ))) ;
		ASSERT_TRUE( SUCCEEDED( conn->OnStartupComplete(NULL) ) ) ;
	}
	TEST( CConnectTest, OnBeginShutdown )
	{
		connPtr conn ;
		ASSERT_TRUE(SUCCEEDED(CComObject< CConnect >::CreateInstance( &conn ))) ;
		ASSERT_TRUE( SUCCEEDED( conn->OnBeginShutdown(NULL) ) ) ;
	}

}
#endif
