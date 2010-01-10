#include "stdafx.h"
#include "EditTransRecordDialogRegGloss.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCEditTransRecordDialogRegGloss, instantiate)
	{
		CEditTransRecordDialogRegGloss dialog ;
		ASSERT_EQUALS_V(IDD_EDIT_RECORD_GLOSS, CEditTransRecordDialogRegGloss::IDD) ;
	}
	TEST( TestCEditTransRecordDialogRegGloss, is_gloss_mem)
	{
		CEditTransRecordDialogRegGloss dialog ;
		ASSERT_TRUE(! dialog.m_is_glossary) ;
	}

	// CEditTransRecordDialogRegGloss message processing
	TEST( TestCEditTransRecordDialogRegGloss, test_message_WM_INITDIALOG)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[1].c_str()), "fill_from_record"); 
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogRegGloss, test_message_IDOK)
	{
		CEditTransRecordDialogRegGloss dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogRegGloss, test_message_IDCANCEL)
	{
		CEditTransRecordDialogRegGloss dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogRegGloss, test_message_IDCLOSE)
	{
		CEditTransRecordDialogRegGloss dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogRegGloss, test_message_ZERO)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
}

#endif