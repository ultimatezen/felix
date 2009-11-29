#include "stdafx.h"
#include "aboutdlg.h"
#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCAboutDialog, instantiate)
	{
		CAboutDialog dialog ;
		ASSERT_EQUALS_V(IDD_ABOUTBOX, CAboutDialog::IDD) ;
	}
	TEST( TestCAboutDialog, EndDialog)
	{
		CAboutDialog dialog ;
		dialog.m_bModal = TRUE ;
		int actual = (int)dialog.OnCloseCmd(IDCANCEL) ;
		ASSERT_EQUALS_V(0, actual) ;
	}
	// message processing
	TEST( TestCAboutDialog, test_message_WM_INITDIALOG)
	{
		CAboutDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCAboutDialog, test_message_IDOK)
	{
		CAboutDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCmd"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCAboutDialog, test_message_IDCANCEL)
	{
		CAboutDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCmd"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCAboutDialog, test_message_IDCLOSE)
	{
		CAboutDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCmd"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCAboutDialog, test_message_ZERO)
	{
		CAboutDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

}

#endif