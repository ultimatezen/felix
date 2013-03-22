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

	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CTMXImportDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(FALSE, result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif