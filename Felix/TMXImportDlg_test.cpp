#include "stdafx.h"
#include "TmxImportDlg.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCTMXImportDlg, instantiate)
	{
		CTMXImportDlg dialog ;
		ASSERT_EQUALS_V(IDD_SELECT_LANG_DLG, CTMXImportDlg::IDD) ;
	}
	TEST( TestCTMXImportDlg, test_message_WM_INITDIALOG)
	{
		CTMXImportDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCTMXImportDlg, test_message_IDCANCEL)
	{
		CTMXImportDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCTMXImportDlg, test_message_IDOK)
	{
		CTMXImportDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCTMXImportDlg, test_message_ZERO)
	{
		CTMXImportDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

}

#endif
#endif