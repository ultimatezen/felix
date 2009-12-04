#include "stdafx.h"
#include "ImportDialog.h"
#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	// CImportDialog
	TEST( TestCImportDialog, instantiate)
	{
		CImportDialog dialog ;
		ASSERT_EQUALS_V(IDD_IMPORT_DIALOG, CImportDialog::IDD) ;
	}

	// CExportDialog
	TEST( TestCExportDialog, instantiate)
	{
		CExportDialog dialog ;
		ASSERT_EQUALS_V(IDD_IMPORT_DIALOG, CExportDialog::IDD) ;
	}
	// CImportDialog message processing
	TEST( TestCImportDialog, test_message_WM_INITDIALOG)
	{
		CImportDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCImportDialog, OnOK_throws)
	{
		CImportDialog dialog ;
		ASSERT_TRUE(dialog.m_source.empty()) ;
		ASSERT_TRUE(dialog.m_trans.empty()) ;
		try
		{
			BOOL dummy(FALSE) ;
			dialog.OnOK(0, 0, NULL, dummy) ;
			FAIL_M("Should have thrown due to identical source and trans") ;
		}
		catch (CProgramException&)
		{
			ASSERT_TRUE(true) ;
		}
	}
	TEST( TestCImportDialog, test_message_IDOK)
	{
		CImportDialog dialog ;
		dialog.m_source = _T("spam") ;
		dialog.m_trans = _T("egg") ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V(IDOK, (int)lResult) ;
	}
	TEST( TestCImportDialog, test_message_IDCANCEL)
	{
		CImportDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( TestCImportDialog, test_message_ZERO)
	{
		CImportDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

	// CExportDialog message processing
	TEST( TestCExportDialog, test_message_WM_INITDIALOG)
	{
		CExportDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCExportDialog, test_message_IDOK)
	{
		CExportDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCExportDialog, test_message_IDCANCEL)
	{
		CExportDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCExportDialog, test_message_ZERO)
	{
		CExportDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
}

#endif