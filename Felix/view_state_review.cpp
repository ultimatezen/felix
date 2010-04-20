#include "stdafx.h"
#include "view_state_review.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "record_local.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateReview
//////////////////////////////////////////////////////////////////////////

void ViewStateReview::handle_toggle_edit_mode()
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

		return ;
	}
}

void ViewStateReview::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
{
	memory_pointer mem ;
	try
	{
		mem = m_model->get_memory_by_id(mem_id) ;
	}
	catch (except::CProgramException &e )
	{
		logging::log_exception(e) ;
		mem = m_model->get_memories()->get_first_memory() ;
	}
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
	m_window_listener->set_review_record(new_rec) ;
	m_window_listener->redo_lookup(current_match, true) ;
	m_window_listener->user_feedback( IDS_CORRECTED_TRANS ) ;
}

void ViewStateReview::show_content()
{
	memory_pointer mem = m_model->get_first_memory() ;

	mem_engine::search_match_ptr match(mem->make_match()) ;
	match->set_record(m_window_listener->get_review_record()) ;
	match->set_values_to_record() ;

	text_tmpl::CTextTemplate engine ;
	m_search_matches->fill_match_template_params(engine, match);

	// fill in the template
	wstring content ;
	if ( m_window_listener->is_short_format() )
	{
		content = engine.Fetch(text_tmpl::get_template_text(_T("review.txt"))) ;
	}
	else
	{
		content = engine.Fetch(text_tmpl::get_template_text(_T("review_full.txt"))) ;
	}
	m_view->set_text(content) ;
	m_view->set_scroll_pos(0) ;
	m_window_listener->set_bg_color_if_needed() ;
}

mem_engine::search_match_ptr ViewStateReview::get_current_match()
{
	if (m_search_matches->empty())
	{
		record_pointer rec(new mem_engine::record_local) ;
		rec->set_source( m_search_matches->get_query_rich() ) ;
		search_match_ptr match(new search_match(rec)) ;
		match->set_values_to_record() ;
		match->set_memory_id(this->m_model->get_first_memory()->get_id()) ;
		return match ;
	}
	return m_search_matches->current( ) ;
}

void ViewStateReview::activate()
{
	m_window_listener->set_menu_checkmark(ID_VIEW_MATCH, true);
	m_window_listener->set_menu_checkmark(ID_VIEW_SEARCH, false);
}

void ViewStateReview::set_search_matches( mem_engine::felix_query *search_matches )
{
	m_search_matches = search_matches ;
}

void ViewStateReview::set_current( size_t num )
{
	m_search_matches->set_current(num) ;
}

void ViewStateReview::delete_match( size_t index )
{
	index ;
	ATLASSERT( index == 0 ) ;
	memory_pointer mem = m_model->get_memories()->get_first_memory() ;

	if ( ! m_window_listener->check_delete() )
	{
		return ;
	}
	mem->erase(m_window_listener->get_review_record()) ;

	const wstring feedback = L"<center><h1>" + resource_string_w( IDS_DELETED_ENTRY ) + L"</h1></center>" ;
	m_view->set_text(feedback) ;
	m_window_listener->user_feedback(IDS_DELETED_ENTRY) ;
	m_window_listener->check_mousewheel() ;
	m_view->set_scroll_pos(0) ;
}

size_t ViewStateReview::get_current()
{
	return m_search_matches->current_pos() ;
}