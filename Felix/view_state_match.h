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
	void set_search_matches(mem_engine::felix_query *search_matches)
	{
		m_search_matches = search_matches ;
	}
	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
};

class ViewStateMatchMain : public ViewStateMatch
{
public:
	void show_content();
	mem_engine::search_match_ptr get_current_match();

};

class ViewStateMatchGloss : public ViewStateMatch
{
public:
	void show_content();
	mem_engine::search_match_ptr get_current_match();
};