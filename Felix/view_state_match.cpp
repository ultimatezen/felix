#include "stdafx.h"
#include "view_state_match.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
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

	erase_by_index(*this, index) ;

	deleted_match_feedback(*this) ;
}

size_t ViewStateMatch::get_current()
{
	return m_search_matches->current_pos() ;
}

ViewStateMatch::ViewStateMatch() : m_search_matches(NULL), m_should_scroll(true)
{

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

		m_view->handle_leave_edit_mode_match( m_model, m_search_matches ) ;

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

void ViewStateMatchMain::retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add)
{
	mem_engine::memory_pointer mem = m_model->get_memory_by_id(mem_id) ;
	mem_engine::search_match_ptr current_match = m_window_listener->get_item_under_edit() ;
	ATLASSERT( mem_id == current_match->get_memory_id() ) ;
	const mem_engine::record_pointer old_rec = current_match->get_record() ;
	if (! is_add)
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
	const wstring content = get_view_content();

	m_view->set_text( content ) ;
	m_window_listener->check_mousewheel() ;

	m_view->ensure_document_complete() ;
	if ( m_window_listener->is_single_page() ) 
	{
		const wstring current_id = ulong2wstring( m_search_matches->current_pos() ) ;
		m_view->scroll_element_into_view( current_id ) ;
	}
	else if (m_should_scroll)
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

void ViewStateMatchMain::set_props( app_props::props_ptr props )
{
	m_props = props ;
}

void ViewStateMatchMain::set_gloss_matches( mem_engine::felix_query *matches )
{
	m_gloss_matches = matches ;
}

wstring ViewStateMatchMain::get_view_content()
{
	const static wstring GlossMatchFmt( L"<span class=\"gloss_match\">%s</span>" ) ;

	const wstring old_source = m_search_matches->m_params.get_source_rich() ;
	wstring content ;

	// If there are no matches and we are marking up gloss matches,
	// set up the source query so that gloss matches are marked up
	if(this->m_search_matches->empty() && 
		this->m_gloss_matches && 
		this->m_props->m_view_props.m_data.m_show_gloss_matches)
	{
		wstring source = old_source ;
		mem_engine::gloss_match_set gloss_sources ;
		for(size_t i = 0 ; i < this->m_gloss_matches->size() ; ++i)
		{
			search_match_ptr gloss_match = this->m_gloss_matches->at(i) ;
			const wstring gloss_source = gloss_match->get_record()->get_source_plain() ;
			gloss_sources.insert(boost::trim_copy(gloss_source)) ;
		}
		FOREACH(wstring gloss, gloss_sources)
		{
			const wstring marked_up = (wformat(GlossMatchFmt) % gloss).str() ;
			source = boost::replace_all_copy(source, gloss, marked_up) ;
		}
		this->m_search_matches->m_params.set_source(source) ;
	}

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
	m_search_matches->m_params.set_source(old_source) ;
	return content ;
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

		if( false == m_view->handle_leave_edit_mode_concordance_glossary( m_model, m_search_matches ) )
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

void ViewStateMatchGloss::retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add)
{
	memory_pointer mem ;
	try
	{
		mem = m_model->get_memory_by_id(mem_id) ;
	}
	catch (except::CProgramException &e )
	{
		logging::log_exception(e) ;
		mem = m_model->get_first_memory() ;
	}
	mem_engine::search_match_ptr current_match = m_window_listener->get_item_under_edit() ;
	if (is_add)
	{
		mem->add_record(new_rec) ;
	}
	else
	{
		ATLASSERT( mem_id == current_match->get_memory_id() ) ;
		const mem_engine::record_pointer old_rec = current_match->get_record() ;
		mem->replace(old_rec, new_rec) ;
	}

	current_match->set_record(new_rec) ;
	current_match->set_values_to_record() ;
	m_window_listener->set_new_record(new_rec) ;
	m_window_listener->user_feedback( IDS_ADDED_TRANSLATION ) ;
}

void ViewStateMatchGloss::show_content()
{
	wstring html_content = get_match_html_content();

	m_view->set_text(html_content) ;
	if (m_should_scroll)
	{
		m_view->set_scroll_pos(0) ;
	}
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


wstring ViewStateMatchGloss::get_match_html_content()
{
	if ( m_search_matches->empty() )
	{
		wstring p = L"<p>" ;
		wstring pe = L"</p>" ;
		return p + static_cast<LPCWSTR>(system_message_w( IDS_FOUND_X_MATCHES, int_arg_w(0) )) +  pe ;
	}
	else
	{
		m_search_matches->m_simple_view = m_properties_gloss->get_simple_view() ;
		return m_search_matches->get_html_short() ;
	}
}

mem_engine::search_match_ptr ViewStateMatchGloss::get_current_match()
{
	if (m_search_matches->empty())
	{
		return get_query_as_match();
	}
	return m_search_matches->current( ) ;
}

mem_engine::search_match_ptr ViewStateMatchGloss::get_query_as_match()
{
	record_pointer rec(new mem_engine::record_local) ;
	rec->set_source( m_search_matches->get_query_rich() ) ;
	search_match_ptr match = this->m_model->get_first_memory()->make_match(rec) ;
	match->set_values_to_record() ;
	return match ;
}

void ViewStateMatchGloss::set_app_props( app_props::properties_glossary *properties )
{
	m_properties_gloss = properties ;
}

void ViewStateMatchGloss::activate()
{

}
