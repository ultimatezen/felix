#pragma once

#include "TranslationMemory.h"
#include "Felix_properties.h"

/**
 * Listener for frame window.
 * Allows children of the window to query the frame.
 */
class FrameListener
{
public:
	virtual bool set_window_title() = 0 ;
	virtual void save_memory_as(mem_engine::memory_pointer mem) = 0;
	virtual void get_qc_messages(mem_engine::record_pointer record, std::vector<wstring> &messages) = 0;
	virtual INT_PTR check_save_memory(mem_engine::memory_pointer mem) = 0;
	virtual app_props::props_ptr get_properties() = 0 ;
};