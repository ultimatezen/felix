#pragma once

#include "GlossaryWindowFrame.h"

typedef std::vector< gloss_window_pointer > gloss_window_list ;

//! Wraps the list of glossary windows maintained by the memory window frame.
class GlossWinCollection
{
public:
	gloss_window_list m_glossary_windows ;

	boost::function<gloss_window_pointer(app_props::props_ptr props)> m_add ;

	GlossWinCollection();
	
	// collection wrappers
	bool empty();
	size_t size();
	gloss_window_pointer first();

	// Remove glossary windows that have been destroyed.
	void remove_destroyed_gloss_windows();
	// Run pre-translate on the message.
	bool pre_translate(MSG *msg);

	// Add a new glossary window. Does not yet create it.
	gloss_window_pointer add(app_props::props_ptr props);
	// Creates the glossary window.
	bool create(gloss_window_pointer gloss, HWND parent);
	// Adds a record to the top glossary window.
	void add_record(mem_engine::record_pointer record);
	// Gets the glossary entry with the specified index.
	wstring get_glossary_entry(short index);
	// Sets the UI language (based on props)
	void set_ui_language();
	// Serializes preferences.
	void save_prefs();
	// Removes the specified record from the glossary windows.
	void remove_gloss_record(mem_engine::record_pointer rec);
	// Gets the glossary matches from the active glossary. 
	// NULL if there are no glossaries.
	mem_engine::felix_query * get_current_matches();
	// Depending on whether the memory window is visible,
	// should the glossary window be visible or hidden?
	int get_show_command(BOOL is_visible);
	// Sets whether to show markup for matches.
	void put_show_marking( const VARIANT_BOOL setting );
	// Whether we should cancel shutdown at user instruction.
	bool pre_shutdown_save_check();
	// Look up the query in the glossaries
	void look_up(const wstring query);
	// Refresh view content
	void show_view_content();
	// show or hide gloss windows
	void put_visibility(int visibility);
	// Switch the next glossary window into focus
	bool gloss_view_switch(HWND child);
	// Save all glossaries with file dialog
	void on_file_save();
	// Clear glossaries
	void clear_glossaries();
	// Exit without prompting user to save glossaries
	void exit_silently();
	// Check for glossaries matching this filename
	bool has_clashes(CString location);
};