#include "StdAfx.h"
#include "MultitermConversionDlg.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestMultitermConversionDlg )


	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CMultitermConversionDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_MULTITERM_CONVERSION_DLG, CMultitermConversionDlg::IDD) ;
	}

	// CExportDialog message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CMultitermConversionDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog"); 
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CMultitermConversionDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnOK"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CMultitermConversionDlg dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDC_MULTI55)
	{
		CMultitermConversionDlg dialog ;
		dialog.m_bModal = TRUE ;
		dialog.m_multiterm_version = 0 ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_MULTI55, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(dialog.get_multiterm_version(), IDC_MULTI55) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDC_MULTI6)
	{
		CMultitermConversionDlg dialog ;
		dialog.m_bModal = TRUE ;
		dialog.m_multiterm_version = 0 ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDC_MULTI6, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(dialog.get_multiterm_version(), IDC_MULTI6) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CMultitermConversionDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(FALSE, result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif