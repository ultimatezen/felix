#include "stdafx.h"
#include "ManagerViewQCBrowse.h"
#include "numberfmt.h"
#include "SearchWindow.h"
#include "cpptempl.h"

namespace mgrview
{


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

	ManagerViewQCBrowse::ManagerViewQCBrowse(size_t item,
		size_t page/*=1*/) : 
		m_item(item),
		m_page(page),
		m_paginator(10u),
		ManagerView()
	{

	}

	void ManagerViewQCBrowse::set_template_data( cpptempl::data_map &data )
	{
		data[L"memtype"] = cpptempl::make_data(L"mem") ;

		std::vector<mem_engine::search_match_ptr> &matches = m_window_listener->get_qc_matches() ;

		m_paginator.set_num_records(matches.size()) ;
		m_paginator.set_current_page(m_page-1);
		mem_engine::memory_pointer mem = m_mem_model->memory_at(m_item) ;

		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
		data[L"name"] = cpptempl::make_data(get_memname(mem)) ;
		// page stuff
		data[L"pagination"] = cpptempl::make_data(get_pagination_text(m_paginator, _T("pagination_qc.txt"))) ;
		data[L"page"] = cpptempl::make_data(ulong2wstring(m_paginator.get_current_page()+1)) ;
		data[L"index"] = cpptempl::make_data(tows(m_item)) ;

		CNumberFmt number_format ;
		data[L"num_pages"] = cpptempl::make_data(fmt_num(m_paginator.get_num_pages())) ;

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