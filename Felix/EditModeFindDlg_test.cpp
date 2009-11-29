#include "stdafx.h"
#include "EditModeFindDlg.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCEditModeFindDlg, instantiate)
	{
		CEditModeFindDlg dialog ;
		ASSERT_EQUALS_V(IDD_EDIT_FIND_DIALOG, CEditModeFindDlg::IDD) ;
	}
	TEST( TestCEditModeReplaceDlg, instantiate)
	{
		CEditModeReplaceDlg dialog ;
		ASSERT_EQUALS_V(IDD_EDIT_MODE_REPLACE_DIALOG, CEditModeReplaceDlg::IDD) ;
	}

	// CEditModeFindDlg message processing
	TEST( TestCEditModeFindDlg, test_message_WM_INITDIALOG)
	{
		CEditModeFindDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCEditModeFindDlg, test_message_IDC_SEARCH)
	{
		CEditModeFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SEARCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnSearch"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditModeFindDlg, test_message_IDCANCEL)
	{
		CEditModeFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditModeFindDlg, test_message_ZERO)
	{
		CEditModeFindDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
	// CEditModeReplaceDlg message processing
	TEST( TestCEditModeReplaceDlg, test_message_WM_INITDIALOG)
	{
		CEditModeReplaceDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCEditModeReplaceDlg, test_message_IDC_SEARCH)
	{
		CEditModeReplaceDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SEARCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnSearch"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditModeReplaceDlg, test_message_IDCANCEL)
	{
		CEditModeReplaceDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditModeReplaceDlg, test_message_ZERO)
	{
		CEditModeReplaceDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
}

#endif