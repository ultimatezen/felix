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
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CImportDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
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
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CExportDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif