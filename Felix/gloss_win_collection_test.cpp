#include "stdafx.h"
#include "gloss_win_collection.h"
#include "record_local.h"

#ifdef UNIT_TEST

#include "GlossaryDlgListenerFake.h"
#include "mem_tests.h"
#include <boost/test/unit_test.hpp>

using namespace mem_engine ;

gloss_window_pointer make_gloss_window_with_fake(app_props::props_ptr props, window_wrapper_ptr window) ;
gloss_window_pointer make_gloss_window(app_props::props_ptr props, BOOL is_window) ;

gloss_window_pointer make_gloss_window_with_fake(app_props::props_ptr props, window_wrapper_ptr window)
{
	gloss_window_pointer gloss_window(new GlossaryWindowFrame(props)) ;
	gloss_window->m_get_window = boost::bind(&get_window_fake, window, _1) ;
	return gloss_window ;
}
gloss_window_pointer make_gloss_window(app_props::props_ptr props, BOOL is_window)
{
	WindowWrapperFake *fake_window = new WindowWrapperFake ;
	window_wrapper_ptr window(fake_window) ;
	fake_window->m_is_window = is_window ;
	return make_gloss_window_with_fake(props, window) ;
}

BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionAdd )

	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss = windows.add(props) ;
		BOOST_CHECK(gloss->is_main()) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss1 = windows.add(props) ;
		gloss_window_pointer gloss2 = windows.add(props) ;
		BOOST_CHECK(gloss1->is_main()) ;
		BOOST_CHECK(! gloss2->is_main()) ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_removed)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss1 = windows.add(props) ;
		BOOST_CHECK(gloss1->is_main()) ;
		windows.m_glossary_windows.clear() ;
		gloss_window_pointer gloss2 = windows.add(props) ;
		BOOST_CHECK(gloss2->is_main()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionCollectionFuncs )

	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss = windows.add(props) ;
		BOOST_CHECK(! windows.empty()) ;
		BOOST_CHECK_EQUAL(1u, windows.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss1 = windows.add(props) ;
		gloss_window_pointer gloss2 = windows.add(props) ;
		BOOST_CHECK(! windows.empty()) ;
		BOOST_CHECK_EQUAL(2u, windows.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_zero)
	{
		GlossWinCollection windows ;
		BOOST_CHECK(windows.empty()) ;
		BOOST_CHECK_EQUAL(0u, windows.size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

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

BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionAddRecord )

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection windows ;
		record_pointer record(new record_local) ;
		windows.add_record(record) ;
		BOOST_CHECK_EQUAL(0u, windows.m_glossary_windows.size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss1 = windows.add(props) ;
		record_pointer record(new record_local) ;
		record->set_source(L"source") ;
		record->set_trans(L"trans") ;
		windows.add_record(record) ;
		BOOST_CHECK_EQUAL(1u, windows.m_glossary_windows.size()) ;
		BOOST_CHECK_EQUAL(1u, gloss1->m_memories->get_first_memory()->size()) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection windows ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss1 = windows.add(props) ;
		gloss_window_pointer gloss2 = windows.add(props) ;

		record_pointer record1(new record_local) ;
		record1->set_source(L"source1") ;
		record1->set_trans(L"trans1") ;
		windows.add_record(record1) ;

		record_pointer record2(new record_local) ;
		record2->set_source(L"source2") ;
		record2->set_trans(L"trans2") ;
		windows.add_record(record2) ;

		BOOST_CHECK_EQUAL(2u, windows.m_glossary_windows.size()) ;
		BOOST_CHECK_EQUAL(2u, gloss1->m_memories->get_first_memory()->size()) ;
		BOOST_CHECK_EQUAL(0u, gloss2->m_memories->get_first_memory()->size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionGetGlossaryEntry )

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection collection ;
		BOOST_CHECK(collection.get_glossary_entry(1).empty()) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( TestGlossWinCollectionCreate )

	HWND create_true(HWND)
	{
		return (HWND)1000 ;
	}
	HWND create_false(HWND)
	{
		return NULL ;
	}

	BOOST_AUTO_TEST_CASE(test_true)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		gloss_window->m_create = boost::bind(&create_true, _1) ;
		BOOST_CHECK(collection.create(gloss_window, NULL)) ;
	}
	BOOST_AUTO_TEST_CASE(test_false)
	{
		GlossWinCollection collection ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		gloss_window->m_create = boost::bind(&create_false, _1) ;
		BOOST_CHECK(! collection.create(gloss_window, NULL)) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionSetUiLanguage)

	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.set_ui_language() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable[0], "set_ui_language") ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, FALSE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.set_ui_language() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable.size(), 0u) ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "set_ui_language") ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, TRUE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.set_ui_language() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable[0], "set_ui_language") ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "set_ui_language") ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionSavePrefs)

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection collection ;
		BOOST_CHECK_NO_THROW(collection.save_prefs()) ;
	}
	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = glosses.add(props) ; 

		glosses.save_prefs() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable[0], "save_prefs") ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window1 = glosses.add(props) ; 
		gloss_window_pointer gloss_window2 = glosses.add(props) ;

		glosses.save_prefs() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable[0], "save_prefs") ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable.size(), 0u) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionFirst)

	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window1 = glosses.add(props) ; 
		gloss_window_pointer gloss_window2 = glosses.add(props) ;

		BOOST_CHECK_EQUAL(2u, glosses.size()) ;
		BOOST_CHECK_EQUAL(gloss_window1, glosses.first()) ;
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionRemoveGlossRecord)

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		record_pointer rec1 = make_record("aaa", "bbb") ;
		BOOST_CHECK_NO_THROW(glosses.remove_gloss_record(rec1)) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = glosses.add(props) ; 

		memory_pointer mem(new memory_local(props)) ;
		gloss_window->get_memories().push_back(mem) ;
		record_pointer rec1 = make_record("aaa", "bbb") ;
		record_pointer rec2 = make_record("yyy", "zzz") ;
		gloss_window->add_record(mem, rec1) ;
		gloss_window->add_record(mem, rec2) ;
		BOOST_CHECK_EQUAL(2u, mem->size()) ;
		glosses.remove_gloss_record(rec1) ;
		BOOST_CHECK_EQUAL(1u, mem->size()) ;
	}

	BOOST_AUTO_TEST_CASE(test_remove_unrelated)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = glosses.add(props) ; 

		memory_pointer mem(new memory_local(props)) ;
		gloss_window->get_memories().push_back(mem) ;
		record_pointer rec1 = make_record("aaa", "bbb") ;
		record_pointer rec2 = make_record("yyy", "zzz") ;
		gloss_window->add_record(mem, rec1) ;
		gloss_window->add_record(mem, rec2) ;
		BOOST_CHECK_EQUAL(2u, mem->size()) ;
		record_pointer rec3 = make_record("111", "222") ;
		glosses.remove_gloss_record(rec3) ;
		BOOST_CHECK_EQUAL(2u, mem->size()) ;
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionGetCurrentMatches)

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		felix_query *null_query = NULL ;
		BOOST_CHECK_EQUAL(null_query, glosses.get_current_matches()) ;
	}
	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = glosses.add(props) ; 

		BOOST_CHECK_EQUAL(gloss_window->get_current_matches(), glosses.get_current_matches()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionGetShowCommand)

	BOOST_AUTO_TEST_CASE(test_visible)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK_EQUAL(SW_SHOWNOACTIVATE, glosses.get_show_command(TRUE)) ;
	}
	BOOST_AUTO_TEST_CASE(test_not_visible)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK_EQUAL(SW_HIDE, glosses.get_show_command(FALSE)) ;
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionPreShutdownSaveCheck)

	INT_PTR return_cancel()
	{
		return IDCANCEL ;
	}
	INT_PTR return_yes()
	{
		return IDYES ;
	}

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK(glosses.pre_shutdown_save_check()) ;
	}
	BOOST_AUTO_TEST_CASE(test_check_save_cancel)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = glosses.add(props) ; 

		memory_pointer mem(new memory_local(props)) ;
		gloss_window->get_memories().push_back(mem) ;
		record_pointer rec1 = make_record("aaa", "bbb") ;
		gloss_window->add_record(mem, rec1) ;

		gloss_window->m_check_save = &return_cancel ;

		BOOST_CHECK(! glosses.pre_shutdown_save_check()) ;
	}
	BOOST_AUTO_TEST_CASE(test_check_save_yes)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = glosses.add(props) ; 

		memory_pointer mem(new memory_local(props)) ;
		gloss_window->get_memories().push_back(mem) ;
		record_pointer rec1 = make_record("aaa", "bbb") ;
		gloss_window->add_record(mem, rec1) ;

		gloss_window->m_check_save = &return_yes ;

		BOOST_CHECK(glosses.pre_shutdown_save_check()) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionShowViewContent)

	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.show_view_content() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable[0], "show_view_content") ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, FALSE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.show_view_content() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable.size(), 0u) ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "show_view_content") ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionPutVisibility)

	BOOST_AUTO_TEST_CASE(test_show)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window = new WindowWrapperFake ;
		window_wrapper_ptr window(fake_window) ;
		gloss_window_pointer gloss_window = make_gloss_window_with_fake(props, window) ;
		glosses.m_glossary_windows.push_back(gloss_window) ;

		glosses.put_visibility(SW_SHOW) ;
		BOOST_CHECK_EQUAL(SW_SHOW, fake_window->m_show) ;
	}
	BOOST_AUTO_TEST_CASE(test_hide)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window = new WindowWrapperFake ;
		window_wrapper_ptr window(fake_window) ;
		gloss_window_pointer gloss_window = make_gloss_window_with_fake(props, window) ;
		glosses.m_glossary_windows.push_back(gloss_window) ;

		glosses.put_visibility(SW_HIDE) ;
		BOOST_CHECK_EQUAL(SW_HIDE, fake_window->m_show) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionGlossViewSwitch)

	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK(! glosses.gloss_view_switch(NULL)) ;
	}
	BOOST_AUTO_TEST_CASE(test_destroyed)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window = new WindowWrapperFake ;
		fake_window->m_is_window = FALSE ;
		window_wrapper_ptr window(fake_window) ;
		gloss_window_pointer gloss_window = make_gloss_window_with_fake(props, window) ;
		gloss_window->m_hWnd = (HWND)0x1000 ;
		glosses.m_glossary_windows.push_back(gloss_window) ;

		BOOST_CHECK(! glosses.gloss_view_switch((HWND)0x1000)) ;
		gloss_window->m_hWnd = NULL ;
	}
	BOOST_AUTO_TEST_CASE(test_has_no_next)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window1 = new WindowWrapperFake ;
		fake_window1->m_is_window = TRUE ;
		window_wrapper_ptr window1(fake_window1) ;
		gloss_window_pointer gloss_window1 = make_gloss_window_with_fake(props, window1) ;
		gloss_window1->m_hWnd = (HWND)0x1000 ;
		glosses.m_glossary_windows.push_back(gloss_window1) ;

		BOOST_CHECK(! glosses.gloss_view_switch((HWND)0x1000)) ;
		BOOST_CHECK(! fake_window1->m_set_focus) ;
		gloss_window1->m_hWnd = NULL ;
	}
	BOOST_AUTO_TEST_CASE(test_has_next)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window1 = new WindowWrapperFake ;
		fake_window1->m_is_window = TRUE ;
		window_wrapper_ptr window1(fake_window1) ;
		gloss_window_pointer gloss_window1 = make_gloss_window_with_fake(props, window1) ;
		gloss_window1->m_hWnd = (HWND)0x1000 ;
		glosses.m_glossary_windows.push_back(gloss_window1) ;

		WindowWrapperFake *fake_window2 = new WindowWrapperFake ;
		fake_window2->m_is_window = TRUE ;
		window_wrapper_ptr window2(fake_window2) ;
		gloss_window_pointer gloss_window2 = make_gloss_window_with_fake(props, window2) ;
		gloss_window2->m_hWnd = (HWND)0x2000 ;
		glosses.m_glossary_windows.push_back(gloss_window2) ;

		BOOST_CHECK(glosses.gloss_view_switch((HWND)0x1000)) ;
		BOOST_CHECK(! fake_window1->m_set_focus) ;
		BOOST_CHECK(fake_window2->m_set_focus) ;
		gloss_window1->m_hWnd = NULL ;
		gloss_window2->m_hWnd = NULL ;
	}

	BOOST_AUTO_TEST_CASE(test_has_previous)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window1 = new WindowWrapperFake ;
		fake_window1->m_is_window = TRUE ;
		window_wrapper_ptr window1(fake_window1) ;
		gloss_window_pointer gloss_window1 = make_gloss_window_with_fake(props, window1) ;
		gloss_window1->m_hWnd = (HWND)0x1000 ;
		glosses.m_glossary_windows.push_back(gloss_window1) ;

		WindowWrapperFake *fake_window2 = new WindowWrapperFake ;
		fake_window2->m_is_window = TRUE ;
		window_wrapper_ptr window2(fake_window2) ;
		gloss_window_pointer gloss_window2 = make_gloss_window_with_fake(props, window2) ;
		gloss_window2->m_hWnd = (HWND)0x2000 ;
		glosses.m_glossary_windows.push_back(gloss_window2) ;

		BOOST_CHECK(glosses.gloss_view_switch((HWND)0x2000)) ;
		BOOST_CHECK(fake_window1->m_set_focus) ;
		BOOST_CHECK(! fake_window2->m_set_focus) ;
		gloss_window1->m_hWnd = NULL ;
		gloss_window2->m_hWnd = NULL ;
	}
	BOOST_AUTO_TEST_CASE(test_unrelated_hwnd)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		WindowWrapperFake *fake_window1 = new WindowWrapperFake ;
		fake_window1->m_is_window = TRUE ;
		window_wrapper_ptr window1(fake_window1) ;
		gloss_window_pointer gloss_window1 = make_gloss_window_with_fake(props, window1) ;
		gloss_window1->m_hWnd = (HWND)0x1000 ;
		glosses.m_glossary_windows.push_back(gloss_window1) ;

		WindowWrapperFake *fake_window2 = new WindowWrapperFake ;
		fake_window2->m_is_window = TRUE ;
		window_wrapper_ptr window2(fake_window2) ;
		gloss_window_pointer gloss_window2 = make_gloss_window_with_fake(props, window2) ;
		gloss_window2->m_hWnd = (HWND)0x2000 ;
		glosses.m_glossary_windows.push_back(gloss_window2) ;

		BOOST_CHECK(! glosses.gloss_view_switch((HWND)0x3000)) ;
		BOOST_CHECK(! fake_window1->m_set_focus) ;
		BOOST_CHECK(! fake_window2->m_set_focus) ;
		gloss_window1->m_hWnd = NULL ;
		gloss_window2->m_hWnd = NULL ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionOnFileSave)
	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK_NO_THROW(glosses.on_file_save()) ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, FALSE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.on_file_save() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable.size(), 0u) ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.on_file_save() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable[0], "on_file_save") ;
		BOOST_CHECK_EQUAL(glosses.size(), 1u) ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, FALSE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.on_file_save() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable.size(), 0u) ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "on_file_save") ;
		BOOST_CHECK_EQUAL(glosses.size(), 1u) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, TRUE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.on_file_save() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable[0], "on_file_save") ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "on_file_save") ;
		BOOST_CHECK_EQUAL(glosses.size(), 2u) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionExitSilently)
	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK_NO_THROW(glosses.exit_silently()) ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, FALSE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.exit_silently() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable.size(), 0u) ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.exit_silently() ;
		BOOST_CHECK_EQUAL(gloss_window->m_sensing_variable[0], "exit_silently") ;
		BOOST_CHECK_EQUAL(glosses.size(), 1u) ;
	}
	BOOST_AUTO_TEST_CASE(test_two_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, FALSE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.exit_silently() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable.size(), 0u) ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "exit_silently") ;
		BOOST_CHECK_EQUAL(glosses.size(), 1u) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, TRUE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.exit_silently() ;
		BOOST_CHECK_EQUAL(gloss_window1->m_sensing_variable[0], "exit_silently") ;
		BOOST_CHECK_EQUAL(gloss_window2->m_sensing_variable[0], "exit_silently") ;
		BOOST_CHECK_EQUAL(glosses.size(), 2u) ;
	}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestGlossWinCollectionDestroyAll)
	BOOST_AUTO_TEST_CASE(test_empty)
	{
		GlossWinCollection glosses ;
		BOOST_CHECK_NO_THROW(glosses.destroy_all()) ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_one_not_window)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, FALSE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.destroy_all() ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_one)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;
		gloss_window_pointer gloss_window = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window);

		glosses.destroy_all() ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
	BOOST_AUTO_TEST_CASE(test_two)
	{
		GlossWinCollection glosses ;
		app_props::props_ptr props(new app_props::properties) ;

		gloss_window_pointer gloss_window1 = make_gloss_window(props, TRUE) ;
		gloss_window_pointer gloss_window2 = make_gloss_window(props, TRUE) ;
		glosses.m_glossary_windows.push_back(gloss_window1);
		glosses.m_glossary_windows.push_back(gloss_window2);

		glosses.destroy_all() ;
		BOOST_CHECK_EQUAL(glosses.size(), 0u) ;
	}
BOOST_AUTO_TEST_SUITE_END()
#endif
