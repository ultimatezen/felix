#include "stdafx.h"
#include "EditTransRecordDialog.h"

#include "easyunit/testharness.h"
#ifdef UNIT_TEST

namespace easyunit
{
	TEST( TestCEditTransRecordDialog, instantiate)
	{
		CEditTransRecordDialog dialog ;
		ASSERT_EQUALS_V(IDD_EDIT_RECORD, CEditTransRecordDialog::IDD) ;
	}
	TEST( TestCEditTransRecordDialog, is_gloss_mem)
	{
		CEditTransRecordDialog dialog ;
		ASSERT_TRUE(! dialog.m_is_glossary) ;
	}
	TEST( TestCEditTransRecordDialog, fill_from_old_record)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_new_record->set_id(10) ;
		dialog.m_new_record->set_refcount(10) ;
		dialog.m_old_record->set_id(5) ;
		dialog.m_old_record->set_refcount(5) ;
		dialog.fill_from_old_record() ;
		ASSERT_EQUALS_V((int)dialog.m_new_record->get_id(), (int)dialog.m_old_record->get_id()) ;
		ASSERT_EQUALS_V((int)dialog.m_new_record->get_refcount(), (int)dialog.m_old_record->get_refcount()) ;
	}

	// CEditTransRecordDialog message processing
	TEST( TestCEditTransRecordDialog, test_message_WM_INITDIALOG)
	{
		CEditTransRecordDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialog, test_message_IDOK)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialog, test_message_IDCANCEL)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialog, test_message_IDCLOSE)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCEditTransRecordDialog, test_message_ZERO)
	{
		CEditTransRecordDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
}

#endif