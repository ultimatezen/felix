#include "stdafx.h"
#include "view_state_initial.h"
#include "text_templates.h"

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

//////////////////////////////////////////////////////////////////////////
// ViewStateInitialMain
//////////////////////////////////////////////////////////////////////////

void ViewStateInitialMain::show_content()
{
	m_view->ensure_document_complete() ;

	const CString filename = text_tmpl::get_template_filename(_T("start.html")) ;
	m_view->navigate(filename) ;

	m_view->ensure_document_complete() ;
}
//////////////////////////////////////////////////////////////////////////
// ViewStateInitialGloss
//////////////////////////////////////////////////////////////////////////

void ViewStateInitialGloss::show_content()
{
	m_view->ensure_document_complete() ;

	const CString filename = text_tmpl::get_template_filename(_T("start_gloss.html")) ;
	m_view->navigate(filename) ;

	m_view->ensure_document_complete() ;
	m_window_listener->set_bg_color_if_needed() ;
}