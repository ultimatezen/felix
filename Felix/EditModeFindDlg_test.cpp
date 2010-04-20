#include "stdafx.h"
#include "EditModeFindDlg.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCEditModeFindDlg )


	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditModeFindDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_FIND_DIALOG, CEditModeFindDlg::IDD) ;
	}


	// CEditModeFindDlg message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CEditModeFindDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDC_SEARCH)
	{
		CEditModeFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SEARCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnSearch"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CEditModeFindDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditModeFindDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	// CEditModeReplaceDlg message processing
BOOST_AUTO_TEST_SUITE( TestCEditModeReplaceDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditModeReplaceDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_MODE_REPLACE_DIALOG, CEditModeReplaceDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CEditModeReplaceDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDC_SEARCH)
	{
		CEditModeReplaceDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_SEARCH, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnSearch"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CEditModeReplaceDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditModeReplaceDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif