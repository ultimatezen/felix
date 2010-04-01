#include "stdafx.h"
#include "view_state_new.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Exceptions.h"

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

void ViewStateNew::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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
//////////////////////////////////////////////////////////////////////////
// ViewStateNewGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateNewGloss::show_content()
{
	m_view->ensure_document_complete() ;

	record_pointer new_rec = m_window_listener->get_new_record() ;
	wstring html_content ;
	html_content << L"<b>" << R2W( IDS_MSG_ADDED_GLOSS_ENTRY_TITLE ) << L"</b>" ;
	// source & trans
	html_content << L"<table class=\"added\" id=\"0\">\n" 
		<< L"<tr><td><b>" << R2W( IDS_SOURCE_TITLE ) << L"</b></td><td class=\"match_content\" id=\"" << R2W( IDS_SOURCE_ID ) << L"\">" << new_rec->get_source_rich() << L"</td></tr>"
		<< L"<tr><td><b>" << R2W( IDS_TRANS_TITLE )  << L"</b></td><td class=\"match_content\" id=\"" << R2W( IDS_TRANS_ID ) << L"\">" << new_rec->get_trans_rich() << L"</td></tr>"
		<< L"<tr><td><b>" << R2W( IDS_CONTEXT_TITLE ) << L"</b></td>" ;
	// context
	if ( new_rec->get_context_plain().empty() )
		html_content << L"<td class=\"match_content\" id=\"context_tmp\"><b><font color=\"red\">" << R2W( IDS_NO_CONTEXT ) << L"</font></b>" ;
	else
		html_content << L"<td class=\"match_content\" id=\"" << R2W( IDS_CONTEXT_ID ) << L"\">" << new_rec->get_context_rich() ;
	html_content << L"</td></tr>\n" ;
	html_content << L"</table>" ;
	// links
	html_content	<< L"<p><hr>"
		<< L"<a href=\"" << int2wstring( IDC_EDIT ) << L":0\" title=\"" << R2W( IDS_EDIT_TITLE ) << L"\">" << R2W( IDS_EDIT ) << L"</a>" 
		<< L" | <a href=\"" << int2wstring( IDC_DELETE ) << L":0\" title=\"" << R2W( IDS_DELETE_TITLE) << L"\">" << R2W( IDS_DELETE ) << L"</a>"
		<< L" | <a href=\"" << int2wstring( IDC_ADD ) << L":0\" title=\"" << R2W( IDS_ADD2MEM_TITLE ) << L"\">" << R2W( IDS_ADD_TO_MEMORY ) << L"</a>" ;

	html_content << L"<p> <a href=\"" << int2wstring( IDC_PREV ) << L":0\">" << R2W( IDS_BACK ) << L"</a>" ;

	m_view->set_text( html_content ) ;
	TRACE(html_content) ;
	m_view->set_scroll_pos(0) ;
	m_window_listener->check_mousewheel() ;
	m_view->ensure_document_complete() ;
}