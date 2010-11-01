#include "stdafx.h"
#include "view_state_new.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "record_local.h"
#include "system_message.h"
#include "cpptempl.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateNew
//////////////////////////////////////////////////////////////////////////



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

void ViewStateNew::delete_match( size_t index )
{
	index ;
	ATLASSERT( index == 0 ) ;
	memory_pointer mem = m_model->get_memories()->get_first_memory() ;

	if ( ! m_window_listener->check_delete() )
	{
		return ;
	}
	mem->erase(m_window_listener->get_new_record()) ;

	const wstring feedback = L"<center><h1>" + resource_string_w( IDS_DELETED_ENTRY ) + L"</h1></center>" ;
	m_view->set_text(feedback) ;
	m_window_listener->user_feedback(IDS_DELETED_ENTRY) ;
	m_window_listener->check_mousewheel() ;
	m_view->set_scroll_pos(0) ;
}
//////////////////////////////////////////////////////////////////////////
// ViewStateNewMain
//////////////////////////////////////////////////////////////////////////
void ViewStateNewMain::handle_toggle_edit_mode()
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

		record_pointer new_rec = m_window_listener->get_new_record() ;
		m_view->handle_leave_edit_mode_new(new_rec) ;
		m_window_listener->user_feedback( IDS_LEFT_EDIT_MODE ) ;

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
void ViewStateNewMain::show_content()
{
	m_view->ensure_document_complete() ;

	const wstring tpl_text = cpptempl::get_template_text("match_new.txt") ;

	record_pointer new_rec = m_window_listener->get_new_record() ;
	cpptempl::data_map data ;
	data[L"source"] = cpptempl::make_data(new_rec->get_source_rich()) ;
	data[L"trans"] = cpptempl::make_data(new_rec->get_trans_rich()) ;

	m_view->set_text( cpptempl::parse(tpl_text, data) ) ;
	m_window_listener->check_mousewheel() ;

	m_window_listener->set_bg_color_if_needed() ;

	m_view->ensure_document_complete() ;
}

mem_engine::search_match_ptr ViewStateNewMain::get_current_match()
{
	search_match_ptr match(new search_match(m_window_listener->get_new_record())) ;
	match->set_values_to_record() ;
	match->set_base_score(1.0) ;
	match->set_memory_id(m_model->get_first_mem_id()) ;
	return match ;
}

void ViewStateNewMain::activate()
{
	m_window_listener->set_menu_checkmark(ID_VIEW_MATCH, false);
	m_window_listener->set_menu_checkmark(ID_VIEW_SEARCH, false);
}
//////////////////////////////////////////////////////////////////////////
// ViewStateNewGloss
//////////////////////////////////////////////////////////////////////////
void ViewStateNewGloss::handle_toggle_edit_mode()
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

		record_pointer record = m_window_listener->get_new_record() ;
		if( false == m_view->handle_leave_edit_mode_new_record_glossary( m_model->get_memories(), 
																		record) )
		{
			m_view->set_text( R2WSTR( IDS_POST_EDIT_ALL_DELETED ) ) ;
			m_window_listener->check_mousewheel() ;
			m_view->set_scroll_pos(0) ;
			m_window_listener->user_feedback( IDS_DELETED_ENTRY ) ;
			m_window_listener->set_new_record(record_pointer(new mem_engine::record_local)) ;
#ifndef UNIT_TEST
			::MessageBeep( MB_ICONINFORMATION ) ;
#endif
		}
		else
		{
			memory_pointer mem = m_model->get_first_memory() ;
			
			if ( !  mem->add_record( m_window_listener->get_new_record() ) )
			{
				if ( mem->is_locked() )
				{
					m_window_listener->user_feedback( IDS_GLOSSARY_LOCKED ) ;
					return ;
				}
				else
				{
					ATLASSERT( mem->record_exists( m_window_listener->get_new_record() ) ) ;
					m_window_listener->user_feedback( IDS_ENTRY_EXISTED ) ; // Entry already existed. Updated with any new information. 
				}
			}
			else
			{
				CString content = system_message( IDS_CURRENT_SIZE, R2W(IDS_GLOSSARY), int_arg( mem->size() ) ) ;
				m_window_listener->user_feedback( content ) ;
			}

			m_window_listener->user_feedback( IDS_LEFT_EDIT_MODE ) ;
		}

		this->show_content() ;
		return ;
	}
}
void ViewStateNewGloss::show_content()
{
	m_view->ensure_document_complete() ;

	record_pointer new_rec = m_window_listener->get_new_record() ;

	const wstring tpl_text = cpptempl::get_template_text("gloss_match_new.txt") ;

	cpptempl::data_map data ;
	data[L"source"] = cpptempl::make_data(new_rec->get_source_rich()) ;
	data[L"trans"] = cpptempl::make_data(new_rec->get_trans_rich()) ;

	// context
	if ( new_rec->get_context_plain().empty() )
	{
		data[L"context"] = cpptempl::make_data(wstring(L"<b><font color=\"red\">") + R2W( IDS_NO_CONTEXT ) + L"</font></b>") ;
		data[L"context_id"] = cpptempl::make_data(L"context_tmp") ;
	}
	else
	{
		data[L"context"] = cpptempl::make_data(new_rec->get_context_rich()) ;
		data[L"context_id"] = cpptempl::make_data(R2W( IDS_CONTEXT_ID )) ;
	}


	m_view->set_text( cpptempl::parse(tpl_text, data) ) ;

	m_view->ensure_document_complete() ;

	m_window_listener->set_bg_color_if_needed() ;
	m_view->set_scroll_pos(0) ;
	m_window_listener->check_mousewheel() ;
}

mem_engine::search_match_ptr ViewStateNewGloss::get_current_match()
{
	search_match_ptr match(new search_match(m_window_listener->get_new_record())) ;
	match->set_values_to_record() ;
	match->set_base_score(1.0) ;
	match->set_memory_id(m_model->get_first_mem_id()) ;
	return match ;
}

void ViewStateNewGloss::activate()
{

}