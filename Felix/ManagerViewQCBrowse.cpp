#include "stdafx.h"
#include "ManagerViewQCBrowse.h"
#include "SearchWindow.h"

namespace mgrview
{
	ManagerViewQCBrowse::ManagerViewQCBrowse(size_t item, size_t page/*=1*/) : 
		m_is_memory(true),
		BrowseView(item, page)
	{

	}

	void ManagerViewQCBrowse::activate()
	{
		SENSE("activate") ;
	}

	void ManagerViewQCBrowse::show_content()
	{
		SENSE("show_content") ;

		cpptempl::data_map data ;
		set_template_data(data);


		wstring tpl_text = cpptempl::get_template_text(_T("manager/browse_qc.txt")) ;
		m_view->set_text(cpptempl::parse(tpl_text, data)) ;
	}



	void ManagerViewQCBrowse::set_template_data( cpptempl::data_map &data )
	{
		data[L"memtype"] = cpptempl::make_data(L"mem") ;

		std::vector<mem_engine::search_match_ptr> &matches = m_window_listener->get_qc_matches() ;

		mem_engine::memory_pointer mem = m_mem_model->memory_at(m_item) ;

		config_gen_data(data, mem);
		config_page_data(data, matches.size(), _T("pagination_qc.txt"));

		cpptempl::data_list items ;

		for (size_t i = m_paginator.get_start() ; i < m_paginator.get_end() ; ++i)
		{
			cpptempl::data_map item ;
			mem_engine::search_match_ptr match = matches[i] ;
			mem_engine::record_pointer record = match->get_record() ;

			set_index_info(item, i);

			set_record_info(item, record);
			// memory info
			set_memory_info(mem, item);

			cpptempl::data_list qc_messages ;
			std::vector<wstring> messages ;
			match->get_qc_messages(messages) ;
			foreach(wstring message, messages)
			{
				qc_messages.push_back(cpptempl::make_data(message)) ;
			}
			item[L"qc_messages"] = cpptempl::make_data(qc_messages) ;


			items.push_back(cpptempl::make_data(item)) ;
		}
		data[L"records"] = cpptempl::make_data(items) ;
	}
}