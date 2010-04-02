#pragma once

#include "view_state.h"
#include "record.h"
#include "TranslationMemory.h"

class ViewStateMatch : public ViewState
{
public:
	typedef mem_engine::record_pointer record_pointer ;
	typedef mem_engine::memory_pointer memory_pointer ;

	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
};

class ViewStateMatchMain : public ViewStateMatch
{
public:
	void show_content();

};

class ViewStateMatchGloss : public ViewStateMatch
{
public:
	void show_content();
};