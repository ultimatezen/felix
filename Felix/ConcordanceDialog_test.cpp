#include "stdafx.h"
#include "ConcordanceDialog.h"
#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCConcordanceDialog, instantiate)
	{
		CConcordanceDialog dialog ;
		ASSERT_EQUALS_V(IDD_CONCORDANCE_DIALOG, CConcordanceDialog::IDD) ;
	}
	TEST( TestCConcordanceDialog, init_get_text)
	{
		CConcordanceDialog dialog ;
		SimpleString expected("") ;
		SimpleString actual(string2string(dialog.get_text()).c_str()) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCConcordanceDialog, test_message_WM_INITDIALOG)
	{
		CConcordanceDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCConcordanceDialog, test_message_IDOK)
	{
		CConcordanceDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCConcordanceDialog, test_message_IDCANCEL)
	{
		CConcordanceDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCConcordanceDialog, test_message_IDCLOSE)
	{
		CConcordanceDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCConcordanceDialog, test_message_ZERO)
	{
		CConcordanceDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}

}

#endif