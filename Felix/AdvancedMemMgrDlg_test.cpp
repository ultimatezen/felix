#include "StdAfx.h"
#include "AdvancedMemMgrDlg.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCAdvancedMemMgrDlg )

BOOST_AUTO_TEST_CASE( get_reliability_0 )
{
	CAdvancedMemMgrDlg dlg ;
	int reliability = dlg.parse_reliability(_T("0")) ;
	BOOST_CHECK_EQUAL(reliability, 0) ;
}
BOOST_AUTO_TEST_CASE(get_reliability_blank )
{
	CAdvancedMemMgrDlg dlg ;
	int reliability = dlg.parse_reliability(_T("")) ;
	BOOST_CHECK_EQUAL(reliability, -1) ;
}
BOOST_AUTO_TEST_CASE(get_reliability_10 )
{
	CAdvancedMemMgrDlg dlg ;
	int reliability = dlg.parse_reliability(_T("10")) ;
	BOOST_CHECK_EQUAL(reliability, 10) ;
}

BOOST_AUTO_TEST_CASE(validation )
{
	CAdvancedMemMgrDlg dlg ;
	BOOST_CHECK_EQUAL(dlg.get_validation(), BST_UNCHECKED) ;
	dlg.set_validation(BST_CHECKED) ;
	BOOST_CHECK_EQUAL(dlg.get_validation(), BST_CHECKED) ;
}
BOOST_AUTO_TEST_CASE(reliability )
{
	CAdvancedMemMgrDlg dlg ;
	BOOST_CHECK_EQUAL((int)dlg.get_reliability(), 0) ;
	dlg.set_reliability(10) ;
	BOOST_CHECK_EQUAL((int)dlg.get_reliability(), 10) ;
}
BOOST_AUTO_TEST_CASE(lock )
{
	CAdvancedMemMgrDlg dlg ;
	BOOST_CHECK_EQUAL(dlg.get_lock(), BST_UNCHECKED) ;
	dlg.set_lock(BST_CHECKED) ;
	BOOST_CHECK_EQUAL(dlg.get_lock(), BST_CHECKED) ;
}

// message processing
BOOST_AUTO_TEST_CASE(test_message_ZERO)
{
	CAdvancedMemMgrDlg dialog ;
	LRESULT lResult = 1 ;
	BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
	BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
	BOOST_CHECK(! result) ;
	BOOST_CHECK_EQUAL(1, lResult) ;
}
BOOST_AUTO_TEST_SUITE_END()

#endif
