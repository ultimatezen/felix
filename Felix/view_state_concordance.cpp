#include "stdafx.h"
#include "view_state_concordance.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "system_message.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateConcordance
//////////////////////////////////////////////////////////////////////////

void ViewStateConcordance::handle_toggle_edit_mode()
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

		ATLTRACE(" ... Showing view content\n") ;
		this->show_content() ;

		ATLTRACE(" ... Giving user feedback content\n") ;
		m_window_listener->user_feedback( IDS_LEFT_EDIT_MODE ) ;
	}
}

void ViewStateConcordance::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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

void ViewStateConcordance::redo_concordance()
{
	mem_engine::search_match_container matches ;
	m_model->perform_search( matches, m_search_matches->m_params ) ;
	m_search_matches->set_matches( matches ) ;
}
//////////////////////////////////////////////////////////////////////////
// ViewStateConcordanceMain
//////////////////////////////////////////////////////////////////////////

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
//////////////////////////////////////////////////////////////////////////
// ViewStateConcordanceGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateConcordanceGloss::show_content()
{

}