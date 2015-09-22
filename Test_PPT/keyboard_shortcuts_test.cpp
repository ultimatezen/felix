#include "stdafx.h"
#include "keyboard_shortcuts.h"
#include "key_mapper_fake.h"

#ifdef UNIT_TEST

#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE( KeyboardShortcutsLoadTest )

	using namespace shortcuts ;

	BOOST_AUTO_TEST_CASE(test_load_simple)
	{
		KeyMapperFake mapper ;
		shortcuts::KeyboardShortcuts keyboard_shortcuts(&mapper) ;
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
		KeyboardShortcuts::shortcut_map_ptr cut = keyboard_shortcuts.m_ctrl_shortcuts[L'K'] ;
		shortcut_ptr keyboard_shortcut = (*cut)[0] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_CTRL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'K') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_command, "GetAndNext") ;
	}
	BOOST_AUTO_TEST_CASE(test_load_twice)
	{
		KeyMapperFake mapper ;
		shortcuts::KeyboardShortcuts keyboard_shortcuts(&mapper) ;
		string text = "<shortcuts>"
			"<shortcut>"
			"<type>CTRL</type>"
			"<first>k</first>"
			"<second />"
			"<command>GetAndNext</command>"
			"</shortcut>"
			"</shortcuts>" ;
		keyboard_shortcuts.load(text) ;
		text = "<shortcuts>"
			"<shortcut>"
			"<type>ALT</type>"
			"<first>k</first>"
			"<second />"
			"<command>DoSomeFoo</command>"
			"</shortcut>"
			"</shortcuts>" ;
		keyboard_shortcuts.load(text) ;
		BOOST_CHECK_EQUAL(0u, keyboard_shortcuts.m_ctrl_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(1u, keyboard_shortcuts.m_alt_shortcuts.size()) ;
		KeyboardShortcuts::shortcut_map_ptr cut = keyboard_shortcuts.m_alt_shortcuts[L'K'] ;
		shortcut_ptr keyboard_shortcut = (*cut)[0] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_ALT) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'K') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
	}
	BOOST_AUTO_TEST_CASE(test_load_alt)
	{
		KeyMapperFake mapper ;
		shortcuts::KeyboardShortcuts keyboard_shortcuts(&mapper) ;
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
		KeyboardShortcuts::shortcut_map_ptr cut = keyboard_shortcuts.m_alt_shortcuts[L'K'] ;
		shortcut_ptr keyboard_shortcut = (*cut)[0] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_ALT) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'K') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
	}
	BOOST_AUTO_TEST_CASE(test_load_ctrl_alt)
	{
		KeyMapperFake mapper ;
		shortcuts::KeyboardShortcuts keyboard_shortcuts(&mapper) ;
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
		KeyboardShortcuts::shortcut_map_ptr cut = keyboard_shortcuts.m_ctrl_alt_shortcuts[L'K'] ;
		shortcut_ptr keyboard_shortcut = (*cut)[0] ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_type, shortcuts::SHORTCUT_CTRL_ALT) ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_firstkey, L'K') ;
		BOOST_CHECK_EQUAL(keyboard_shortcut->m_secondkey, 0) ;
	}

	BOOST_AUTO_TEST_CASE(test_load_two)
	{
		KeyMapperFake mapper ;
		shortcuts::KeyboardShortcuts keyboard_shortcuts(&mapper) ;
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
		KeyboardShortcuts::shortcut_map_ptr cut = keyboard_shortcuts.m_ctrl_shortcuts[L'K'] ;
		BOOST_CHECK_EQUAL((*cut)[0]->m_firstkey, L'K') ;
		cut = keyboard_shortcuts.m_ctrl_shortcuts[L'P'] ;
		BOOST_CHECK_EQUAL((*cut)[0]->m_firstkey, L'P') ;
	}

	BOOST_AUTO_TEST_CASE(test_load_secondkey)
	{
		KeyMapperFake mapper ;
		shortcuts::KeyboardShortcuts keyboard_shortcuts(&mapper) ;
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
		KeyboardShortcuts::shortcut_map_ptr cut = keyboard_shortcuts.m_ctrl_shortcuts[L'K'] ;
		BOOST_CHECK_EQUAL((*cut)[L'M']->m_type, shortcuts::SHORTCUT_CTRL) ;
		BOOST_CHECK_EQUAL((*cut)[L'M']->m_firstkey, L'K') ;
		BOOST_CHECK_EQUAL((*cut)[L'M']->m_secondkey, L'M') ;
	}
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( KeyboardShortcutsMapperTest )

	using namespace shortcuts ;
	
	BOOST_AUTO_TEST_CASE(test_ctrl_m)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;
		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, L'M', 0, "FooBar"))) ;

		keyboard_shortcuts.receive_keypress(true, false, L'M') ;

		BOOST_CHECK_EQUAL(1u, mapper.m_sensing_variable.size());
		BOOST_CHECK_EQUAL("FooBar", mapper.m_sensing_variable[0]);
	}
	BOOST_AUTO_TEST_CASE(test_toggle)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;

		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, L'M', 0, "FooBar"))) ;
		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, VK_F1, 0, "ToggleShortcuts"))) ;

		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, VK_F1)) ;
		BOOST_CHECK(! keyboard_shortcuts.receive_keypress(true, false, L'M')) ;

		BOOST_CHECK_EQUAL(false, keyboard_shortcuts.m_shortcuts_enabled);
		BOOST_CHECK_EQUAL(0u, mapper.m_sensing_variable.size());
	}
	BOOST_AUTO_TEST_CASE(test_toggle_back)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;

		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, L'M', 0, "FooBar"))) ;
		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, VK_F1, 0, "ToggleShortcuts"))) ;

		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, VK_F1)) ;
		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, VK_F1)) ;
		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, L'M')) ;

		BOOST_CHECK_EQUAL(true, keyboard_shortcuts.m_shortcuts_enabled);
		BOOST_CHECK_EQUAL(1u, mapper.m_sensing_variable.size());
		BOOST_CHECK_EQUAL("FooBar", mapper.m_sensing_variable[0]);
	}
	BOOST_AUTO_TEST_CASE(test_ctrl_m_missed)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;
		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, L'K', 0, "FooBar"))) ;

		BOOST_CHECK(! keyboard_shortcuts.receive_keypress(true, false, L'M')) ;

		BOOST_CHECK_EQUAL(0u, mapper.m_sensing_variable.size());
	}

	BOOST_AUTO_TEST_CASE(test_second_key_first_hit)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;
		shortcut_ptr cut(new KeyboardShortcut(SHORTCUT_CTRL, L'M', L'X', "FooBar")) ;
		keyboard_shortcuts.add_shortcut(cut) ;

		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, L'M')) ;

		BOOST_CHECK_EQUAL(0u, mapper.m_sensing_variable.size());
		BOOST_CHECK_EQUAL(L'M', keyboard_shortcuts.m_second_key);
	}
	BOOST_AUTO_TEST_CASE(test_second_key_second_hit)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;
		keyboard_shortcuts.add_shortcut(shortcut_ptr(new KeyboardShortcut(SHORTCUT_CTRL, L'M', L'X', "FooBar"))) ;

		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, L'M')) ;
		BOOST_CHECK(keyboard_shortcuts.receive_keypress(true, false, L'X')) ;

		BOOST_CHECK_EQUAL("FooBar", mapper.m_sensing_variable[0]);
		BOOST_CHECK_EQUAL(1u, mapper.m_sensing_variable.size());
		BOOST_CHECK_EQUAL(0, keyboard_shortcuts.m_second_key);
	}
	BOOST_AUTO_TEST_CASE(test_second_key_second_miss)
	{
		KeyMapperFake mapper ;
		KeyboardShortcuts keyboard_shortcuts(&mapper) ;
		shortcut_ptr cut(new KeyboardShortcut(SHORTCUT_CTRL, L'M', L'X', "FooBar")) ;
		keyboard_shortcuts.add_shortcut(cut) ;

		keyboard_shortcuts.receive_keypress(true, false, L'M') ;
		keyboard_shortcuts.receive_keypress(false, false, L'K') ;

		BOOST_CHECK_EQUAL(0u, mapper.m_sensing_variable.size());
		BOOST_CHECK_EQUAL(0, keyboard_shortcuts.m_second_key);
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
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("a"), L'A') ;
	}
	BOOST_AUTO_TEST_CASE(test_A)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("A"), L'A') ;
	}
	BOOST_AUTO_TEST_CASE(test_1)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("1"), L'1') ;
	}
	BOOST_AUTO_TEST_CASE(test_vk_right)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("RIGHT"), VK_RIGHT) ;
	}
	BOOST_AUTO_TEST_CASE(test_vk_delete)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_key("delete"), VK_DELETE) ;
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( test_get_shortcut_type )

	BOOST_AUTO_TEST_CASE(test_ctrl)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_shortcut_type("ctrl"), shortcuts::SHORTCUT_CTRL) ;
	}
	BOOST_AUTO_TEST_CASE(test_alt)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_shortcut_type("alt"), shortcuts::SHORTCUT_ALT) ;
	}
	BOOST_AUTO_TEST_CASE(test_ctrl_alt)
	{
		shortcuts::KeyboardShortcuts keyboard_shortcuts(NULL) ;
		BOOST_CHECK_EQUAL(keyboard_shortcuts.get_shortcut_type("ctrl+alt"), shortcuts::SHORTCUT_CTRL_ALT) ;
	}

BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
