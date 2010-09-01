#include "stdafx.h"
#include "ManagerViewDetails.h"
#include "TextTemplate.h"
#include "SearchWindow.h"

namespace mgrview
{
	void ManagerViewDetails::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewDetails::show_content()
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
		engine.Assign("message", m_window_listener->get_message()) ;
		set_mem_tmpl_info(mem, engine, m_is_memory, m_item);

		wstring tpl_text = text_tmpl::get_template_text(_T("manager/view.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}
}