#include "stdafx.h"
#include "keyboard_shortcuts.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( KeyboardShortcutsTest )

	BOOST_AUTO_TEST_CASE(test_load_simple)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		string text = "<shortcuts>"
			"<shortcut>"
			"<type>CTRL</type>"
			"<first>k</first>"
			"<second />"
			"<command>GetAndNext</command>"
			"</shortcut>"
			"</shortcuts>" ;
		keyboard_shortcuts.load(text) ;
		BOOST_CHECK_EQUAL(1u, keyboard_shortcuts.m_ctrl_shortcuts.size()) ;
		shortcuts::shortcut_ptr keyboard_shortcut = keyboard_shortcuts.m_ctrl_shortcuts[L'k'] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_CTRL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'k') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
	}
	BOOST_AUTO_TEST_CASE(test_load_alt)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		string text = "<shortcuts>"
			"<shortcut>"
			"<type>alt</type>"
			"<first>k</first>"
			"<second />"
			"<command>GetAndNext</command>"
			"</shortcut>"
			"</shortcuts>";
		keyboard_shortcuts.load(text) ;
		BOOST_CHECK_EQUAL(1u, keyboard_shortcuts.m_alt_shortcuts.size()) ;
		shortcuts::shortcut_ptr keyboard_shortcut = keyboard_shortcuts.m_alt_shortcuts[L'k'] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_ALT) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'k') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
	}
	BOOST_AUTO_TEST_CASE(test_load_ctrl_alt)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		string text = "<shortcuts>"
			"<shortcut>"
			"<type>CTRL+ALT</type>"
			"<first>k</first>"
			"<second />"
			"<command>GetAndNext</command>"
			"</shortcut>"
			"</shortcuts>" ;
		keyboard_shortcuts.load(text) ;
		BOOST_CHECK_EQUAL(1u, keyboard_shortcuts.m_ctrl_alt_shortcuts.size()) ;
		shortcuts::shortcut_ptr keyboard_shortcut = keyboard_shortcuts.m_ctrl_alt_shortcuts[L'k'] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_CTRL_ALT) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'k') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
	}

	BOOST_AUTO_TEST_CASE(test_load_two)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		string text = "<?xml version=\"1.0\"?>"
			"<shortcuts>"
			"<shortcut>"
			"<type>CTRL</type>"
			"<first>k</first>"
			"<second />"
			"<command>GetAndNext</command>"
			"</shortcut>"
			"<shortcut>"
			"<type>CTRL</type>"
			"<first>p</first>"
			"<second />"
			"<command>SetAndNext</command>"
			"</shortcut>"
			"</shortcuts>" ;
		keyboard_shortcuts.load(text) ;
		BOOST_CHECK_EQUAL(2u, keyboard_shortcuts.m_ctrl_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.m_ctrl_shortcuts[L'k']->m_firstkey, L'k') ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.m_ctrl_shortcuts[L'p']->m_firstkey, L'p') ;
	}

	BOOST_AUTO_TEST_CASE(test_load_secondkey)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		string text = "<shortcuts>"
			"<shortcut>"
			"<type>CTRL</type>"
			"<first>k</first>"
			"<second>m</second>"
			"<command>GetAndNext</command>"
			"</shortcut>"
			"</shortcuts>" ;
		keyboard_shortcuts.load(text) ;
		BOOST_CHECK_EQUAL(1u, keyboard_shortcuts.m_ctrl_shortcuts.size()) ;
		shortcuts::shortcut_ptr keyboard_shortcut = keyboard_shortcuts.m_ctrl_shortcuts[L'k'] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_CTRL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'k') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, L'm') ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( KeyboardShortcutTest )

	BOOST_AUTO_TEST_CASE(test_default)
	{
		string command("GetAndNext") ;
		shortcuts::KeyboardShortcut shortcut(shortcuts::SHORTCUT_CTRL, L'K', 0, command) ;
		BOOST_CHECK_EQUAL(shortcut.m_type, shortcuts::SHORTCUT_CTRL) ;
		BOOST_CHECK_EQUAL(shortcut.m_firstkey, L'K') ;
		BOOST_CHECK_EQUAL(shortcut.m_secondkey, 0) ;
		BOOST_CHECK_EQUAL(shortcut.m_command, command) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_get_shortcut_key )

	BOOST_AUTO_TEST_CASE(test_a)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("a"), L'a') ;
	}
	BOOST_AUTO_TEST_CASE(test_A)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("A"), L'a') ;
	}
	BOOST_AUTO_TEST_CASE(test_1)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("1"), L'1') ;
	}
	BOOST_AUTO_TEST_CASE(test_vk_right)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("VK_RIGHT"), VK_RIGHT) ;
	}
	BOOST_AUTO_TEST_CASE(test_vk_delete)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("vk_delete"), VK_DELETE) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_get_shortcut_type )

	BOOST_AUTO_TEST_CASE(test_ctrl)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_shortcut_type("ctrl"), shortcuts::SHORTCUT_CTRL) ;
	}
	BOOST_AUTO_TEST_CASE(test_alt)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_shortcut_type("alt"), shortcuts::SHORTCUT_ALT) ;
	}
	BOOST_AUTO_TEST_CASE(test_ctrl_alt)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_shortcut_type("ctrl+alt"), shortcuts::SHORTCUT_CTRL_ALT) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
