#include "stdafx.h"
#include "view_state_review.h"
#include "text_templates.h"
#include "logging.h"
#include "resource_string.h"
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

		return ;
	}
}

void ViewStateReview::retrieve_edit_record(size_t mem_id, mem_engine::record_pointer new_rec, bool is_add)
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
	m_window_listener->set_review_match(new_rec, mem_id) ;
	m_window_listener->redo_lookup(current_match, true) ;
	m_window_listener->user_feedback( IDS_CORRECTED_TRANS ) ;
}

void ViewStateReview::show_content()
{
	memory_pointer mem = m_model->get_first_memory() ;

	mem_engine::search_match_ptr match(mem->make_match(m_window_listener->get_review_match()->get_record())) ;
	match->set_values_to_record() ;

	cpptempl::data_map data ;
	m_search_matches->fill_match_template_params(data, match);

	// fill in the template
	wstring content ;
	if ( m_window_listener->is_short_format() )
	{
		content = cpptempl::parse(cpptempl::get_template_text(_T("review.txt")), data) ;
	}
	else
	{
		content = cpptempl::parse(cpptempl::get_template_text(_T("review_full.txt")), data) ;
	}
	m_view->set_text(content) ;
	m_view->set_scroll_pos(0) ;
	m_window_listener->set_bg_color_if_needed() ;
}

mem_engine::search_match_ptr ViewStateReview::get_current_match()
{
	search_match_ptr match = this->m_model->get_first_memory()->make_match(this->m_window_listener->get_review_match()->get_record()) ;
	match->set_values_to_record() ;
	return match ;
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
	memory_pointer mem = m_model->get_first_memory() ;

	if ( ! m_window_listener->check_delete() )
	{
		return ;
	}
	mem->erase(m_window_listener->get_review_match()->get_record()) ;

	const wstring feedback = L"<center><h1>" + resource_string_w( IDS_DELETED_ENTRY ) + L"</h1></center>" ;
	m_view->set_text(feedback) ;
	m_window_listener->user_feedback(IDS_DELETED_ENTRY) ;
	m_window_listener->check_mousewheel() ;
	m_view->set_scroll_pos(0) ;
}

size_t ViewStateReview::get_current()
{
	return 0u ;
}