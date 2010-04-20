#include "stdafx.h"
#include "ImportDialog.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCImportDialog )

	// CImportDialog
	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CImportDialog dialog ;
		BOOST_CHECK_EQUAL(IDD_IMPORT_DIALOG, CImportDialog::IDD) ;
	}

	// CImportDialog message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CImportDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( OnOK_throws)
	{
		CImportDialog dialog ;
		BOOST_CHECK(dialog.m_source.empty()) ;
		BOOST_CHECK(dialog.m_trans.empty()) ;

		BOOL dummy(FALSE) ;
		BOOST_CHECK_THROW(dialog.OnOK(0, 0, NULL, dummy), except::CProgramException) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CImportDialog dialog ;
		dialog.m_source = _T("spam") ;
		dialog.m_trans = _T("egg") ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		BOOST_CHECK_EQUAL(IDOK, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CImportDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CImportDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestCExportDialog )

	// CExportDialog
	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CExportDialog dialog ;
		BOOST_CHECK_EQUAL(IDD_IMPORT_DIALOG, CExportDialog::IDD) ;
	}
	// CExportDialog message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CExportDialog dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CExportDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnOK"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CExportDialog dialog ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( string(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CExportDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif