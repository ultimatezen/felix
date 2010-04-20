#include "stdafx.h"
#include "TmxImportDlg.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST


BOOST_AUTO_TEST_SUITE( TestCTMXImportDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CTMXImportDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_SELECT_LANG_DLG, CTMXImportDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CTMXImportDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CTMXImportDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CTMXImportDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CTMXImportDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif