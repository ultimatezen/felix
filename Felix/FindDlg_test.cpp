#include "stdafx.h"
#include "FindDlg.h"

#ifdef UNIT_TEST

#include "string_logger.h"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE( TestCFindDlg, StringLoggerTester )

	BOOST_AUTO_TEST_CASE(test_instantiate)
	{
		CFindDlg dialog ;
		BOOST_CHECK_EQUAL(IDD_FIND_DLG, CFindDlg::IDD) ;
	}
	BOOST_AUTO_TEST_CASE(test_get_params)
	{
		CFindDlg dialog ;
		BOOST_CHECK_EQUAL(dialog.get_search_params().get_source_plain(), L"") ;
	}
	BOOST_AUTO_TEST_CASE(TestOnDestroy)
	{
		logging::set_logging_level(LOGGING_VERBOSE);
		CFindDlg dialog ;
		BOOST_CHECK_EQUAL(dialog.OnDestroy(), 0) ;
		BOOST_CHECK(has_text("Destroying the find dialog window")) ;
	}
	BOOST_AUTO_TEST_CASE(TestOnInitDialog)
	{
		logging::set_logging_level(LOGGING_VERBOSE);
		CFindDlg dialog ;
		BOOST_CHECK_EQUAL(dialog.OnInitDialog(), 0) ;
		BOOST_CHECK(has_text("Initializing the find dialog")) ;
	}

	BOOST_AUTO_TEST_CASE(Test_WM_DESTROY)
	{
		logging::set_logging_level(LOGGING_VERBOSE);
		CFindDlg dialog ;
		LRESULT result = 11 ;
		BOOST_CHECK(! dialog.ProcessWindowMessage(NULL, WM_DESTROY, 0, 0, result)) ;
		BOOST_CHECK_EQUAL(result, 0) ;
		BOOST_CHECK(has_text("Destroying the find dialog window")) ;
	}
	BOOST_AUTO_TEST_CASE(Test_WM_INITDIALOG)
	{
		logging::set_logging_level(LOGGING_VERBOSE);
		CFindDlg dialog ;
		LRESULT result = 11 ;
		BOOST_CHECK(dialog.ProcessWindowMessage(NULL, WM_INITDIALOG, 0, 0, result)) ;
		BOOST_CHECK_EQUAL(result, 0) ;
		BOOST_CHECK(has_text("Initializing the find dialog")) ;
	}
	BOOST_AUTO_TEST_CASE(test_nonexistent_message)
	{
		logging::set_logging_level(LOGGING_VERBOSE);
		CFindDlg dialog ;
		LRESULT result = 11 ;
		BOOST_CHECK(! dialog.ProcessWindowMessage(NULL, 0, 0, 0, result)) ;
		BOOST_CHECK_EQUAL(result, 11) ;
		BOOST_CHECK(! has_text("Initializing the find dialog")) ;
	}


BOOST_AUTO_TEST_SUITE_END()

#endif
