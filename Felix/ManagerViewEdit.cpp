#include "stdafx.h"
#include "ManagerViewEdit.h"
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

		cpptempl::data_map data ;

		set_template_data(data);

		wstring tpl_text = cpptempl::get_template_text(_T("manager/edit.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;

		m_view->ensure_document_complete() ;
		m_view->ensure_navigation_complete() ;

		m_view->run_script("focusItem", "creator") ;

	}

	void ManagerViewEdit::set_template_data( cpptempl::data_map &data )
	{
		if(m_is_memory)
		{
			m_model = m_mem_model ;
		}
		else
		{
			m_model = m_gloss_model ;
		}
		mem_engine::memory_pointer mem = m_model->memory_at(m_item) ;
		set_mem_tmpl_info(mem, data, m_is_memory, m_item);
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
	}
}