#include "stdafx.h"
#include "keyboard_shortcuts_ppt.h"
#include "file.h"
#include "Path.h"

struct xml_string_writer: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};


string get_shortcuts_text(void)
{
	CString filename = get_config_filename(SHORTCUTS_FILE) ;
	if (! file::CPath(filename).FileExists())
	{
		write_default_shortcuts_file(filename) ;
	}
	file::view config_view ;
	string text(static_cast<LPCSTR>(config_view.create_view_readonly(filename))) ;

	return text ;
}

void add_child_text(pugi::xml_node &node, string name, string text)
{
	pugi::xml_node child_node = node.append_child();
	child_node.set_name(name.c_str()) ;
	child_node.append_child(pugi::node_pcdata).set_value(text.c_str());
}

void create_shortcut_node(pugi::xml_node &node, string stype, string first, string second, string command)
{
	pugi::xml_node shortcut_node = node.append_child();
	shortcut_node.set_name("shortcut") ;
	add_child_text(shortcut_node, "type", stype) ;
	add_child_text(shortcut_node, "first", first) ;
	add_child_text(shortcut_node, "second", second) ;
	add_child_text(shortcut_node, "command", command) ;
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

void write_default_shortcuts_file(CString filename)
{
	const string text = get_default_file_text() ;

	file::file outfile ;
	outfile.open_always(filename) ;
	outfile.write(text) ;
	outfile.close() ;
}