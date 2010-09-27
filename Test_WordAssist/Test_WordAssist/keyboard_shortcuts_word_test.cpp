#include "stdafx.h"
#include "keyboard_shortcuts_word.h"
#include "keyboard_shortcuts.h"
#include "key_mapper_word.h"
#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( KeyboardShortcutsWordTest )

	using namespace shortcuts ;

	BOOST_AUTO_TEST_CASE(test_get_default_file_text)
	{
		const string text = get_default_file_text() ;
		KeyboardShortcuts keys(NULL) ;
		keys.load(text) ;

		BOOST_CHECK_EQUAL(23u, keys.m_alt_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(2u, keys.m_ctrl_alt_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(1u, keys.m_ctrl_shortcuts.size()) ;

		KeyboardShortcuts::shortcut_map_ptr cut = keys.m_alt_shortcuts[L'0'] ;
		BOOST_CHECK_EQUAL(CmdEntry0Action, (*cut)[0]->m_command) ;
		cut = keys.m_ctrl_shortcuts[VK_RIGHT] ;
		BOOST_CHECK_EQUAL(CmdExtendLookupAction, (*cut)[0]->m_command) ;
		cut = keys.m_ctrl_alt_shortcuts[VK_F9] ;
		BOOST_CHECK_EQUAL("ToggleShortcuts", (*cut)[0]->m_command) ;

	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
