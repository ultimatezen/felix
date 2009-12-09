#include "stdafx.h"
#include "FindDlg.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCFindDlg, instantiate)
	{
		CFindDlg dialog ;
		ASSERT_EQUALS_V(IDD_FIND_DLG, CFindDlg::IDD) ;
	}
	// CFindDlg message processing
	TEST( TestCFindDlg, test_message_WM_INITDIALOG)
	{
		CFindDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( TestCFindDlg, test_message_IDOK)
	{
		CFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SEARCH, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnSearch"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCFindDlg, test_message_IDCANCEL)
	{
		CFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCFindDlg, test_message_IDCLOSE)
	{
		CFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCFindDlg, test_message_ZERO)
	{
		CFindDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[0].c_str()), "check_focus"); 
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}


}

#endif