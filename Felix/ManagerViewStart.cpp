#include "stdafx.h"
#include "ManagerViewStart.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "numberfmt.h"

namespace mgrview
{


	void ManagerViewStart::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewStart::show_content()
	{
		text_tmpl::CTextTemplate engine ;

		// tms
		wstring memlist = make_mem_list(m_mem_model, L"mem") ;
		engine.Assign("memlist", memlist) ;

		wstring glosslist = make_mem_list(m_gloss_model, L"gloss") ;
		engine.Assign("glosslist", glosslist) ;

		// fill in the template
		wstring tpl_text = text_tmpl::get_template_text(_T("manager/start.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}

	wstring make_mem_list(FelixModelInterface *mems, 
										const wstring memtype )
	{
		text_tmpl::CTextTemplate engine ;

		engine.Assign("memtype", memtype) ;

		text_tmpl::DictListPtr tms = engine.CreateDictList() ;
		for(size_t i = 0 ; i < mems->size() ; ++i)
		{
			mem_engine::memory_pointer mem = mems->memory_at(i) ;

			text_tmpl::DictPtr item = engine.CreateDict() ;
	
			CNumberFmt number_format ;
			wstring memsize = (LPCWSTR)(number_format.Format(mem->size())) ;
			engine.Assign(item, L"size", memsize) ;

			// up/down arrows
			if (i > 0 && mems->size() > 1)
			{
				engine.Assign(item, L"has_up", L"true") ;
			}
			else
			{
				engine.Assign(item, L"has_up", L"") ;
			}
			if (i < mems->size() - 1 && mems->size() > 1)
			{
				engine.Assign(item, L"has_down", L"true") ;
			}
			else
			{
				engine.Assign(item, L"has_down", L"") ;
			}

			engine.Assign(item, L"name", get_memname(mem)) ;

			tms->push_back(item) ;
		}
		engine.Assign("tms", tms) ;

		wstring tpl_text = text_tmpl::get_template_text(_T("manager/datalist.txt")) ;
		return engine.Fetch(tpl_text) ;
	}

	void ManagerViewStartMem::show_content()
	{
		text_tmpl::CTextTemplate engine ;

		// tms
		wstring memlist = make_mem_list(m_mem_model, L"mem") ;
		engine.Assign("memlist", memlist) ;

		// fill in the template
		wstring tpl_text = text_tmpl::get_template_text(_T("manager/startmem.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}
	void ManagerViewStartGloss::show_content()
	{
		text_tmpl::CTextTemplate engine ;

		// tms
		wstring glosslist = make_mem_list(m_gloss_model, L"gloss") ;
		engine.Assign("glosslist", glosslist) ;

		// fill in the template
		wstring tpl_text = text_tmpl::get_template_text(_T("manager/startgloss.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}
}