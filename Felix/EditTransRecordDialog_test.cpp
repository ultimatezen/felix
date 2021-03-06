#include "stdafx.h"
#include "EditTransRecordDialog.h"

#include <boost/test/unit_test.hpp>
#include "window_listener.h"

#ifdef UNIT_TEST
BOOST_AUTO_TEST_SUITE( TestCEditTransRecordDialog )

	BOOST_AUTO_TEST_CASE(test_instantiate)
	{
		CEditTransRecordDialog dialog ;
		BOOST_CHECK_EQUAL(IDD_EDIT_RECORD, CEditTransRecordDialog::IDD) ;
	}
	BOOST_AUTO_TEST_CASE(test_is_gloss_mem)
	{
		CEditTransRecordDialog dialog ;
		BOOST_CHECK(! dialog.m_is_glossary) ;
	}
	BOOST_AUTO_TEST_CASE(test_set_display_state)
	{
		CEditTransRecordDialog dialog ;
		BOOST_CHECK_EQUAL(dialog.m_display_state, 0) ;
		dialog.set_display_state(WindowListener::NEW_RECORD_DISPLAY_STATE) ;
		BOOST_CHECK_EQUAL(dialog.m_display_state, WindowListener::NEW_RECORD_DISPLAY_STATE) ;
	}
	BOOST_AUTO_TEST_CASE( fill_from_old_record)
	{
		CEditTransRecordDialog dialog ;
		dialog.m_new_record->set_id(10) ;
		dialog.m_new_record->set_refcount(10) ;
		dialog.m_old_record->set_id(5) ;
		dialog.m_old_record->set_refcount(5) ;
		dialog.fill_from_old_record() ;
		BOOST_CHECK_EQUAL((int)dialog.m_new_record->get_id(), (int)dialog.m_old_record->get_id()) ;
		BOOST_CHECK_EQUAL((int)dialog.m_new_record->get_refcount(), (int)dialog.m_old_record->get_refcount()) ;
	}

	// CEditTransRecordDialog message processing

BOOST_AUTO_TEST_SUITE_END()

#endif