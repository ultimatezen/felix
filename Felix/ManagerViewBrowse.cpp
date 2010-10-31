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
	
		text_tmpl::CTextTemplate engine ;
		if(m_is_memory)
		{
			m_model = m_mem_model ;
			engine.Assign(L"memtype", L"mem") ;
		}
		else
		{
			m_model = m_gloss_model ;
			engine.Assign(L"memtype", L"gloss") ;
		}


		mem_engine::memory_pointer mem = m_model->memory_at(m_item) ;

		m_paginator.set_num_records(mem->size()) ;
		m_paginator.set_current_page(m_page-1);

		engine.Assign(L"message", m_message) ;
		m_message.clear() ;
		engine.Assign("name", get_memname(mem)) ;
		// page stuff
		engine.Assign(L"pagination", get_pagination_text(m_paginator)) ;
		engine.Assign(L"page", ulong2wstring(m_paginator.get_current_page()+1)) ;
		engine.Assign(L"index", tows(m_item)) ;

		CNumberFmt number_format ;
		engine.Assign(L"num_pages", wstring((LPCWSTR)(number_format.Format(m_paginator.get_num_pages())))) ;

		text_tmpl::DictListPtr items = engine.CreateDictList();

		for (size_t i = m_paginator.get_start() ; i < m_paginator.get_end() ; ++i)
		{
			text_tmpl::DictPtr item = engine.CreateDict() ;

			mem_engine::record_pointer record = mem->get_record_at(i) ;

			item->insert(std::make_pair(L"num0", tows(i))) ;
			item->insert(std::make_pair(L"num", tows(i+1))) ;
			item->insert(std::make_pair(L"source", record->get_source_rich())) ;
			item->insert(std::make_pair(L"trans", record->get_trans_rich())) ;
			item->insert(std::make_pair(L"context", record->get_context_rich())) ;
			item->insert(std::make_pair(L"created", record->get_created().get_date_time_string())) ;
			item->insert(std::make_pair(L"modified", record->get_modified().get_date_time_string())) ;
			item->insert(std::make_pair(L"reliability", tows(record->get_reliability()))) ;
			item->insert(std::make_pair(L"validated", bool2wstring(record->is_validated()))) ;

			item->insert(std::make_pair(L"creator", record->get_creator())) ;
			item->insert(std::make_pair(L"modified_by", record->get_modified_by())) ;
			// other info
			const wstring filename = (LPCWSTR)mem->get_location() ;
			wstring loc ;
			if ( filename.empty() )
			{
				loc = R2WSTR( IDS_NEW ) ;
			}
			else
			{
				loc = fs::wpath(filename).leaf();		
			}

			item->insert(std::make_pair(L"mem", loc)) ;
			item->insert(std::make_pair(L"memory", loc)) ;
			item->insert(std::make_pair(L"refcount", tows(record->get_refcount()))) ;
			item->insert(std::make_pair(L"ref_count", tows(record->get_refcount()))) ;

			items->push_back(item) ;
		}
		engine.Assign(L"records", items) ;

		wstring tpl_text = cpptempl::get_template_text(_T("manager/browse.txt")) ;
		m_view->set_text(engine.Fetch(tpl_text)) ;
	}

	ManagerViewBrowse::ManagerViewBrowse( size_t item, bool is_memory/*=true*/, size_t page/*=1*/ ) : 
	m_item(item),
		m_page(page),
		m_is_memory(is_memory),
		m_model(NULL),
		m_paginator(10u),
		ManagerView()
	{

	}
}