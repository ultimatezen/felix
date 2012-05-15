#pragma once

#include "ManagerView.h"
#include "pagination.h"
#include "cpptempl.h"


namespace mgrview
{
	class BrowseView : public ManagerView
	{
	public:
		// for paginating browse list
		Paginator m_paginator ;
		size_t m_item ;
		size_t m_page ;

		BrowseView(size_t item, size_t page) : 
			m_paginator(10u),
			m_item(item),
			m_page(page),
			ManagerView() 
		{}

		void config_page_data( cpptempl::data_map &data, size_t num_pages, CString page_name );
		void config_gen_data( cpptempl::data_map &data, mem_engine::memory_pointer mem );

	};
}