#include "stdafx.h"
#include "ManagerViewActions.h"
#include "TextTemplate.h"
#include "SearchWindow.h"
#include "cpptempl.h"

namespace mgrview
{
	void ManagerViewActions::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewActions::show_content()
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

		cpptempl::data_map data ;

		mem_engine::memory_pointer mem = m_model->memory_at(m_item) ;
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
		set_mem_tmpl_info(mem, data, m_is_memory, m_item);

		wstring tpl_text = cpptempl::get_template_text(_T("manager/actions.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;
	}
}