#include "stdafx.h"
#include "keyboard_shortcuts.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( KeyboardShortcutsTest )


BOOST_AUTO_TEST_CASE(test_load_simple)
{
	shortcuts::KeyboardShortcuts shortcuts ;
	string text = "<PowerPoint><shortcut><type>CTRL</type></shortcut></PowerPoint>" ;
	shortcuts.load(text.c_str(), "PowerPoint") ;
	BOOST_CHECK_EQUAL(1u, shortcuts.m_ctrl_shortcuts.size()) ;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( KeyboardShortcutTest )


BOOST_AUTO_TEST_CASE(test_default)
{
	shortcuts::KeyboardShortcut shortcut(shortcuts::SHORTCUT_CTRL, L'K', 0) ;
	BOOST_CHECK_EQUAL(shortcut.m_type, shortcuts::SHORTCUT_CTRL) ;
	BOOST_CHECK_EQUAL(shortcut.m_firstkey, L'K') ;
	BOOST_CHECK_EQUAL(shortcut.m_secondkey, 0) ;
}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
