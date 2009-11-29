
#include "StdAfx.h"
#include "WindowsMessage.h"

#include "easyunit/testharness.h"

#ifdef _DEBUG


namespace easyunit
{

	TEST( WindowsMessageTests, TestInstantiate)
	{
		WindowsMessage message ;

		ASSERT_EQUALS_M( message.hWnd, NULL, "hWnd should be NULL" ) ;
		ASSERT_EQUALS_M( message.uMsg, 0, "uMsg should be 0" ) ;
		ASSERT_EQUALS_M( message.wParam, 0, "wParam should be 0" ) ;
		ASSERT_EQUALS_M( message.lParam, 0, "lParam should be 0" ) ;
		ASSERT_EQUALS_M( message.bHandled, true, "Message should be handled" ) ;
			// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	}
	TEST( WindowsMessageTests, TestNotHandled)
	{
		WindowsMessage message ;
		message.notHandled() ;

		ASSERT_EQUALS_M( message.isHandled(), false, "Message should be handled" ) ;
	}
	TEST( WindowsMessageTests, TestuMsg)
	{
		WindowsMessage message( NULL, WM_CREATE) ;

		ASSERT_EQUALS_M( message.hWnd, NULL, "hWnd should be NULL" ) ;
		ASSERT_EQUALS_M( message.uMsg, WM_CREATE, "uMsg should be WM_CREATE" ) ;
		ASSERT_EQUALS_M( message.wParam, 0, "wParam should be 0" ) ;
		ASSERT_EQUALS_M( message.lParam, 0, "lParam should be 0" ) ;
		// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
	}

}

#endif // #ifdef _DEBUG


