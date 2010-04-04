#pragma once

#include "view_state.h"
#include "record.h"
#include "TranslationMemory.h"

class ViewStateConcordance : public ViewState
{
public:
	typedef mem_engine::record_pointer record_pointer ;
	typedef mem_engine::memory_pointer memory_pointer ;
	mem_engine::felix_query		*m_search_matches ;

	ViewStateConcordance() : m_search_matches(NULL)
	{

	}
	void set_search_matches(mem_engine::felix_query *search_matches)
	{
		m_search_matches = search_matches ;
	}
	void redo_concordance();
	void handle_toggle_edit_mode();
	void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec);
};

class ViewStateConcordanceMain : public ViewStateConcordance
{
public:
	void show_content();

};

class ViewStateConcordanceGloss : public ViewStateConcordance
{
public:
	void show_content();
};