#include "stdafx.h"
#include "FindDlg.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( TestCFindDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CFindDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_FIND_DLG, CFindDlg::IDD) ;
	}
	// CFindDlg message processing
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CFindDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "check_focus"); 
		BOOST_CHECK(result) ;
		BOOST_CHECK_EQUAL(0, lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
