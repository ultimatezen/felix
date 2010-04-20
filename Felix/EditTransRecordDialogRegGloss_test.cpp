#include "stdafx.h"
#include "EditTransRecordDialogRegGloss.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCEditTransRecordDialogRegGloss )


	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditTransRecordDialogRegGloss dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_RECORD_GLOSS, CEditTransRecordDialogRegGloss::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( is_gloss_mem)
	{
		CEditTransRecordDialogRegGloss dialog ;
		BOOST_CHECK(! dialog.m_is_glossary) ;
	}

	// CEditTransRecordDialogRegGloss message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(2, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL(string(dialog.m_sensing_variable[1].c_str()), "fill_from_record"); 
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CEditTransRecordDialogRegGloss dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(string(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CEditTransRecordDialogRegGloss dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
	{
		CEditTransRecordDialogRegGloss dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(1, (int)result) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif