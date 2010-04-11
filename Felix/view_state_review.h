#pragma once

#include "view_state.h"
#include "record.h"
#include "TranslationMemory.h"
#include "query.h"

class ViewStateReview : public ViewState
{
public:
	typedef mem_engine::record_pointer record_pointer ;
	typedef mem_engine::memory_pointer memory_pointer ;

	mem_engine::felix_query *m_search_matches ;

	ViewStateReview() : m_search_matches(NULL)
	{

	}	
	void set_search_matches(mem_engine::felix_query *search_matches);
	void set_current(size_t num);
	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
	void show_content();
	mem_engine::search_match_ptr get_current_match();
	void activate();
	void delete_match(size_t index);
};

