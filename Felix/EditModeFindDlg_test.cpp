#include "stdafx.h"
#include "EditModeFindDlg.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( test_emode_search_params )

	BOOST_AUTO_TEST_CASE(test_instantiate)
	{
		emode_search_params parameters ;
		BOOST_CHECK_EQUAL(parameters.m_ignore_case, false) ;
		BOOST_CHECK_EQUAL(parameters.m_whole_words, false) ;
		BOOST_CHECK_EQUAL(parameters.m_search_type, IDS_SEARCH_ALL) ;
		BOOST_CHECK_EQUAL(parameters.m_search_string, L"") ;
		BOOST_CHECK_EQUAL(parameters.m_replace_string, L"") ;

	}
	BOOST_AUTO_TEST_CASE(test_copy_constructor)
	{
		emode_search_params parameters ;
		parameters.m_ignore_case = true ;
		parameters.m_whole_words = true ;
		parameters.m_search_type = IDS_SEARCH_SOURCE ;
		parameters.m_search_string = L"search" ;
		parameters.m_replace_string = L"replace" ;

		emode_search_params parameters_copy(parameters) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_ignore_case, true) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_whole_words, true) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_search_type, IDS_SEARCH_SOURCE) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_search_string, L"search") ;
		BOOST_CHECK_EQUAL(parameters_copy.m_replace_string, L"replace") ;
	}

	BOOST_AUTO_TEST_CASE(test_assignment_operator)
	{
		emode_search_params parameters ;
		parameters.m_ignore_case = true ;
		parameters.m_whole_words = true ;
		parameters.m_search_type = IDS_SEARCH_SOURCE ;
		parameters.m_search_string = L"search" ;
		parameters.m_replace_string = L"replace" ;

		emode_search_params parameters_copy ;
		parameters_copy = parameters ;
		BOOST_CHECK_EQUAL(parameters_copy.m_ignore_case, true) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_whole_words, true) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_search_type, IDS_SEARCH_SOURCE) ;
		BOOST_CHECK_EQUAL(parameters_copy.m_search_string, L"search") ;
		BOOST_CHECK_EQUAL(parameters_copy.m_replace_string, L"replace") ;
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( TestCEditModeFindDlg )

	BOOST_AUTO_TEST_CASE(test_instantiate)
	{
		CEditModeFindDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_FIND_DIALOG, CEditModeFindDlg::IDD) ;
	}

	BOOST_AUTO_TEST_CASE(test_OnInitDialog)
	{
		CEditModeFindDlg dialog ;
		dialog.OnInitDialog() ; 
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog") ;
	}

	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditModeFindDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

// CEditModeReplaceDlg message processing
BOOST_AUTO_TEST_SUITE( TestCEditModeReplaceDlg )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditModeReplaceDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_MODE_REPLACE_DIALOG, CEditModeReplaceDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnInitDialog)
	{
		CEditModeReplaceDlg dialog ;
		dialog.OnInitDialog() ; 
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL(dialog.m_sensing_variable[0], "OnInitDialog") ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditModeReplaceDlg dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif