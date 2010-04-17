#include "stdafx.h"
#include "InputKeyDlg.h"
#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCInputKeyDlg, instantiate)
	{
		CInputKeyDlg dialog ;
		ASSERT_EQUALS_V(IDD_INPUT_KEY_DLG, CInputKeyDlg::IDD) ;
		ASSERT_EQUALS_V(3, 3) ;
	}
	 CExportDialog message processing
	TEST( TestCInputKeyDlg, test_message_WM_INITDIALOG)
	{
		CInputKeyDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCInputKeyDlg, test_message_IDOK)
	{
		CInputKeyDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCInputKeyDlg, test_message_IDCANCEL)
	{
		CInputKeyDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCInputKeyDlg, test_message_ZERO)
	{
		CInputKeyDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

}

#endif