#include "stdafx.h"
#include "ConcordanceDialog.h"

#include <boost/test/unit_test.hpp>

#ifdef UNIT_TEST

BOOST_AUTO_TEST_SUITE( TestCConcordanceDialog )

	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CConcordanceDialog dialog ;
		BOOST_CHECK_EQUAL(IDD_CONCORDANCE_DIALOG, CConcordanceDialog::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( init_get_text)
	{
		CConcordanceDialog dialog ;
		string expected("") ;
		string actual(string2string(dialog.get_text()).c_str()) ;
		BOOST_CHECK_EQUAL(expected, actual) ;
	}
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CConcordanceDialog dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(! result) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif
