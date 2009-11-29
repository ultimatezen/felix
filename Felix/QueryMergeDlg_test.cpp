#include "StdAfx.h"
#include "QueryMergeDlg.h"

#include "easyunit/testharness.h"
#ifdef _DEBUG

#ifdef UNIT_TEST
namespace easyunit
{
	TEST( TestCQueryMergeDlg, instantiate_mem_merge_msg)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;

		SimpleString expected = "Memory already open: [c:\\foo.xml]\rDo you want to merge?" ;
		SimpleString actual = CT2A(dialog.m_merge_message) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCQueryMergeDlg, instantiate_mem_title)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;

		SimpleString expected = "Merge Memories" ;
		SimpleString actual = CT2A(dialog.m_title) ;
		ASSERT_EQUALS_V(expected, actual) ;
		CString m_title ;
	}
	TEST( TestCQueryMergeDlg, instantiate_gloss_merge_msg)
	{
		CQueryMergeDlg dialog(IDS_MERGE_GLOSS_TITLE, 
			IDS_MERGE_GLOSS_TEXT, 
			_T("c:\\foo.xml")) ;

		SimpleString expected = "Glossary already open: [c:\\foo.xml]\rDo you want to merge?" ;
		SimpleString actual = CT2A(dialog.m_merge_message) ;
		ASSERT_EQUALS_V(expected, actual) ;
	}
	TEST( TestCQueryMergeDlg, instantiate_gloss_title)
	{
		CQueryMergeDlg dialog(IDS_MERGE_GLOSS_TITLE, 
			IDS_MERGE_GLOSS_TEXT, 
			_T("c:\\foo.xml")) ;

		SimpleString expected = "Merge Glossaries" ;
		SimpleString actual = CT2A(dialog.m_title) ;
		ASSERT_EQUALS_V(expected, actual) ;
		CString m_title ;
	}

	// CExportDialog message processing
	TEST( TestCQueryMergeDlg, test_message_WM_INITDIALOG)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnInitDialog"); 
		ASSERT_EQUALS_V( 1, (int)lResult) ;
	}
	TEST( TestCQueryMergeDlg, test_message_IDOK)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCmd"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCQueryMergeDlg, test_message_IDCANCEL)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(1, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V( SimpleString(dialog.m_sensing_variable[0].c_str()), "OnCloseCmd"); 
		ASSERT_EQUALS_V( 0, (int)lResult) ;
	}
	TEST( TestCQueryMergeDlg, test_message_ZERO)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(0, (int)dialog.m_sensing_variable.size()) ;
		ASSERT_EQUALS_V(0, (int)result) ;
		ASSERT_EQUALS_V(1, (int)lResult) ;
	}


	TEST( TestCQueryMergeDlg, test_message_IDC_MERGE)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_iMerge = 0 ;
		LRESULT lResult = 1 ;
		LPARAM lparam =  MAKELPARAM(IDC_MERGE, 0) ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, lparam, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(IDC_MERGE, (int)dialog.m_iMerge) ;
	}
	TEST( TestCQueryMergeDlg, test_message_IDC_SEPARATE)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_iMerge = 0 ;
		LRESULT lResult = 1 ;
		LPARAM lparam =  MAKELPARAM(IDC_SEPARATE, 0) ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, lparam, 0, lResult, 0)  ;
		ASSERT_EQUALS_V(IDC_SEPARATE, (int)dialog.m_iMerge) ;
	}
}

#endif
#endif