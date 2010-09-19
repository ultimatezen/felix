#include "stdafx.h"
#include "keyboard_shortcuts_excel.h"
#include "key_mapper_excel.h"
#include "file.h"
#include "Path.h"

string get_shortcuts_text(CString base_filename)
{
	CString filename = get_config_filename(base_filename) ;
	if (! file::CPath(filename).FileExists())
	{
		write_default_shortcuts_file(filename) ;
	}
	file::view config_view ;
	string text(static_cast<LPCSTR>(config_view.create_view_readonly(filename))) ;

	return text ;
}


string get_default_file_text(void)
{

	pugi::xml_document doc;
	pugi::xml_node shortcuts_node = doc.append_child();
	shortcuts_node.set_name("shortcuts") ;

	// ctrl+alt
	create_shortcut_node(shortcuts_node, "CTRL+ALT", "F9", "", "ToggleShortcuts");

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

void write_default_shortcuts_file(CString filename)
{
	const string text = get_default_file_text() ;

	file::file outfile ;
	outfile.open_always(filename) ;
	outfile.write(text) ;
	outfile.close() ;
}