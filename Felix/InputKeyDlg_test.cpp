#include "stdafx.h"
#include "InputKeyDlg.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCInputKeyDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		BOOST_CHECK_NO_THROW(CInputKeyDlg()) ;
	}
	BOOST_AUTO_TEST_CASE( id_val)
	{
		CInputKeyDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_INPUT_KEY_DLG, CInputKeyDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CInputKeyDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnInitDialog)
	{
		CInputKeyDlg dialog ;
		LRESULT lResult = dialog.OnInitDialog() ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnInitDialog", dialog.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnOK_sensed)
	{
		CInputKeyDlg dialog ;
		LRESULT lResult = dialog.OnOK(1) ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnOK", dialog.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(0, lResult) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_key_text)
	{
		CInputKeyDlg dialog ;
		CString text = L"a\nb\rc\td e" ;
		string key = dialog.get_key_text(text) ;
		BOOST_CHECK_EQUAL("abcde", key) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_key_text_quoted)
	{
		CInputKeyDlg dialog ;
		CString text = L"> abc\n> def" ;
		string key = dialog.get_key_text(text) ;
		BOOST_CHECK_EQUAL("abcdef", key) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif