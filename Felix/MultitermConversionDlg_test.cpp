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