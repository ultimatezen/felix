#include "stdafx.h"
#include "ManagerViewBrowse.h"
#include "TextTemplate.h"
#include "numberfmt.h"
#include "SearchWindow.h"

namespace mgrview
{


	void ManagerViewBrowse::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewBrowse::show_content()
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

		CNumberFmt number_format ;
		wstring memsize = (LPCWSTR)(number_format.Format(mem->size())) ;
		engine.Assign("size", memsize) ;

		engine.Assign("name", get_memname(mem)) ;
		engine.Assign("page", tows(m_page)) ;
		engine.Assign("memtype", m_is_memory ? L"mem" : L"gloss") ;
		engine.Assign("index", tows(m_item)) ;

		wstring tpl_text = text_tmpl::get_template_text(_T("manager/browse.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}
}