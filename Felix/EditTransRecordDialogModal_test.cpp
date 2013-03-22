#include "stdafx.h"
#include "EditTransRecordDialogModal.h"


#include <boost/test/unit_test.hpp>
#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCEditTransRecordDialogModal )


	BOOST_AUTO_TEST_CASE( instantiate)
	{
		CEditTransRecordDialogModal dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_RECORD, CEditTransRecordDialogModal::IDD) ;
	}
	BOOST_AUTO_TEST_CASE( is_gloss_mem)
	{
		CEditTransRecordDialogModal dialog ;
		BOOST_CHECK(! dialog.m_is_glossary) ;
	}

	// CEditTransRecordDialogModal message processing

BOOST_AUTO_TEST_SUITE_END()
#endif
