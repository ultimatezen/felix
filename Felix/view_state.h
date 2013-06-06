#pragma once

#include "view_interface.h"
#include "FelixModelInterface.h"
#include "window_listener.h"
#include "search_match.h"
#include "document_wrapper.h"

wstring get_no_match_text() ;

struct record_string_prefs 
{
	bool m_plain_text ;
	bool m_to_lower ;
	record_string_prefs(bool plain_text=false,
					    bool to_lower=false):
		m_plain_text(plain_text),
		m_to_lower(to_lower)
	{
	}
};
// This represents the viewing state in one of the
// windows.
class ViewState
{
public:
	typedef mem_engine::search_match_ptr search_match_ptr ;
	typedef mem_engine::search_match search_match ;
	typedef mem_engine::record_pointer record_pointer ;

	DECLARE_SENSING_VAR ;

	view_interface *m_view ;
	model_iface_ptr m_model ;
	WindowListener *m_window_listener ;

	ViewState():
		m_view(NULL),
		m_window_listener(NULL)
	{
	}

	virtual ~ViewState(){}

	void set_view(view_interface *view);
	void set_model(model_iface_ptr model);
	void set_window_listener(WindowListener *listener);

	void on_user_edit();

	wstring retrieve_record_trans(record_pointer rec, record_string_prefs prefs);
	virtual void set_current(size_t num);
	virtual size_t get_current();

	virtual int get_edit_record_title();


	mem_engine::memory_pointer get_memory_from_match( mem_engine::search_match_ptr match );
	void erase_from_memory( mem_engine::search_match_ptr match );
	doc3_wrapper_ptr get_doc3();
	void set_div_content(const wstring div_name, const wstring &div_content);
	// pure virtual
	virtual void activate() = 0 ;
	virtual void handle_toggle_edit_mode() = 0 ;
	virtual void show_content() = 0 ;
	virtual void retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add) = 0 ;
	virtual mem_engine::search_match_ptr get_current_match() = 0 ;
	virtual void delete_match(size_t index) = 0 ;
};

template<class VIEW_STATE>
void deleted_match_feedback(VIEW_STATE &view_state)
{
	view_state.m_window_listener->user_feedback( IDS_DELETED_ENTRY ) ;

	if ( view_state.m_search_matches->empty() )
	{
		const wstring feedback = get_no_match_text() ;
		view_state.m_view->set_text(feedback) ;
	}
	else
	{
		view_state.show_content() ;
	}
}

template<class VIEW_STATE>
void erase_by_index(VIEW_STATE &view_state, size_t index)
{
	search_match_ptr match = view_state.m_search_matches->at(index) ;
	view_state.erase_from_memory(match) ;
	view_state.m_search_matches->erase_at(index) ;
}
