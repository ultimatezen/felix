
#include "StdAfx.h"
#include "WindowsMessage.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( WindowsMessageTests )

	BOOST_AUTO_TEST_CASE( TestInstantiate)
	{
		WindowsMessage message ;

		BOOST_CHECK( message.hWnd == NULL) ;
		BOOST_CHECK( message.uMsg == 0) ;
		BOOST_CHECK( message.wParam == 0) ;
		BOOST_CHECK( message.lParam == 0) ;
		BOOST_CHECK( message.bHandled == true) ;
			// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	}
	BOOST_AUTO_TEST_CASE( TestNotHandled)
	{
		WindowsMessage message ;
		message.notHandled() ;

		BOOST_CHECK(!message.isHandled()) ;
	}
	BOOST_AUTO_TEST_CASE( TestuMsg)
	{
		WindowsMessage message( NULL, WM_CREATE) ;

		BOOST_CHECK( message.hWnd == NULL) ;
		BOOST_CHECK( message.uMsg == WM_CREATE) ;
		BOOST_CHECK( message.wParam == 0) ;
		BOOST_CHECK( message.lParam == 0) ;
		// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	}


BOOST_AUTO_TEST_SUITE_END()

#endif


