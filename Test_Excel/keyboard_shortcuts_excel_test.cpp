#include "stdafx.h"
#include "keyboard_shortcuts_excel.h"
#include "keyboard_shortcuts.h"

#ifdef UNIT_TEST
#include <boost/test/unit_test.hpp>
#include "input_device_fake.h"
#include "output_device_fake.h"

BOOST_AUTO_TEST_SUITE( KeyboardShortcutsPptTest )

using namespace shortcuts;

	BOOST_AUTO_TEST_CASE(test_get_default_file_text)
	{
		const string text = get_default_file_text() ;
		shortcuts::KeyboardShortcuts keys(NULL) ;
		keys.load(text) ;

		BOOST_CHECK_EQUAL(19u, keys.m_alt_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(1u, keys.m_ctrl_alt_shortcuts.size()) ;
		BOOST_CHECK_EQUAL(0u, keys.m_ctrl_shortcuts.size()) ;

		KeyboardShortcuts::shortcut_map_ptr cut = keys.m_alt_shortcuts[L'5'] ;
		BOOST_CHECK_EQUAL(CmdEntry5Action, (*cut)[0]->m_command) ;
		cut = keys.m_alt_shortcuts[VK_UP] ;
		BOOST_CHECK_EQUAL(CmdSet, (*cut)[0]->m_command) ;
	}
	BOOST_AUTO_TEST_CASE(test_empty_file_brings_default_text)
	{
		InputDeviceFake *fake_in = new InputDeviceFake ;
		fake_in->set_view("") ;
		input_device_ptr input(fake_in) ;

		OutputDeviceFake *fake_out = new OutputDeviceFake ;
		output_device_ptr output(fake_out) ;

		string actual = get_shortcuts_text(_T("foo.xml"), input, output) ;
		string expected = get_default_file_text() ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
	BOOST_AUTO_TEST_CASE(test_write_default_shortcuts_file)
	{
		OutputDeviceFake *fake = new OutputDeviceFake ;
		output_device_ptr output(fake) ;

		write_default_shortcuts_file(_T("foo.xml"), output) ;

		string actual = string2string(fake->m_value) ;
		string expected = get_default_file_text() ;

		BOOST_CHECK_EQUAL(actual, expected) ;
	}
BOOST_AUTO_TEST_SUITE_END()

#endif // #ifdef _DEBUG
