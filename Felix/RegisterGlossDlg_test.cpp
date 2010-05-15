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
	// trim_text
	BOOST_AUTO_TEST_CASE( trim_text_empty)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_one_letter)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"a" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "a" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_only_spaces)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"  " ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_no_spaces)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_simple)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam " ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_middle)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam egg" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam egg" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_tag)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam </B>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam</B>" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_two_tags)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam </B></I>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam</B></I>" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_two_tags_spaces_between)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam </B> </I>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam</B></I>" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_tag_no_space)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam</B>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam</B>" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_starting_spaces)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L" spam" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_both)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L" spam " ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_nl)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam\negg" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam egg" ;
		string actual(after) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( trim_text_cr_nl)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam\r\negg" ;
		CStringA after((LPCWSTR)dialog.trim_text(before).c_str()) ;
		string expected = "spam egg" ;
		string actual(after) ;
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
	BOOST_AUTO_TEST_CASE( test_message_WM_SIZE)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_SIZE, SIZE_MINIMIZED, MAKELPARAM(5, 5), lResult, 0)  ;
		BOOST_CHECK_EQUAL(2u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnSize"); 
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[1], "SIZE_MINIMIZED"); 
		BOOST_CHECK_EQUAL(TRUE, result) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_WM_SIZE_shebang)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_SIZE, 0, MAKELPARAM(5, 5), lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnSize"); 
		BOOST_CHECK_EQUAL(TRUE, result) ;
		BOOST_CHECK_EQUAL(0, (int)lResult) ;
	}

	// sizing
	BOOST_AUTO_TEST_CASE( pad_rectangle)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.pad_rectangle(rect, 5) ;

		BOOST_CHECK_EQUAL(15, rect.left) ;
		BOOST_CHECK_EQUAL(15, rect.top) ;
		BOOST_CHECK_EQUAL(5, rect.right) ;
		BOOST_CHECK_EQUAL(5, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_row_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 100, 100) ;
		dialog.resize_row_button(rect, 5, 5) ;

		BOOST_CHECK_EQUAL(95, rect.left) ;
		BOOST_CHECK_EQUAL(95, rect.top) ;
		BOOST_CHECK_EQUAL(100, rect.right) ;
		BOOST_CHECK_EQUAL(100, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_advanced_rect)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 100, 100) ;
		CRect advanced_rect(10, 10, 20, 20) ;
		dialog.resize_advanced_rect(rect, advanced_rect) ;

		BOOST_CHECK_EQUAL(10, rect.left) ;
		BOOST_CHECK_EQUAL(90, rect.top) ;
		BOOST_CHECK_EQUAL(20, rect.right) ;
		BOOST_CHECK_EQUAL(100, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_register_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.resize_register_button(rect) ;

		BOOST_CHECK_EQUAL(10, rect.left) ;
		BOOST_CHECK_EQUAL(10, rect.top) ;
		BOOST_CHECK_EQUAL(10, rect.right) ;
		BOOST_CHECK_EQUAL(10, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_close_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.resize_close_button(rect) ;

		BOOST_CHECK_EQUAL(10, rect.left) ;
		BOOST_CHECK_EQUAL(10, rect.top) ;
		BOOST_CHECK_EQUAL(10, rect.right) ;
		BOOST_CHECK_EQUAL(10, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_advanced_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.resize_advanced_button(rect) ;

		BOOST_CHECK_EQUAL(10, rect.left) ;
		BOOST_CHECK_EQUAL(10, rect.top) ;
		BOOST_CHECK_EQUAL(10, rect.right) ;
		BOOST_CHECK_EQUAL(10, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_client_rect_close)
	{
		CRegisterGlossDlg dialog ;
		dialog.m_CloseBtn.m_window_rect.top = 10 ;
		dialog.m_CloseBtn.m_window_rect.bottom = 20 ;

		CRect rect(10, 10, 100, 100) ;
		dialog.resize_client_rect_close(rect, 5) ;

		BOOST_CHECK_EQUAL(10, rect.left) ;
		BOOST_CHECK_EQUAL(10, rect.top) ;
		BOOST_CHECK_EQUAL(100, rect.right) ;
		BOOST_CHECK_EQUAL(85, rect.bottom) ;
	}
	BOOST_AUTO_TEST_CASE( resize_client_rect_advanced)
	{
		CRegisterGlossDlg dialog ;

		dialog.m_AdvancedBtn.m_window_rect.top = 5 ;
		dialog.m_AdvancedBtn.m_window_rect.bottom = 10 ;

		CRect rect(10, 10, 100, 100) ;
		dialog.resize_client_rect_advanced(rect, 5) ;

		BOOST_CHECK_EQUAL(10, rect.left) ;
		BOOST_CHECK_EQUAL(10, rect.top) ;
		BOOST_CHECK_EQUAL(100, rect.right) ;
		BOOST_CHECK_EQUAL(90, rect.bottom) ;
	}
BOOST_AUTO_TEST_SUITE_END()
#endif