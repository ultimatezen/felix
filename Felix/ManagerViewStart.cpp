#include "stdafx.h"
#include "ManagerViewStart.h"
#include "TextTemplate.h"
#include "text_templates.h"
#include "Path.h"

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
		text_tmpl::DictListPtr tms = engine.CreateDictList() ;
		for(size_t i = 0 ; i < m_mem_model->size() ; ++i)
		{
			mem_engine::memory_pointer mem = m_mem_model->memory_at(i) ;

			text_tmpl::DictPtr item = engine.CreateDict() ;
			item->insert(std::make_pair(L"memtype", L"mem")) ;

			file::CPath path(mem->get_location()) ;
			path.StripPath() ;
			path.RemoveExtension() ;
			item->insert(std::make_pair(L"name", wstring((LPCWSTR)path.Path()))) ;

			tms->push_back(item) ;
		}

		engine.Assign(L"tms", tms) ;

		// glossaries
		text_tmpl::DictListPtr glossaries = engine.CreateDictList() ;
		for(size_t i = 0 ; i < m_gloss_model->size() ; ++i)
		{
			mem_engine::memory_pointer mem = m_gloss_model->memory_at(i) ;

			text_tmpl::DictPtr item = engine.CreateDict() ;
			item->insert(std::make_pair(L"memtype", L"gloss")) ;
			file::CPath path(mem->get_location()) ;
			path.RemoveExtension() ;
			path.StripPath() ;
			item->insert(std::make_pair(L"name", wstring((LPCWSTR)path.Path()))) ;


			glossaries->push_back(item) ;
		}

		engine.Assign(L"glossaries", glossaries) ;


		// fill in the template
		wstring tpl_text = text_tmpl::get_template_text(_T("manager/start.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}
}