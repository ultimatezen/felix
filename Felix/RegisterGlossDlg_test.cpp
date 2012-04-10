#include "StdAfx.h"
#include "RegisterGlossDlg.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCRegisterGlossDlg )


	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CRegisterGlossDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_ADD_GLOSS, CRegisterGlossDlg::IDD) ;
	}
	// trim_and_normalize
	BOOST_AUTO_TEST_CASE( trim_and_normalize_empty)
	{
		_bstr_t before = L"" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_one_letter)
	{
		_bstr_t before = L"a" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"a" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_only_spaces)
	{
		_bstr_t before = L"  " ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_no_spaces)
	{
		_bstr_t before = L"spam" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_simple)
	{
		_bstr_t before = L"spam " ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_middle)
	{
		_bstr_t before = L"spam egg" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam egg" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_tag)
	{
		_bstr_t before = L"spam </B>" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam</B>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_two_tags)
	{
		_bstr_t before = L"spam </B></I>" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam</B></I>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_two_tags_spaces_between)
	{
		_bstr_t before = L"spam </B> </I>" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam</B></I>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_tag_no_space)
	{
		_bstr_t before = L"spam</B>" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam</B>" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_starting_spaces)
	{
		_bstr_t before = L" spam" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_both)
	{
		_bstr_t before = L" spam " ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_nl)
	{
		_bstr_t before = L"spam\negg" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam egg" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_and_normalize_cr_nl)
	{
		_bstr_t before = L"spam\r\negg" ;
		wstring actual = trim_and_normalize(before) ;
		wstring expected = L"spam egg" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}

	// trim_text
	BOOST_AUTO_TEST_CASE( trim_text_tags_before)
	{
		wstring before = L" <b> spam" ;
		wstring actual = trim_text(before) ;
		wstring expected = L"<b>spam" ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}	


	// messages
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog"); 
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCancel"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCLOSE)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnClose"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(TRUE, result) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}





BOOST_AUTO_TEST_SUITE_END()
#endif