#include "stdafx.h"
#include "view_state_concordance.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateConcordance
//////////////////////////////////////////////////////////////////////////

void ViewStateConcordance::handle_toggle_edit_mode()
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

		record_pointer edit_rec = m_window_listener->get_new_record() ;
		m_view->handle_leave_edit_mode_new(edit_rec) ;
		m_window_listener->user_feedback( IDS_LEFT_EDIT_MODE ) ;

		record_pointer new_rec = m_window_listener->get_new_record() ;
		if ( new_rec->is_valid_record() ) 
		{
			this->show_content() ;
			return ;
		}

		ATLASSERT ( m_model->get_memories()->empty() == false ) ; 
		memory_pointer mem = m_model->get_memories()->get_first_memory() ;
		try
		{
			m_model->get_memories()->remove_record( new_rec, mem->get_id() ) ;
			wstring content ; 
			content << L"<center><h1>" << resource_string_w( IDS_DELETED_ENTRY ) << L"</h1></center>" ;

			m_view->set_text( content ) ;
			m_window_listener->check_mousewheel() ;
			m_view->set_scroll_pos(0) ;
		}
		catch (except::CProgramException& e)
		{
			logging::log_exception(e) ;
			e.notify_user("Failed to delete record: memory not found") ;
		}

		return ;
	}
}

void ViewStateConcordance::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
{
	mem_engine::memory_pointer mem = m_model->get_memory_by_id(mem_id) ;
	const mem_engine::record_pointer old_rec = m_window_listener->get_new_record() ;
	if (old_rec->is_valid_record())
	{
		mem->replace(old_rec, new_rec) ;
	}
	else
	{
		mem->add_record(new_rec) ;
	}
	m_window_listener->set_new_record(new_rec) ;
	m_window_listener->user_feedback( IDS_ADDED_TRANSLATION ) ;
}

//////////////////////////////////////////////////////////////////////////
// ViewStateConcordanceMain
//////////////////////////////////////////////////////////////////////////

void ViewStateConcordanceMain::show_content()
{

}
//////////////////////////////////////////////////////////////////////////
// ViewStateConcordanceGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateConcordanceGloss::show_content()
{

}