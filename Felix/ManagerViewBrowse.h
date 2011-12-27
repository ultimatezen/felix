#pragma once

#include "ManagerView.h"
#include "BrowseView.h"
#include "pagination.h"

namespace mgrview
{
	class ManagerViewBrowse : public BrowseView
	{
	public:
		DECLARE_SENSING_VAR ;

		bool m_is_memory ;

		model_iface_ptr m_model ;


		ManagerViewBrowse(size_t item, 
							bool is_memory=true,
							size_t page=1);

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );

		void populate_item( cpptempl::data_map &item, size_t i, mem_engine::memory_pointer mem );


	};
}