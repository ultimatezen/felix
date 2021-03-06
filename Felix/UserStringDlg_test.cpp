#include "StdAfx.h"
#include "UserStringDlg.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCUserStringDlg )

	BOOST_AUTO_TEST_CASE( init_values)
	{
		CUserStringDlg dlg ;
		wstring key = dlg.GetKey() ;
		wstring val = dlg.GetValue() ;
		BOOST_CHECK(key.empty()) ;
		BOOST_CHECK(val.empty()) ;
	}
	// CUserStringDlg message processing
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CUserStringDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(FALSE, result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()
#endif