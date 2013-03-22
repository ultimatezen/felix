#include "stdafx.h"
#include "EditTransRecordDialogRegGloss.h"

#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCEditTransRecordDialogRegGloss )


	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditTransRecordDialogRegGloss dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_RECORD_GLOSS, CEditTransRecordDialogRegGloss::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( is_gloss_mem)
	{
		CEditTransRecordDialogRegGloss dialog ;
		BOOST_CHECK(! dialog.m_is_glossary) ;
	}

	// CEditTransRecordDialogRegGloss message processing
	BOOST_AUTO_TEST_CASE( test_message_ZERO)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = 1 ;
		BOOL result = dialog.ProcessWindowMessage(NULL, WM_COMMAND, 0, 0, lResult, 0)  ;
		BOOST_CHECK_EQUAL(0u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK(result) ;
		BOOST_CHECK_EQUAL(0, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif