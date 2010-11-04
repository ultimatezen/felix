#include "stdafx.h"
#include "keyboard_shortcuts_word.h"
#include "key_mapper_word.h"
#include "file.h"
#include "Path.h"
#include "logging.h"

string get_shortcuts_text(CString base_filename, input_device_ptr input)
{
	try
	{
		CString filename = get_config_filename(base_filename) ;
		if (! input->exists(filename))
		{
			OutputDeviceFile output ;
			write_default_shortcuts_file(filename, &output) ;
		}
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
	create_shortcut_node(shortcuts_node, "CTRL", "RIGHT", "", CmdExtendLookupAction);

	// ctrl+alt
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "UP", "", CmdCorrectAction);
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "F9", "", "ToggleShortcuts");

	// alt
	create_shortcut_node(shortcuts_node, "ALT", "RIGHT", "", CmdLookupNextAction);
	create_shortcut_node(shortcuts_node, "ALT", "UP", "", CmdSetAction);
	create_shortcut_node(shortcuts_node, "ALT", "DOWN", "", CmdGetAction);
	create_shortcut_node(shortcuts_node, "ALT", "OEM_PERIOD", "", CmdToMaruAction);
	create_shortcut_node(shortcuts_node, "ALT", ".", "", CmdToMaruAction);
	create_shortcut_node(shortcuts_node, "ALT", "P", "", CmdPrevAction);
	create_shortcut_node(shortcuts_node, "ALT", "N", "", CmdNextAction);
	create_shortcut_node(shortcuts_node, "ALT", "G", "", CmdGetAndNextAction);
	create_shortcut_node(shortcuts_node, "ALT", "S", "", CmdSetAndNextAction);
	create_shortcut_node(shortcuts_node, "ALT", "L", "", CmdLookupAction);
	create_shortcut_node(shortcuts_node, "ALT", "Z", "", CmdAutoTransFuzzyAction);
	create_shortcut_node(shortcuts_node, "ALT", "D", "", CmdDeleteAction);

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

	// meta-keys
	create_shortcut_node(shortcuts_node, "ALT", "M", "H", CmdGlossNAction);
	create_shortcut_node(shortcuts_node, "ALT", "M", "A", CmdAutoTransAction);
	create_shortcut_node(shortcuts_node, "ALT", "M", "G", CmdRegisterGlossAction);
	create_shortcut_node(shortcuts_node, "ALT", "M", "S", CmdSaveMemoryAction);
	create_shortcut_node(shortcuts_node, "ALT", "M", "C", CmdConcordanceAction);

	//////////////////////////////////////////////////////////////////////////
	// unused
	//////////////////////////////////////////////////////////////////////////
	create_shortcut_node(shortcuts_node, "", "", "", CmdRestoreAction);

	xml_string_writer writer ;
	doc.save(writer) ;

	return writer.result ;
}

void write_default_shortcuts_file(CString filename, OutputDevice *output)
{
	const string text = get_default_file_text() ;

	output->open(filename) ;
	output->write(text) ;
	output->close() ;
}