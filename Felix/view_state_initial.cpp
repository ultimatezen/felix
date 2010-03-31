#include "stdafx.h"
#include "view_state_initial.h"
#include "text_templates.h"

void ViewStateInitial::handle_toggle_edit_mode()
{
	m_view->put_edit_mode( ! m_view->is_edit_mode() ) ;
}

void ViewStateInitial::show_content()
{
	m_view->ensure_document_complete() ;

	const CString filename = text_tmpl::get_template_filename(_T("start.html")) ;
	m_view->navigate(filename) ;

	m_view->ensure_document_complete() ;
}