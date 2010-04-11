#pragma once

#include "view_state.h"

class ViewStateInitial : public ViewState
{
public:
	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
	int get_edit_record_title();
	void delete_match(size_t index);

};

class ViewStateInitialMain : public ViewStateInitial
{
public:
	void show_content();

	mem_engine::search_match_ptr get_current_match();
	void activate();
};

class ViewStateInitialGloss : public ViewStateInitial
{
public:
	void show_content();
	mem_engine::search_match_ptr get_current_match();
	void activate()
	{

	}
};