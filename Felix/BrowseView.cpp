#include "stdafx.h"
#include "BrowseView.h"
#include "numberfmt.h"
#include "SearchWindow.h"
#include "cpptempl.h"

namespace mgrview
{

	void BrowseView::config_page_data( cpptempl::data_map &data, size_t num_pages )
	{
		m_paginator.set_num_records(num_pages) ;
		m_paginator.set_current_page(m_page-1);
		// page stuff
		data[L"pagination"] = cpptempl::make_data(get_pagination_text(m_paginator, _T("pagination_qc.txt"))) ;
		data[L"page"] = cpptempl::make_data(ulong2wstring(m_paginator.get_current_page()+1)) ;
		data[L"index"] = cpptempl::make_data(tows(m_item)) ;

		CNumberFmt number_format ;
		data[L"num_pages"] = cpptempl::make_data(fmt_num(m_paginator.get_num_pages())) ;
	}

	void BrowseView::config_gen_data( cpptempl::data_map &data, mem_engine::memory_pointer mem )
	{
		data[L"message"] = cpptempl::make_data(m_window_listener->get_message()) ;
		data[L"name"] = cpptempl::make_data(get_memname(mem)) ;
	}
}