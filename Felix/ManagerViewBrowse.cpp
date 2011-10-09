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

			populate_item(item, i, mem);

			items.push_back(cpptempl::make_data(item)) ;
		}
		data[L"records"] = cpptempl::make_data(items) ;
	}

	void ManagerViewBrowse::populate_item( cpptempl::data_map &item, size_t i, mem_engine::memory_pointer mem )
	{
		set_index_info(item, i);

		mem_engine::record_pointer record = mem->get_record_at(i) ;
		set_record_info(item, record);

		// location
		set_memory_info(mem, item);
	}
}