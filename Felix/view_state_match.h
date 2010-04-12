#pragma once

#include "view_state.h"
#include "record.h"
#include "TranslationMemory.h"
#include "query.h"

class ViewStateMatch : public ViewState
{
public:
	typedef mem_engine::record_pointer record_pointer ;
	typedef mem_engine::memory_pointer memory_pointer ;

	mem_engine::felix_query *m_search_matches ;

	ViewStateMatch() : m_search_matches(NULL)
	{

	}	
	void set_current(size_t num);
	virtual size_t get_current();
	void set_search_matches(mem_engine::felix_query *search_matches);
	int get_edit_record_title();
	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
	void delete_match(size_t index);
};

class ViewStateMatchMain : public ViewStateMatch
{
public:
	void show_content();
	mem_engine::search_match_ptr get_current_match();
	void activate();

};

class ViewStateMatchGloss : public ViewStateMatch
{
public:
	void show_content();
	mem_engine::search_match_ptr get_current_match();
	void activate()
	{
	}
};