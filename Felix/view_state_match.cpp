#include "stdafx.h"
#include "view_state_match.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "record_local.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateMatch
//////////////////////////////////////////////////////////////////////////

void ViewStateMatch::handle_toggle_edit_mode()
{
	if ( ! m_view->is_edit_mode() ) // we are entering edit mode
	{
		// user feedback
		m_window_listener->user_feedback( IDS_ENTERING_EDIT_MODE ) ;

		m_view->handle_enter_edit_mode_match(m_search_matches) ;

		m_window_listener->user_feedback( IDS_IN_EDIT_MODE ) ;
	}
	else
	{
		m_window_listener->user_feedback( IDS_LEAVING_EDIT_MODE ) ;

		m_view->handle_leave_edit_mode_match( m_model->get_memories(), m_search_matches ) ;

		m_window_listener->user_feedback( IDS_LEFT_EDIT_MODE ) ;

		for ( size_t i = 0u ; i < m_search_matches->size() ; ++i )
		{
			mem_engine::search_match_ptr match = m_search_matches->at( i ) ;
			if ( i == m_search_matches->current_pos() ) 
			{
				m_window_listener->redo_lookup( match, true ) ;
			}
			else
			{
				m_window_listener->redo_lookup( match, false ) ;
			}
		}

		this->show_content() ;
	}
}

void ViewStateMatch::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
{
	mem_engine::memory_pointer mem = m_model->get_memory_by_id(mem_id) ;
	mem_engine::search_match_ptr current_match = m_window_listener->get_item_under_edit() ;
	ATLASSERT( mem_id == current_match->get_memory_id() ) ;
	const mem_engine::record_pointer old_rec = current_match->get_record() ;
	if (old_rec->is_valid_record())
	{
		mem->replace(old_rec, new_rec) ;
	}
	else
	{
		mem->add_record(new_rec) ;
	}
	current_match->set_record(new_rec) ;
	current_match->set_values_to_record() ;
	m_window_listener->set_new_record(new_rec) ;
	m_window_listener->redo_lookup(current_match, true) ;
	m_window_listener->user_feedback(IDS_CORRECTED_TRANS) ;
}

//////////////////////////////////////////////////////////////////////////
// ViewStateMatchMain
//////////////////////////////////////////////////////////////////////////

void ViewStateMatchMain::show_content()
{
	wstring content ;
	if ( m_window_listener->is_single_page() ) 
	{
		content = m_search_matches->get_html_all() ;			
	}
	else if ( m_window_listener->is_short_format() )
	{
		content = m_search_matches->get_html_short() ; 
	}
	else
	{
		content = m_search_matches->get_html_long() ;
	}

	m_view->set_text( content ) ;
	m_window_listener->check_mousewheel() ;

	m_view->ensure_document_complete() ;
	if ( m_window_listener->is_single_page() ) 
	{
		const wstring current_id = ulong2wstring( m_search_matches->current_pos() ) ;
		m_view->scroll_element_into_view( current_id ) ;
	}
	else
	{
		m_view->set_scroll_pos(0) ;
	}
	m_window_listener->set_bg_color_if_needed() ;
}

mem_engine::search_match_ptr ViewStateMatchMain::get_current_match()
{
	if (m_search_matches->empty())
	{
		record_pointer rec(new mem_engine::record_local) ;
		rec->set_source( m_search_matches->get_query_rich() ) ;
		search_match_ptr match(new search_match(rec)) ;
		match->set_values_to_record() ;
		return match ;
	}
	return m_search_matches->current( ) ;
}
//////////////////////////////////////////////////////////////////////////
// ViewStateMatchGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateMatchGloss::show_content()
{

}

mem_engine::search_match_ptr ViewStateMatchGloss::get_current_match()
{
	search_match_ptr match(new search_match(record_pointer(new mem_engine::record_local))) ;
	return match ;
}