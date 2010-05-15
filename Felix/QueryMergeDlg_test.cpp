#include "StdAfx.h"
#include "QueryMergeDlg.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCQueryMergeDlg )


	BOOST_AUTO_TEST_CASE( instantiate_mem_merge_msg)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;

		string expected = "Memory already open: [c:\\foo.xml]\rDo you want to merge?" ;
		string actual = CT2A(dialog.m_merge_message) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( instantiate_mem_title)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;

		string expected = "Merge Memories" ;
		string actual = CT2A(dialog.m_title) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		CString m_title ;
	}
	BOOST_AUTO_TEST_CASE( instantiate_gloss_merge_msg)
	{
		CQueryMergeDlg dialog(IDS_MERGE_GLOSS_TITLE, 
			IDS_MERGE_GLOSS_TEXT, 
			_T("c:\\foo.xml")) ;

		string expected = "Glossary already open: [c:\\foo.xml]\rDo you want to merge?" ;
		string actual = CT2A(dialog.m_merge_message) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( instantiate_gloss_title)
	{
		CQueryMergeDlg dialog(IDS_MERGE_GLOSS_TITLE, 
			IDS_MERGE_GLOSS_TEXT, 
			_T("c:\\foo.xml")) ;

		string expected = "Merge Glossaries" ;
		string actual = CT2A(dialog.m_title) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
		CString m_title ;
	}

	// CExportDialog message processing
	BOOST_AUTO_TEST_CASE( test_message_WM_INITDIALOG)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog"); 
		BOOST_CHECK_EQUAL(1, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDOK)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDOK, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCloseCmd"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDCANCEL)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_bModal = TRUE ;
		LRESULT lResult = 1 ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, IDCANCEL, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnCloseCmd"); 
		BOOST_CHECK_EQUAL( 0, (int)lResult) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(FALSE, result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}


	BOOST_AUTO_TEST_CASE( test_message_IDC_MERGE)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_iMerge = 0 ;
		LRESULT lResult = 1 ;
		LPARAM lparam =  MAKELPARAM(IDC_MERGE, 0) ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, lparam, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(IDC_MERGE, (int)dialog.m_iMerge) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_IDC_SEPARATE)
	{
		CQueryMergeDlg dialog(IDS_MERGE_MEM_TITLE, 
			IDS_MERGE_MEM_TEXT, 
			_T("c:\\foo.xml")) ;
		dialog.m_iMerge = 0 ;
		LRESULT lResult = 1 ;
		LPARAM lparam =  MAKELPARAM(IDC_SEPARATE, 0) ;
		dialog.ProcessWindowMessage(NULL, WM_COMMAND, lparam, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(IDC_SEPARATE, (int)dialog.m_iMerge) ;
	}
BOOST_AUTO_TEST_SUITE_END()
#endif