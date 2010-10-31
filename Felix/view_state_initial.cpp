#include "stdafx.h"
#include "view_state_initial.h"
#include "text_templates.h"
#include "record_local.h"

//////////////////////////////////////////////////////////////////////////
// ViewStateInitial
//////////////////////////////////////////////////////////////////////////

void ViewStateInitial::handle_toggle_edit_mode()
{
	m_view->put_edit_mode( ! m_view->is_edit_mode() ) ;
}

void ViewStateInitial::retrieve_edit_record( int mem_id, mem_engine::record_pointer new_rec )
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

int ViewStateInitial::get_edit_record_title()
{
	return IDS_ADD_ENTRY;
}

void ViewStateInitial::delete_match( size_t index )
{
	index ;
#ifndef UNIT_TEST
	MessageBeep(MB_ICONEXCLAMATION) ;
#endif
	m_window_listener->user_feedback(IDS_NO_MATCHES);
}
//////////////////////////////////////////////////////////////////////////
// ViewStateInitialMain
//////////////////////////////////////////////////////////////////////////

void ViewStateInitialMain::show_content()
{
	m_view->ensure_document_complete() ;

	const CString filename = cpptempl::get_template_filename(_T("start.html")) ;
	m_view->navigate(filename) ;

	m_view->ensure_document_complete() ;
}

mem_engine::search_match_ptr ViewStateInitialMain::get_current_match()
{
	record_pointer rec(new mem_engine::record_local) ;
	search_match_ptr match(new search_match(rec)) ;
	match->set_values_to_record() ;
	match->set_memory_id(m_model->get_first_mem_id()) ;

	return match ;
}

void ViewStateInitialMain::activate()
{
	m_window_listener->set_menu_checkmark(ID_VIEW_MATCH, false);
	m_window_listener->set_menu_checkmark(ID_VIEW_SEARCH, false);
}
//////////////////////////////////////////////////////////////////////////
// ViewStateInitialGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateInitialGloss::show_content()
{
	m_view->ensure_document_complete() ;

	const CString filename = cpptempl::get_template_filename(_T("start_gloss.html")) ;
	m_view->navigate(filename) ;

	m_view->ensure_document_complete() ;
	m_window_listener->set_bg_color_if_needed() ;
}

mem_engine::search_match_ptr ViewStateInitialGloss::get_current_match()
{
	record_pointer rec(new mem_engine::record_local) ;
	search_match_ptr match(new search_match(rec)) ;
	match->set_values_to_record() ;
	match->set_memory_id(m_model->get_first_mem_id()) ;

	return match ;
}