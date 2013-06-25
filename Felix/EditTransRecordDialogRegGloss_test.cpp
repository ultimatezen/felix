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
	BOOST_AUTO_TEST_CASE(test_OnInitDialog)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = dialog.OnInitDialog() ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnInitDialog", dialog.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(1, lResult) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnCancel)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = dialog.OnCancel() ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnCancel", dialog.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(0, lResult) ;
	}
	BOOST_AUTO_TEST_CASE(test_OnOK)
	{
		CEditTransRecordDialogRegGloss dialog ;
		LRESULT lResult = dialog.OnOK() ;
		BOOST_CHECK_EQUAL(1u, dialog.m_sensing_variable.size()) ;
		BOOST_CHECK_EQUAL("OnOK", dialog.m_sensing_variable[0]) ;
		BOOST_CHECK_EQUAL(0, lResult) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif