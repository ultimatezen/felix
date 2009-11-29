#include "stdafx.h"
#include "InputSymbolDlg.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCInputSymbolDlg, instantiate)
	{
		CInputSymbolDlg dialog ;
		ASSERT_EQUALS_V(IDD_INPUT_SYMBOL_DLG, CInputSymbolDlg::IDD) ;
	}
	TEST( TestCInputSymbolDlg, get_text_alpha)
	{
		CInputSymbolDlg dialog ;
		_tcscpy(dialog.m_text, _T("alpha")) ;
		SimpleString expected("&alpha;") ;
		SimpleString actual = CW2A(dialog.get_text().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCInputSymbolDlg, get_text_empty)
	{
		CInputSymbolDlg dialog ;
		_tcscpy(dialog.m_text, _T("")) ;
		SimpleString expected("") ;
		SimpleString actual = CW2A(dialog.get_text().c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	// CInputSymbolDlg message processing
	TEST( TestCInputSymbolDlg, test_message_WM_INITDIALOG)
	{
		CInputSymbolDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCInputSymbolDlg, test_message_IDOK)
	{
		CInputSymbolDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCInputSymbolDlg, test_message_IDCANCEL)
	{
		CInputSymbolDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCInputSymbolDlg, test_message_ZERO)
	{
		CInputSymbolDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}


}

#endif