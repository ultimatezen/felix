#include "stdafx.h"
#include "ConcordanceDialog.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCConcordanceDialog )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CConcordanceDialog dialog ;
		BOOST_CHECK_EQUAL(IDD_CONCORDANCE_DIALOG, CConcordanceDialog::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( init_get_text)
	{
		CConcordanceDialog dialog ;
		string expected("") ;
		string actual(string2string(dialog.get_text()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CConcordanceDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CConcordanceDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CConcordanceDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
	{
		CConcordanceDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CConcordanceDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
