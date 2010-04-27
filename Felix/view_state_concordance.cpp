#include "stdafx.h"
#include "view_state_concordance.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "system_message.h"
#include "record_local.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateConcordance
//////////////////////////////////////////////////////////////////////////



void ViewStateConcordance::redo_concordance()
{
	mem_engine::search_match_container matches ;
	m_model->perform_search( matches, m_search_matches->m_params ) ;
	m_search_matches->set_matches( matches ) ;
}

void ViewStateConcordance::set_search_matches( mem_engine::felix_query *search_matches )
{
	m_search_matches = search_matches ;
}

int ViewStateConcordance::get_edit_record_title()
{
	if (m_search_matches->empty())
	{
		return IDS_ADD_ENTRY;
	}
	return IDS_EDIT_RECORD_TITLE ;
}

void ViewStateConcordance::set_current( size_t num )
{
	m_search_matches->set_current(num) ;
}

void ViewStateConcordance::delete_match( size_t index )
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

size_t ViewStateConcordance::get_current()
{
	return m_search_matches->current_pos() ;
}
//////////////////////////////////////////////////////////////////////////
// ViewStateConcordanceMain
//////////////////////////////////////////////////////////////////////////

void ViewStateConcordanceMain::handle_toggle_edit_mode()
{
	if ( ! m_view->is_edit_mode() ) // we are entering edit mode
	{
		// user feedback
		m_window_listener->user_feedback( IDS_ENTERING_EDIT_MODE ) ;

		m_view->handle_enter_edit_mode_concordance(m_search_matches) ;

		m_window_listener->user_feedback( IDS_IN_EDIT_MODE ) ;
	}
	else
	{
		m_window_listener->user_feedback( IDS_LEAVING_EDIT_MODE ) ;

		if( false == m_view->handle_leave_edit_mode_concordance( m_model->get_memories(), m_search_matches ) )
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

void ViewStateConcordanceMain::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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
	this->redo_concordance() ;
	m_window_listener->user_feedback( IDS_ADDED_TRANSLATION ) ;
}

void ViewStateConcordanceMain::show_content()
{
	if ( m_search_matches->size() == 0 )
	{
		wstring content = wstring(L"<b>")
			+ R2WSTR( IDS_SEARCH_RESULTS ) 
			+ L":</b><br />";
		content += (LPCWSTR)system_message_w( IDS_FOUND_X_MATCHES, L"0" ) ;
		m_view->set_text(content) ;
	}
	else
	{
		m_view->set_text(m_search_matches->get_html_long()) ;
	}

	m_window_listener->check_mousewheel() ;
}

mem_engine::search_match_ptr ViewStateConcordanceMain::get_current_match()
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

void ViewStateConcordanceMain::activate()
{
	m_window_listener->set_menu_checkmark(ID_VIEW_MATCH, false);
	m_window_listener->set_menu_checkmark(ID_VIEW_SEARCH, true);
}
//////////////////////////////////////////////////////////////////////////
// ViewStateConcordanceGloss
//////////////////////////////////////////////////////////////////////////
void ViewStateConcordanceGloss::handle_toggle_edit_mode()
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

void ViewStateConcordanceGloss::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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

void ViewStateConcordanceGloss::show_content()
{
	wstring html_content ;

	if ( m_search_matches->size() == 0 )
	{
		CStringW msg = system_message_w( IDS_FOUND_X_MATCHES, int_arg_w( 0 ) ) ;
		html_content << L"<p>" <<  static_cast< LPCWSTR >( msg )  << L"</p>" ;
	}
	else
	{
		m_search_matches->set_start_numbering(m_properties_gloss->m_data.m_numbering) ;
		html_content = m_search_matches->get_html_long() ;
	}


	// give the user feedback
	CString status_text = system_message
		( 
		IDS_FOUND_X_MATCHES_FOR_STRING, 
		int_arg( m_search_matches->size() ), 
		CString( m_search_matches->get_source_plain().c_str() )
		) ;
	m_window_listener->user_feedback( status_text ) ;


	m_view->set_text( html_content ) ;
	m_window_listener->check_mousewheel() ;
	m_view->set_scroll_pos(0) ;
}

mem_engine::search_match_ptr ViewStateConcordanceGloss::get_current_match()
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