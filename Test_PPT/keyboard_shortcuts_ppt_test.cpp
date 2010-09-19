#include "stdafx.h"
#include "keyboard_shortcuts_ppt.h"
#include "keyboard_shortcuts.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( KeyboardShortcutsPptTest )

	using namespace shortcuts;

	BOOST_AUTO_TEST_CASE(test_get_default_file_text)
	{
		const string text = get_default_file_text() ;
		shortcuts::KeyboardShortcuts keys(NULL) ;
		keys.load(text) ;

		BOOST_CHECK_EQUAL(20u, keys.m_alt_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(6u, keys.m_ctrl_alt_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(1u, keys.m_ctrl_shortcuts.size()) ;

		KeyboardShortcuts::shortcut_map_ptr cut = keys.m_alt_shortcuts[L'5'] ;
		BOOST_CHECK_EQUAL("Entry 5", (*cut)[0]->m_command) ;
		cut = keys.m_ctrl_shortcuts[VK_RIGHT] ;
		BOOST_CHECK_EQUAL("Extend Lookup", (*cut)[0]->m_command) ;
		cut = keys.m_ctrl_alt_shortcuts[VK_UP] ;
		BOOST_CHECK_EQUAL("Correct Trans", (*cut)[0]->m_command) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
