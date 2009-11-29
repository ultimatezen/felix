#include "StdAfx.h"
#include "AdvancedMemMgrDlg.h"

#include "easyunit/testharness.h"

#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCAdvancedMemMgrDlg, get_reliability_0 )
	{
		CAdvancedMemMgrDlg dlg ;
		int reliability = dlg.parse_reliability(_T("0")) ;
		ASSERT_EQUALS_V(reliability, 0) ;
	}
	TEST( TestCAdvancedMemMgrDlg, get_reliability_blank )
	{
		CAdvancedMemMgrDlg dlg ;
		int reliability = dlg.parse_reliability(_T("")) ;
		ASSERT_EQUALS_V(reliability, -1) ;
	}
	TEST( TestCAdvancedMemMgrDlg, get_reliability_10 )
	{
		CAdvancedMemMgrDlg dlg ;
		int reliability = dlg.parse_reliability(_T("10")) ;
		ASSERT_EQUALS_V(reliability, 10) ;
	}

	TEST( TestCAdvancedMemMgrDlg, validation )
	{
		CAdvancedMemMgrDlg dlg ;
		ASSERT_EQUALS_V(dlg.get_validation(), BST_UNCHECKED) ;
		dlg.set_validation(BST_CHECKED) ;
		ASSERT_EQUALS_V(dlg.get_validation(), BST_CHECKED) ;
	}
	TEST( TestCAdvancedMemMgrDlg, reliability )
	{
		CAdvancedMemMgrDlg dlg ;
		ASSERT_EQUALS_V((int)dlg.get_reliability(), -1) ;
		dlg.set_reliability(10) ;
		ASSERT_EQUALS_V((int)dlg.get_reliability(), 10) ;
	}
	TEST( TestCAdvancedMemMgrDlg, lock )
	{
		CAdvancedMemMgrDlg dlg ;
		ASSERT_EQUALS_V(dlg.get_lock(), BST_UNCHECKED) ;
		dlg.set_lock(BST_CHECKED) ;
		ASSERT_EQUALS_V(dlg.get_lock(), BST_CHECKED) ;
	}

	// message processing
	TEST( TestCAdvancedMemMgrDlg, test_message_WM_INITDIALOG)
	{
		CAdvancedMemMgrDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCAdvancedMemMgrDlg, test_message_IDOK)
	{
		CAdvancedMemMgrDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCAdvancedMemMgrDlg, test_message_IDCANCEL)
	{
		CAdvancedMemMgrDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCAdvancedMemMgrDlg, test_message_ZERO)
	{
		CAdvancedMemMgrDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
}


#endif // #ifdef _DEBUG