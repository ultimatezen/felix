#include "stdafx.h"
#include "EditTransRecordDialog.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCEditTransRecordDialog )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditTransRecordDialog dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_RECORD, CEditTransRecordDialog::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( is_gloss_mem)
	{
		CEditTransRecordDialog dialog ;
		BOOST_CHECK(! dialog.m_is_glossary) ;
	}
	BOOST_AUTO_TEST_CASE( fill_from_old_record)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_new_record->set_id(10) ;
		dialog.m_new_record->set_refcount(10) ;
		dialog.m_old_record->set_id(5) ;
		dialog.m_old_record->set_refcount(5) ;
		dialog.fill_from_old_record() ;
		BOOST_CHECK_EQUAL((int)dialog.m_new_record->get_id(), (int)dialog.m_old_record->get_id()) ;
		BOOST_CHECK_EQUAL((int)dialog.m_new_record->get_refcount(), (int)dialog.m_old_record->get_refcount()) ;
	}

	// CEditTransRecordDialog message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CEditTransRecordDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog"); 
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnOK"); 
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditTransRecordDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(result) ;
		BOOST_CHECK_EQUAL(0, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif