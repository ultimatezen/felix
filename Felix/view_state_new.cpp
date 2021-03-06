#include "stdafx.h"
#include "view_state_new.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
#include "text_templates.h"
#include "Exceptions.h"
#include "record_local.h"
#include "system_message.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateNew
//////////////////////////////////////////////////////////////////////////



void ViewStateNew::retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add)
{
	mem_engine::memory_pointer mem = m_model->get_memory_by_id(mem_id) ;
	if (is_add)
	{
		mem->add_record(new_rec) ;
	}
	else
	{
		const mem_engine::record_pointer old_rec = m_window_listener->get_item_under_edit()->get_record() ;
		mem->replace(old_rec, new_rec) ;
	}

	m_window_listener->set_new_record(new_rec) ;
	m_window_listener->user_feedback( IDS_ADDED_TRANSLATION ) ;
}

void ViewStateNew::delete_match( size_t index )
{
	index ;
	ATLASSERT( index == 0 ) ;
	memory_pointer mem = m_model->get_first_memory() ;

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

		ATLASSERT ( m_model->empty() == false ) ; 
		memory_pointer mem = m_model->get_first_memory() ;
		try
		{
			m_model->get_memory_by_id(mem->get_id())->erase(new_rec) ;
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

	cpptempl::data_map record ;

	record[L"num0"] = cpptempl::make_data(ulong2wstring(0u)) ;
	record[L"num"] = cpptempl::make_data(ulong2wstring(1u)) ;
	record[L"source"] = cpptempl::make_data(new_rec->get_source_rich()) ;
	record[L"trans"] = cpptempl::make_data(new_rec->get_trans_rich()) ;
	record[L"context"] = cpptempl::make_data(new_rec->get_context_rich()) ;
	record[L"created"] = cpptempl::make_data(new_rec->get_created().get_date_time_string()) ;
	record[L"modified"] = cpptempl::make_data(new_rec->get_modified().get_date_time_string()) ;
	record[L"reliability"] = cpptempl::make_data(ulong2wstring(new_rec->get_reliability())) ;
	record[L"validated"] = cpptempl::make_data(bool2wstring(new_rec->is_validated())) ;

	record[L"creator"] = cpptempl::make_data(new_rec->get_creator()) ;
	record[L"modified_by"] = cpptempl::make_data(new_rec->get_modified_by()) ;
	record[L"refcount"] = cpptempl::make_data(ulong2wstring(new_rec->get_refcount())) ;
	record[L"ref_count"] = cpptempl::make_data(ulong2wstring(new_rec->get_refcount())) ;

	data[L"record"] = cpptempl::make_data(record) ;


	cpptempl::data_list qc_messages ;
	if (m_props && m_props->live_checking())
	{
		std::vector<wstring> messages ;
		m_window_listener->get_qc_messages(new_rec, messages) ;
		FOREACH(wstring message, messages)
		{
			qc_messages.push_back(cpptempl::make_data(message)) ;
		}
	}
	data[L"qc_messages"] = cpptempl::make_data(qc_messages) ;

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
		if( false == m_view->handle_leave_edit_mode_new_record_glossary(m_model, record) )
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

	cpptempl::data_map record ;

	record[L"num0"] = cpptempl::make_data(ulong2wstring(0u)) ;
	record[L"num"] = cpptempl::make_data(ulong2wstring(1u)) ;
	record[L"source"] = cpptempl::make_data(new_rec->get_source_rich()) ;
	record[L"trans"] = cpptempl::make_data(new_rec->get_trans_rich()) ;
	record[L"context"] = cpptempl::make_data(new_rec->get_context_rich()) ;
	record[L"created"] = cpptempl::make_data(new_rec->get_created().get_date_time_string()) ;
	record[L"modified"] = cpptempl::make_data(new_rec->get_modified().get_date_time_string()) ;
	record[L"reliability"] = cpptempl::make_data(ulong2wstring(new_rec->get_reliability())) ;
	record[L"validated"] = cpptempl::make_data(bool2wstring(new_rec->is_validated())) ;

	record[L"creator"] = cpptempl::make_data(new_rec->get_creator()) ;
	record[L"modified_by"] = cpptempl::make_data(new_rec->get_modified_by()) ;
	record[L"refcount"] = cpptempl::make_data(ulong2wstring(new_rec->get_refcount())) ;
	record[L"ref_count"] = cpptempl::make_data(ulong2wstring(new_rec->get_refcount())) ;

	data[L"record"] = cpptempl::make_data(record) ;

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