#include "stdafx.h"
#include "keyboard_shortcuts_excel.h"
#include "key_mapper_excel.h"
#include "file.h"
#include "Path.h"
#include "logging.h"


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

	// ctrl+alt
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "F9", "", "ToggleShortcuts");
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "UP", "", CmdCorrectAction);

	// alt
	create_shortcut_node(shortcuts_node, "ALT", "RIGHT", "", CmdLookupNext);
	create_shortcut_node(shortcuts_node, "ALT", "UP", "", CmdSet);
	create_shortcut_node(shortcuts_node, "ALT", "DOWN", "", CmdGet);
	create_shortcut_node(shortcuts_node, "ALT", "P", "", CmdPrev);
	create_shortcut_node(shortcuts_node, "ALT", "N", "", CmdNext);
	create_shortcut_node(shortcuts_node, "ALT", "G", "", CmdGetAndNext);
	create_shortcut_node(shortcuts_node, "ALT", "S", "", CmdSetAndNext);
	create_shortcut_node(shortcuts_node, "ALT", "L", "", CmdLookup);
	create_shortcut_node(shortcuts_node, "ALT", "Z", "", CmdAutoTransSel);

	// entries
	create_shortcut_node(shortcuts_node, "ALT", "0", "", CmdEntry0Action);
	create_shortcut_node(shortcuts_node, "ALT", "1", "", CmdEntry1Action);
	create_shortcut_node(shortcuts_node, "ALT", "2", "", CmdEntry2Action);
	create_shortcut_node(shortcuts_node, "ALT", "3", "", CmdEntry3Action);
	create_shortcut_node(shortcuts_node, "ALT", "4", "", CmdEntry4Action);
	create_shortcut_node(shortcuts_node, "ALT", "5", "", CmdEntry5Action);
	create_shortcut_node(shortcuts_node, "ALT", "6", "", CmdEntry6Action);
	create_shortcut_node(shortcuts_node, "ALT", "7", "", CmdEntry7Action);
	create_shortcut_node(shortcuts_node, "ALT", "8", "", CmdEntry8Action);
	create_shortcut_node(shortcuts_node, "ALT", "9", "", CmdEntry9Action);

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