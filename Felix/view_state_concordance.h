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

	bool m_should_scroll ;

	ViewStateConcordance();
	virtual ~ViewStateConcordance(){}
	void set_current(size_t num);
	virtual size_t get_current();
	void set_search_matches(mem_engine::felix_query *search_matches);
	int get_edit_record_title();
	void redo_concordance();
	void delete_match(size_t index);
	mem_engine::search_match_ptr get_current_match();
};

class ViewStateConcordanceMain : public ViewStateConcordance
{
public:
	void show_content();

	wstring get_no_match_content();

	void activate();
	void handle_toggle_edit_mode();
	void retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add);

};

class ViewStateConcordanceGloss : public ViewStateConcordance
{
public:
	app_props::properties_glossary		*m_properties_gloss ;

	ViewStateConcordanceGloss() : m_properties_gloss(NULL){}

	void set_app_props(app_props::properties_glossary *properties)
	{
		m_properties_gloss = properties ;
	}
	void show_content();
	void handle_toggle_edit_mode();
	void retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add);
	void activate()
	{

	}
};
