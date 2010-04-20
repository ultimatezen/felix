#include "StdAfx.h"
#include "MinViewDlg.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestMinViewDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CMinViewDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_MIN_VIEW_DLG, CMinViewDlg::IDD) ;
	}
	// CExportDialog message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CMinViewDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL( "OnInitDialog", string(dialog.m_sensing_variable[0].c_str())); 
		BOOST_CHECK_EQUAL( 1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CMinViewDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(0, (int)result) ;
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
