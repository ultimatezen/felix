#pragma once

#include "view_state.h"
#include "record.h"
#include "TranslationMemory.h"

class ViewStateNew : public ViewState
{
public:
	typedef mem_engine::record_pointer record_pointer ;
	typedef mem_engine::memory_pointer memory_pointer ;

	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
	void delete_match(size_t index);
};

class ViewStateNewMain : public ViewStateNew
{
public:
	void handle_toggle_edit_mode();
	void show_content();
	mem_engine::search_match_ptr get_current_match();
	void activate();

};

class ViewStateNewGloss : public ViewStateNew
{
public:
	void handle_toggle_edit_mode();
	void show_content();
	mem_engine::search_match_ptr get_current_match();
	void activate();
};