#pragma once

#include "view_interface.h"
#include "FelixModelInterface.h"
#include "window_listener.h"
#include "search_match.h"
#include "document_wrapper.h"

wstring get_no_match_text() ;

//! Stores preferences for how records (translations) should be retrieved.
//! Some people want to retrieve (mostly gloss entries) as plain text
//! without HTML markup, or as all lowercase (especially glossary).
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

//! This represents the viewing state in one of the windows.
class ViewState
{
public:
	typedef mem_engine::search_match_ptr search_match_ptr ;
	typedef mem_engine::search_match search_match ;
	typedef mem_engine::record_pointer record_pointer ;

	DECLARE_SENSING_VAR ;

	view_interface *m_view ; //!< The view object. We manipulate this depending on our state.
	model_iface_ptr m_model ; //!< The TM/glossary collection
	WindowListener *m_window_listener ; //!< Listener to parent frame

	ViewState():
		m_view(NULL),
		m_window_listener(NULL)
	{
	}

	virtual ~ViewState(){}

	//! Set the view object
	void set_view(view_interface *view);
	//! Set the TM/gloss model
	void set_model(model_iface_ptr model);
	//! Set the listener to the parent frame
	void set_window_listener(WindowListener *listener);

	//! Shows the edit dialog for the record under edit
	void on_user_edit();

	//! Gets the translation string according to the string prefs.
	//! The prefs might be stripping HTML tags or converting to lowercase.
	wstring retrieve_record_trans(record_pointer rec, record_string_prefs prefs);
	//! Gets the record at `index`.
	record_pointer get_specified_record(size_t index);
	//! Sets the current record. 
	//! This makes the record at `num` the "record under edit".
	virtual void set_current(size_t num);
	//! Gets the current "record under edit"
	virtual size_t get_current();

	//! Gets the title string for the edit record dialog.
	//! Differs depending on whether this is a TM or glossary view state.
	virtual int get_edit_record_title();

	//! Given a match, retrieves the containing TM
	mem_engine::memory_pointer get_memory_from_match( mem_engine::search_match_ptr match );
	//! Removes `match` from TM
	void erase_from_memory( mem_engine::search_match_ptr match );
	//! Retrieves a pointer to the HTML view
	doc3_wrapper_ptr get_doc3();
	//! Sets the content of the div element with name `div_name`
	void set_div_content(const wstring div_name, const wstring &div_content);

	//////////////////////////////////////////////////////////////////////////
	// pure virtual
	//////////////////////////////////////////////////////////////////////////

	//! This view has just become active
	virtual void activate() = 0 ;
	//! Edit mode has been toggled
	virtual void handle_toggle_edit_mode() = 0 ;
	//! Set view state's content in the HTML window
	virtual void show_content() = 0 ;
	//! Puts the edited record in the TM
	virtual void retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add) = 0 ;
	//! Gets the current match
	virtual mem_engine::search_match_ptr get_current_match() = 0 ;
	//! Deletes the match at `index`
	virtual void delete_match(size_t index) = 0 ;
};

//! Shows feedback when a match has been deleted.
//! View state is a template param so we can test with a fake object.
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
		view_state.m_should_scroll = false ;
		view_state.show_content() ;
		view_state.m_should_scroll = true ;
	}
}

//! Deletes the search match at `index` from the TM
//! View state is a template param so we can test with a fake object.
template<class VIEW_STATE>
void erase_by_index(VIEW_STATE &view_state, size_t index)
{
	view_state.erase_from_memory(view_state.m_search_matches->at(index)) ;
	view_state.m_search_matches->erase_at(index) ;
}
