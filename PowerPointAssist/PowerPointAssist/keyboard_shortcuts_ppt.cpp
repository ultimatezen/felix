#include "stdafx.h"
#include "keyboard_shortcuts_ppt.h"
#include "file.h"
#include "Path.h"

string get_shortcuts_text(CString base_filename, input_device_ptr input, output_device_ptr output)
{
	try
	{
		CString filename = get_config_filename(base_filename, output) ;
		if (! input->exists(filename))
		{
			write_default_shortcuts_file(filename, output) ;
		}
		file::view config_view ;
		string text(input->create_view_const_char(filename)) ;

		if (text.empty())
		{
			return get_default_file_text() ;
		}
		return text ;
	}
	catch (except::CException& e)
	{
		logging::log_error("Program exception: Failed to get shortcuts text") ;
		logging::log_exception(e) ;
		return get_default_file_text() ;
	}
}


string get_default_file_text(void)
{
	
	pugi::xml_document doc;
	pugi::xml_node shortcuts_node = doc.append_child();
	shortcuts_node.set_name("shortcuts") ;

	// ctrl
	create_shortcut_node(shortcuts_node, "CTRL", "RIGHT", "", CMD_EXTEND_LOOKUP);

	// ctrl+alt
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "RIGHT", "", CMD_LOOK_UP_NEXT_TRANS);
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "UP", "", CMD_CORRECT_TRANS);
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "E", "", CMD_EXTEND_TRANS_LOOKUP);
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "L", "", CMD_LOOK_UP_TRANS);
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "C", "", CMD_TRANS_CONCORDANCE);

	create_shortcut_node(shortcuts_node, "CTRL+ALT", "F9", "", "ToggleShortcuts");

	// alt
	create_shortcut_node(shortcuts_node, "ALT", "RIGHT", "", CMD_LOOK_UP_NEXT);
	create_shortcut_node(shortcuts_node, "ALT", "UP", "", CMD_SET);
	create_shortcut_node(shortcuts_node, "ALT", "DOWN", "", CMD_GET);
	create_shortcut_node(shortcuts_node, "ALT", "P", "", CMD_PREVIOUS);
	create_shortcut_node(shortcuts_node, "ALT", "N", "", CMD_NEXT);
	create_shortcut_node(shortcuts_node, "ALT", "G", "", CMD_GET_AND_NEXT);
	create_shortcut_node(shortcuts_node, "ALT", "S", "", CMD_SET_AND_NEXT);
	create_shortcut_node(shortcuts_node, "ALT", "L", "", CMD_LOOK_UP);
	create_shortcut_node(shortcuts_node, "ALT", "C", "", CMD_AUTO_TRANS_TO_FUZZY);
	create_shortcut_node(shortcuts_node, "ALT", "D", "", CMD_DELETE);

	 // entries
	create_shortcut_node(shortcuts_node, "ALT", "0", "", CMD_ENTRY_0);
	create_shortcut_node(shortcuts_node, "ALT", "1", "", CMD_ENTRY_1);
	create_shortcut_node(shortcuts_node, "ALT", "2", "", CMD_ENTRY_2);
	create_shortcut_node(shortcuts_node, "ALT", "3", "", CMD_ENTRY_3);
	create_shortcut_node(shortcuts_node, "ALT", "4", "", CMD_ENTRY_4);
	create_shortcut_node(shortcuts_node, "ALT", "5", "", CMD_ENTRY_5);
	create_shortcut_node(shortcuts_node, "ALT", "6", "", CMD_ENTRY_6);
	create_shortcut_node(shortcuts_node, "ALT", "7", "", CMD_ENTRY_7);
	create_shortcut_node(shortcuts_node, "ALT", "8", "", CMD_ENTRY_8);
	create_shortcut_node(shortcuts_node, "ALT", "9", "", CMD_ENTRY_9);

	xml_string_writer writer ;
	doc.save(writer) ;

	return writer.result ;
}

void write_default_shortcuts_file(CString filename, output_device_ptr output)
{
	const string text = get_default_file_text() ;

	output->open(filename) ;
	output->write(text) ;
	output->close() ;
}