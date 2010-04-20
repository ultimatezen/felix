#include "stdafx.h"
#include "InputSymbolDlg.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCInputSymbolDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CInputSymbolDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_INPUT_SYMBOL_DLG, CInputSymbolDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( get_text_alpha)
	{
		CInputSymbolDlg dialog ;
		_tcscpy(dialog.m_text, _T("alpha")) ;
		string expected("&alpha;") ;
		string actual = CW2A(dialog.get_text().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( get_text_empty)
	{
		CInputSymbolDlg dialog ;
		_tcscpy(dialog.m_text, _T("")) ;
		string expected("") ;
		string actual = CW2A(dialog.get_text().c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	// CInputSymbolDlg message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CInputSymbolDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CInputSymbolDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CInputSymbolDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCloseCommand"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CInputSymbolDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif