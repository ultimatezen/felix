#include "stdafx.h"
#include "view_state_match.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "record_local.h"
#include "system_message.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateMatch
//////////////////////////////////////////////////////////////////////////

void ViewStateMatch::set_search_matches( mem_engine::felix_query *search_matches )
{
	m_search_matches = search_matches ;
}

int ViewStateMatch::get_edit_record_title()
{
	if (m_search_matches->empty())
	{
		return IDS_ADD_ENTRY;
	}
	return IDS_EDIT_RECORD_TITLE ;
}

void ViewStateMatch::set_current( size_t num )
{
	m_search_matches->set_current(num) ;
}

void ViewStateMatch::delete_match( size_t index )
{
	if ( m_search_matches->empty() )
	{
		m_window_listener->user_feedback(IDS_NO_MATCHES);
		return ;
	}

	if ( index >= m_search_matches->size() )
	{
#ifndef UNIT_TEST
		MessageBeep(MB_ICONEXCLAMATION) ;
#endif
		m_window_listener->user_feedback(IDS_OUT_OF_RANGE) ;
		return ;
	}

	if ( ! m_window_listener->check_delete() )
	{
		return ;
	}

	search_match_ptr match = m_search_matches->at(index) ;
	mem_engine::memory_pointer mem = m_model->get_memory_by_id(match->get_memory_id()) ;
	mem->erase(match->get_record()) ;

	m_search_matches->erase_at(index) ;
	m_window_listener->user_feedback( IDS_DELETED_ENTRY ) ;

	if ( m_search_matches->empty() )
	{
		const wstring feedback = L"<center><h1>" + resource_string_w( IDS_NO_MATCHES ) + L"</h1></center>" ;
		m_view->set_text(feedback) ;
		return ;
	}

	this->show_content() ;

}

size_t ViewStateMatch::get_current()
{
	return m_search_matches->current_pos() ;
}

//////////////////////////////////////////////////////////////////////////
// ViewStateMatchMain
//////////////////////////////////////////////////////////////////////////
void ViewStateMatchMain::handle_toggle_edit_mode()
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

void ViewStateMatchMain::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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
		match->set_memory_id(this->m_model->get_first_memory()->get_id()) ;
		return match ;
	}
	return m_search_matches->current( ) ;
}

void ViewStateMatchMain::activate()
{
	m_window_listener->set_menu_checkmark(ID_VIEW_MATCH, true);
	m_window_listener->set_menu_checkmark(ID_VIEW_SEARCH, false);
}

//////////////////////////////////////////////////////////////////////////
// ViewStateMatchGloss
//////////////////////////////////////////////////////////////////////////
void ViewStateMatchGloss::handle_toggle_edit_mode()
{
	if ( ! m_view->is_edit_mode() ) // we are entering edit mode
	{
		// user feedback
		m_window_listener->user_feedback( IDS_ENTERING_EDIT_MODE ) ;
		m_view->handle_enter_edit_mode_new_record() ;
		m_window_listener->user_feedback( IDS_IN_EDIT_MODE ) ;
	}
	else
	{
		m_window_listener->user_feedback( IDS_LEAVING_EDIT_MODE ) ;

		if( false == m_view->handle_leave_edit_mode_concordance_glossary( m_model->get_memories(), m_search_matches ) )
		{
			ATLTRACE(" ... All records deleted\n") ;
			m_window_listener->user_feedback( IDS_DELETED_ENTRY ) ;
			m_view->set_text( R2WSTR( IDS_POST_EDIT_ALL_DELETED ) ) ;
			m_window_listener->check_mousewheel() ;
			::MessageBeep( MB_ICONINFORMATION ) ;
			return ;
		}
		m_window_listener->user_feedback( IDS_LEFT_EDIT_MODE ) ;

		this->show_content() ;
	}
}

void ViewStateMatchGloss::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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
	m_window_listener->set_new_record(new_rec) ;
	m_window_listener->user_feedback( IDS_ADDED_TRANSLATION ) ;
}

void ViewStateMatchGloss::show_content()
{
	wstring html_content ;

	if ( m_search_matches->empty() )
	{
		html_content << L"<p>" << system_message_w( IDS_FOUND_X_MATCHES, int_arg_w(0) ) << L"</p>" ;
	}
	else
	{
		html_content << m_search_matches->get_html_short() ;
	}

	m_view->set_text(html_content) ;
	m_view->set_scroll_pos(0) ;
	m_window_listener->check_mousewheel() ;
	// give the user feedback
	if ( m_search_matches->size() == 1 )
	{
		m_window_listener->user_feedback( IDS_FOUND_1_MATCH ) ;
	}
	else
	{
		m_window_listener->user_feedback(system_message(IDS_FOUND_X_MATCHES, int_arg(m_search_matches->size()))) ;
	}
}

mem_engine::search_match_ptr ViewStateMatchGloss::get_current_match()
{
	if (m_search_matches->empty())
	{
		record_pointer rec(new mem_engine::record_local) ;
		rec->set_source( m_search_matches->get_query_rich() ) ;
		search_match_ptr match = this->m_model->get_first_memory()->make_match() ;
		match->set_record(rec) ;
		match->set_values_to_record() ;
		return match ;
	}
	return m_search_matches->current( ) ;
}