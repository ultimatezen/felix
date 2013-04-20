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

BOOST_AUTO_TEST_SUITE_END()

#endif