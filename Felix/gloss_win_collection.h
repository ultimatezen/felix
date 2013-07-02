#pragma once

#include "GlossaryWindowFrame.h"

typedef std::vector< gloss_window_pointer > gloss_window_list ;

class GlossWinCollection
{
public:
	gloss_window_list m_glossary_windows ;

	boost::function<gloss_window_pointer(app_props::props_ptr props)> m_add ;

	GlossWinCollection();
	void remove_destroyed_gloss_windows();
	bool pre_translate(MSG *msg);
	gloss_window_pointer add(app_props::props_ptr props);
	bool create(gloss_window_pointer gloss, HWND parent);
	void add_record(mem_engine::record_pointer record);
	bool empty();

};