#include "stdafx.h"
#include "gloss_win_collection.h"

#ifdef UNIT_TEST
#include "GlossaryDlgListenerFake.h"
#include <boost/test/unit_test.hpp>

gloss_window_pointer make_gloss_window(app_props::props_ptr props, BOOL is_window)
{
	GlossWinCollection collection ;
	gloss_window_pointer gloss_window(new GlossaryWindowFrame(props)) ;
	WindowWrapperFake *fake_window = new WindowWrapperFake ;
	window_wrapper_ptr window(fake_window) ;
	fake_window->m_is_window = is_window ;
	gloss_window->m_get_window = boost::bind(&get_window_fake, window, _1) ;
	return gloss_window ;
}

BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionPreTranslate )

	BOOL not_translated(MSG*)
	{
		return FALSE ;
	}
	BOOL translated(MSG*)
	{
		return TRUE ;
	}

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection collection ;
		collection.remove_destroyed_gloss_windows() ;
		MSG msg ;
		BOOST_CHECK(! collection.pre_translate(&msg)) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_false)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		collection.m_glossary_windows.push_back(gloss_window) ;
		gloss_window->m_pre_translate_msg = boost::bind(&not_translated, _1) ;
		MSG msg ;
		BOOST_CHECK(! collection.pre_translate(&msg)) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_true)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		collection.m_glossary_windows.push_back(gloss_window) ;
		gloss_window->m_pre_translate_msg = boost::bind(&translated, _1) ;
		MSG msg ;
		BOOST_CHECK(collection.pre_translate(&msg)) ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_true)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window_false = make_gloss_window(props, TRUE) ;
		gloss_window_pointer gloss_window_true = make_gloss_window(props, TRUE) ;
		collection.m_glossary_windows.push_back(gloss_window_false) ;
		collection.m_glossary_windows.push_back(gloss_window_true) ;
		gloss_window_true->m_pre_translate_msg = boost::bind(&translated, _1) ;
		gloss_window_false->m_pre_translate_msg = boost::bind(&not_translated, _1) ;
		MSG msg ;
		BOOST_CHECK(collection.pre_translate(&msg)) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionRemoveDestroyed)

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection collection ;
		collection.remove_destroyed_gloss_windows() ;
		BOOST_CHECK(collection.m_glossary_windows.empty()) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_is_window)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;

		gloss_window_list gloss_windows ;
		collection.m_glossary_windows.push_back(gloss_window) ;
		collection.remove_destroyed_gloss_windows() ;
		BOOST_CHECK_EQUAL(1u, collection.m_glossary_windows.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_not_window)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, FALSE) ;

		collection.m_glossary_windows.push_back(gloss_window) ;
		BOOST_CHECK_EQUAL(1u, collection.m_glossary_windows.size()) ;
		collection.remove_destroyed_gloss_windows() ;
		BOOST_CHECK_EQUAL(0u, collection.m_glossary_windows.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_is_window)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window1 = make_gloss_window(props, FALSE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;

		collection.m_glossary_windows.push_back(gloss_window1) ;
		collection.m_glossary_windows.push_back(gloss_window2) ;
		BOOST_CHECK_EQUAL(2u, collection.m_glossary_windows.size()) ;
		collection.remove_destroyed_gloss_windows() ;
		BOOST_CHECK_EQUAL(1u, collection.m_glossary_windows.size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif