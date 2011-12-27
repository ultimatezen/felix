#include "stdafx.h"
#include "ManagerViewBrowse.h"
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
		m_is_memory(is_memory),
		BrowseView(item, page)
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

		config_gen_data(data, mem);
		config_page_data(data, mem->size());

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