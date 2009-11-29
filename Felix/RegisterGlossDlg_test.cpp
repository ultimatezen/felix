#include "StdAfx.h"
#include "RegisterGlossDlg.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

#ifdef UNIT_TEST
namespace easyunit
{
	TEST( TestCRegisterGlossDlg, instantiate)
	{
		CRegisterGlossDlg dialog ;
		ASSERT_EQUALS_V(IDD_ADD_GLOSS, CRegisterGlossDlg::IDD) ;
	}
	// trim_text
	TEST( TestCRegisterGlossDlg, trim_text_empty)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_one_letter)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"a" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "a" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_only_spaces)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"  " ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_no_spaces)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_simple)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam " ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_middle)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam egg" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam egg" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_tag)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam </B>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam</B>" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_two_tags)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam </B></I>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam</B></I>" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_two_tags_spaces_between)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam </B> </I>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam</B></I>" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_tag_no_space)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L"spam</B>" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam</B>" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_starting_spaces)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L" spam" ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCRegisterGlossDlg, trim_text_both)
	{
		CRegisterGlossDlg dialog ;
		_bstr_t before = L" spam " ;
		CStringA after((LPCWSTR)dialog.trim_text(before)) ;
		SimpleString expected = "spam" ;
		SimpleString actual(after) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}

	// messages
	TEST( TestCRegisterGlossDlg, test_message_WM_INITDIALOG)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}
	TEST( TestCRegisterGlossDlg, test_message_IDCANCEL)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCancel"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCRegisterGlossDlg, test_message_IDCLOSE)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCLOSE, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnClose"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCRegisterGlossDlg, test_message_ZERO)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( TestCRegisterGlossDlg, test_message_WM_SIZE)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_SIZE, SIZE_MINIMIZED, MAKELPARAM(5, 5), lResult, 0)  ;
		ASSERT_EQUALS_V(2, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[0].c_str()), "OnSize"); 
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[1].c_str()), "SIZE_MINIMIZED"); 
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}
	TEST( TestCRegisterGlossDlg, test_message_WM_SIZE_shebang)
	{
		CRegisterGlossDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_SIZE, 0, MAKELPARAM(5, 5), lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(SimpleString(dialog.m_sensing_variable[0].c_str()), "OnSize"); 
		ASSERT_EQUALS_V(1, (int)result) ;
		ASSERT_EQUALS_V(0, (int)lResult) ;
	}

	// sizing
	TEST( TestCRegisterGlossDlg, pad_rectangle)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.pad_rectangle(rect, 5) ;

		ASSERT_EQUALS_V(15, rect.left) ;
		ASSERT_EQUALS_V(15, rect.top) ;
		ASSERT_EQUALS_V(5, rect.right) ;
		ASSERT_EQUALS_V(5, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_row_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 100, 100) ;
		dialog.resize_row_button(rect, 5, 5) ;

		ASSERT_EQUALS_V(95, rect.left) ;
		ASSERT_EQUALS_V(95, rect.top) ;
		ASSERT_EQUALS_V(100, rect.right) ;
		ASSERT_EQUALS_V(100, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_advanced_rect)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 100, 100) ;
		CRect advanced_rect(10, 10, 20, 20) ;
		dialog.resize_advanced_rect(rect, advanced_rect) ;

		ASSERT_EQUALS_V(10, rect.left) ;
		ASSERT_EQUALS_V(90, rect.top) ;
		ASSERT_EQUALS_V(20, rect.right) ;
		ASSERT_EQUALS_V(100, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_register_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.resize_register_button(rect) ;

		ASSERT_EQUALS_V(10, rect.left) ;
		ASSERT_EQUALS_V(10, rect.top) ;
		ASSERT_EQUALS_V(10, rect.right) ;
		ASSERT_EQUALS_V(10, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_close_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.resize_close_button(rect) ;

		ASSERT_EQUALS_V(10, rect.left) ;
		ASSERT_EQUALS_V(10, rect.top) ;
		ASSERT_EQUALS_V(10, rect.right) ;
		ASSERT_EQUALS_V(10, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_advanced_button)
	{
		CRegisterGlossDlg dialog ;
		CRect rect(10, 10, 10, 10) ;
		dialog.resize_advanced_button(rect) ;

		ASSERT_EQUALS_V(10, rect.left) ;
		ASSERT_EQUALS_V(10, rect.top) ;
		ASSERT_EQUALS_V(10, rect.right) ;
		ASSERT_EQUALS_V(10, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_client_rect_close)
	{
		CRegisterGlossDlg dialog ;
		dialog.m_CloseBtn.m_window_rect.top = 10 ;
		dialog.m_CloseBtn.m_window_rect.bottom = 20 ;

		CRect rect(10, 10, 100, 100) ;
		dialog.resize_client_rect_close(rect, 5) ;

		ASSERT_EQUALS_V(10, rect.left) ;
		ASSERT_EQUALS_V(10, rect.top) ;
		ASSERT_EQUALS_V(100, rect.right) ;
		ASSERT_EQUALS_V(85, rect.bottom) ;
	}
	TEST( TestCRegisterGlossDlg, resize_client_rect_advanced)
	{
		CRegisterGlossDlg dialog ;

		dialog.m_AdvancedBtn.m_window_rect.top = 5 ;
		dialog.m_AdvancedBtn.m_window_rect.bottom = 10 ;

		CRect rect(10, 10, 100, 100) ;
		dialog.resize_client_rect_advanced(rect, 5) ;

		ASSERT_EQUALS_V(10, rect.left) ;
		ASSERT_EQUALS_V(10, rect.top) ;
		ASSERT_EQUALS_V(100, rect.right) ;
		ASSERT_EQUALS_V(90, rect.bottom) ;
	}
}

#endif
#endif