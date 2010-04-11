#pragma once

#include "view_interface.h"
#include "FelixModelInterface.h"
#include "window_listener.h"
#include "search_match.h"

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
	FelixModelInterface *m_model ;
	WindowListener *m_window_listener ;

	ViewState():
		m_view(NULL),
		m_model(NULL),
		m_window_listener(NULL)
	{
	}

	void set_view(view_interface *view)
	{
		m_view = view ;
	}
	void set_model(FelixModelInterface *model)
	{
		m_model = model ;
	}
	void set_window_listener(WindowListener *listener)
	{
		m_window_listener = listener ;
	}

	void on_user_edit()
	{
		search_match_ptr match = this->get_current_match() ;
		m_window_listener->set_item_under_edit(match) ;
		ATLASSERT( match->get_memory_id() != 0 ) ;
		m_window_listener->show_edit_dialog( match->get_record(), match->get_memory_id(), get_edit_record_title() ) ;
	}

	virtual void set_current(size_t num)
	{
		num ;
	}

	virtual int get_edit_record_title() 
	{
		return IDS_EDIT_RECORD_TITLE;
	}
	// pure virtual
	virtual void activate() = 0 ;
	virtual void handle_toggle_edit_mode() = 0 ;
	virtual void show_content() = 0 ;
	virtual void retrieve_edit_record(int mem_id, mem_engine::record_pointer new_rec) = 0 ;
	virtual mem_engine::search_match_ptr get_current_match() = 0 ;
	virtual void delete_match(size_t index) = 0 ;
};

