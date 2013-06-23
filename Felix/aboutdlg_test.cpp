#include "stdafx.h"
#include "aboutdlg.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestCAboutDialog )

BOOST_AUTO_TEST_CASE( EndDialog)
{
	CAboutDialog dialog ;
	int actual = (int)dialog.OnCloseCmd(IDCANCEL) ;
	BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL( dialog.m_sensing_variable[0], "OnCloseCmd"); 
	BOOST_CHECK_EQUAL(0, actual) ;
}
// message processing
BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
{
	CAboutDialog dialog ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL( dialog.m_sensing_variable[0], "OnInitDialog"); 
	BOOST_CHECK_EQUAL( 1, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_IDOK)
{
	CAboutDialog dialog ;
	dialog.m_bModal = TRUE ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL( dialog.m_sensing_variable[0], "OnCloseCmd"); 
	BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
{
	CAboutDialog dialog ;
	dialog.m_bModal = TRUE ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL( dialog.m_sensing_variable[0], "OnCloseCmd"); 
	BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
{
	CAboutDialog dialog ;
	dialog.m_bModal = TRUE ;
	LRESULT lResult = 1 ;
	dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(1, (int)dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL( dialog.m_sensing_variable[0], "OnCloseCmd"); 
	BOOST_CHECK_EQUAL( 0, (int)lResult) ;
}
BOOST_AUTO_TEST_CASE( test_message_ZERO)
{
	CAboutDialog dialog ;
	LRESULT lResult = 1 ;
	BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(0, (int)dialog.m_sensing_variable.size()) ;
	BOOST_CHECK_EQUAL(0, (int)result) ;
	BOOST_CHECK_EQUAL(1, (int)lResult) ;
}

BOOST_AUTO_TEST_SUITE_END()

#endif