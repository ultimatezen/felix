#include "stdafx.h"
#include "EditTransRecordDialogModal.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCEditTransRecordDialogModal, instantiate)
	{
		CEditTransRecordDialogModal dialog ;
		ASSERT_EQUALS_V(IDD_EDIT_RECORD, CEditTransRecordDialogModal::IDD) ;
	}
	TEST( TestCEditTransRecordDialogModal, is_gloss_mem)
	{
		CEditTransRecordDialogModal dialog ;
		ASSERT_TRUE(! dialog.m_is_glossary) ;
	}

	// CEditTransRecordDialogModal message processing
	TEST( TestCEditTransRecordDialogModal, test_message_WM_INITDIALOG)
	{
		CEditTransRecordDialogModal dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogModal, test_message_IDOK)
	{
		CEditTransRecordDialogModal dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogModal, test_message_IDCANCEL)
	{
		CEditTransRecordDialogModal dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogModal, test_message_IDCLOSE)
	{
		CEditTransRecordDialogModal dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialogModal, test_message_ZERO)
	{
		CEditTransRecordDialogModal dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
}

#endif