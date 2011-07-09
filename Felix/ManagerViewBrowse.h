#pragma once

#include "ManagerView.h"
#include "pagination.h"

namespace mgrview
{
	class ManagerViewBrowse : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		size_t m_item ;
		size_t m_page ;
		bool m_is_memory ;

		// for paginating browse list
		Paginator m_paginator ;
		model_iface_ptr m_model ;


		ManagerViewBrowse(size_t item, 
							bool is_memory=true,
							size_t page=1);

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );
	};
}