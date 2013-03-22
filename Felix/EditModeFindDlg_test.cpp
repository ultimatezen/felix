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
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditModeFindDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()
	// CEditModeReplaceDlg message processing
BOOST_AUTO_TEST_SUITE( TestCEditModeReplaceDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditModeReplaceDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_MODE_REPLACE_DIALOG, CEditModeReplaceDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditModeReplaceDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif