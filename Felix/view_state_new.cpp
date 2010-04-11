#include "stdafx.h"
#include "view_state_new.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "record_local.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateNew
//////////////////////////////////////////////////////////////////////////

void ViewStateNew::handle_toggle_edit_mode()
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

		record_pointer new_rec = m_window_listener->get_item_under_edit()->get_record() ;
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

void ViewStateNew::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
{
	mem_engine::memory_pointer mem = m_model->get_memory_by_id(mem_id) ;
	const mem_engine::record_pointer old_rec = m_window_listener->get_item_under_edit()->get_record() ;
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
// ViewStateNewMain
//////////////////////////////////////////////////////////////////////////

void ViewStateNewMain::show_content()
{
	m_view->ensure_document_complete() ;

	const wstring tpl_text = text_tmpl::get_template_text("match_new.txt") ;

	record_pointer new_rec = m_window_listener->get_new_record() ;
	text_tmpl::CTextTemplate engine ;
	engine.Assign(L"source", new_rec->get_source_rich()) ;
	engine.Assign(L"trans", new_rec->get_trans_rich()) ;

	m_view->set_text( engine.Fetch(tpl_text) ) ;
	m_window_listener->check_mousewheel() ;

	m_window_listener->set_bg_color_if_needed() ;

	m_view->ensure_document_complete() ;
}

mem_engine::search_match_ptr ViewStateNewMain::get_current_match()
{
	search_match_ptr match(new search_match(m_window_listener->get_new_record())) ;
	match->set_values_to_record() ;
	match->set_base_score(1.0) ;
	return match ;
}
//////////////////////////////////////////////////////////////////////////
// ViewStateNewGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateNewGloss::show_content()
{
	m_view->ensure_document_complete() ;

	record_pointer new_rec = m_window_listener->get_new_record() ;

	const wstring tpl_text = text_tmpl::get_template_text("gloss_match_new.txt") ;

	text_tmpl::CTextTemplate engine ;
	engine.Assign(L"source", new_rec->get_source_rich()) ;
	engine.Assign(L"trans", new_rec->get_trans_rich()) ;

	// context
	if ( new_rec->get_context_plain().empty() )
	{
		engine.Assign(L"context", wstring(L"<b><font color=\"red\">") + R2W( IDS_NO_CONTEXT ) + L"</font></b>") ;
		engine.Assign(L"context_id", L"context_tmp") ;
	}
	else
	{
		engine.Assign(L"context", new_rec->get_context_rich()) ;
		engine.Assign(L"context_id", R2W( IDS_CONTEXT_ID )) ;
	}


	m_view->set_text( engine.Fetch(tpl_text) ) ;

	m_view->ensure_document_complete() ;

	m_window_listener->set_bg_color_if_needed() ;
	m_view->set_scroll_pos(0) ;
	m_window_listener->check_mousewheel() ;
}

mem_engine::search_match_ptr ViewStateNewGloss::get_current_match()
{
	search_match_ptr match(new search_match(record_pointer(new mem_engine::record_local))) ;
	return match ;
}