#include "stdafx.h"
#include "ManagerViewBrowse.h"
#include "numberfmt.h"
#include "SearchWindow.h"
#include "cpptempl.h"

namespace mgrview
{


	void ManagerViewBrowse::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewBrowse::show_content()
	{
		SENSE("show_content") ;
	
		cpptempl::data_map data ;
		set_template_data(data);


		wstring tpl_text = cpptempl::get_template_text(_T("manager/browse.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;
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

	void ManagerViewBrowse::set_template_data( cpptempl::data_map &data )
	{
		if(m_is_memory)
		{
			m_model = m_mem_model ;
			data[L"memtype"] = cpptempl::make_data(L"mem") ;
		}
		else
		{
			m_model = m_gloss_model ;
			data[L"memtype"] = cpptempl::make_data(L"gloss") ;
		}

		mem_engine::memory_pointer mem = m_model->memory_at(m_item) ;

		m_paginator.set_num_records(mem->size()) ;
		m_paginator.set_current_page(m_page-1);

		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
		data[L"name"] = cpptempl::make_data(get_memname(mem)) ;
		// page stuff
		data[L"pagination"] = cpptempl::make_data(get_pagination_text(m_paginator)) ;
		data[L"page"] = cpptempl::make_data(ulong2wstring(m_paginator.get_current_page()+1)) ;
		data[L"index"] = cpptempl::make_data(tows(m_item)) ;

		CNumberFmt number_format ;
		data[L"num_pages"] = cpptempl::make_data(fmt_num(m_paginator.get_num_pages())) ;

		cpptempl::data_list items ;

		for (size_t i = m_paginator.get_start() ; i < m_paginator.get_end() ; ++i)
		{
			cpptempl::data_map item ;
			mem_engine::record_pointer record = mem->get_record_at(i) ;

			item[L"num0"] = cpptempl::make_data(tows(i)) ;
			item[L"num"] = cpptempl::make_data(tows(i+1)) ;
			item[L"source"] = cpptempl::make_data(record->get_source_rich()) ;
			item[L"trans"] = cpptempl::make_data(record->get_trans_rich()) ;
			item[L"context"] = cpptempl::make_data(record->get_context_rich()) ;
			item[L"created"] = cpptempl::make_data(record->get_created().get_date_time_string()) ;
			item[L"modified"] = cpptempl::make_data(record->get_modified().get_date_time_string()) ;
			item[L"reliability"] = cpptempl::make_data(tows(record->get_reliability())) ;
			item[L"validated"] = cpptempl::make_data(bool2wstring(record->is_validated())) ;

			item[L"creator"] = cpptempl::make_data(record->get_creator()) ;
			item[L"modified_by"] = cpptempl::make_data(record->get_modified_by()) ;
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

			item[L"mem"] = cpptempl::make_data(loc) ;
			item[L"memory"] = cpptempl::make_data(loc) ;
			item[L"refcount"] = cpptempl::make_data(tows(record->get_refcount())) ;
			item[L"ref_count"] = cpptempl::make_data(tows(record->get_refcount())) ;

			items.push_back(cpptempl::make_data(item)) ;
		}
		data[L"records"] = cpptempl::make_data(items) ;
	}
}