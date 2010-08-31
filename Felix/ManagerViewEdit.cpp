#include "stdafx.h"
#include "ManagerViewEdit.h"
#include "TextTemplate.h"
#include "SearchWindow.h"
#include "document_wrapper.h"

#ifdef UNIT_TEST
#include "element_wrapper_fake.h"
#include "document_wrapper_fake.h"
#endif

namespace mgrview
{
	void ManagerViewEdit::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewEdit::show_content()
	{
		SENSE("show_content") ;
		if(m_is_memory)
		{
			m_model = m_mem_model ;
		}
		else
		{
			m_model = m_gloss_model ;
		}

		text_tmpl::CTextTemplate engine ;

		mem_engine::memory_pointer mem = m_model->memory_at(m_item) ;
		set_mem_tmpl_info(mem, engine, m_is_memory, m_item);

		wstring tpl_text = text_tmpl::get_template_text(_T("manager/edit.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;

		m_view->ensure_document_complete() ;
		m_view->ensure_navigation_complete() ;

		m_view->run_script("focusItem", "creator") ;

	}
}