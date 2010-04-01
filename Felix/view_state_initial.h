#pragma once

#include "view_state.h"

class ViewStateInitial : public ViewState
{
public:
	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
};

class ViewStateInitialMain : public ViewStateInitial
{
public:
	void show_content();

};

class ViewStateInitialGloss : public ViewStateInitial
{
public:
	void show_content();
};