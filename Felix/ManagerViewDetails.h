#pragma once

#include "ManagerView.h"

namespace mgrview
{
	class ManagerViewDetails : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		size_t m_item ;
		bool m_is_memory ;
		model_iface_ptr m_model ;

		ManagerViewDetails(size_t item, bool is_memory=true) : 
				m_item(item),
				m_is_memory(is_memory),
				ManagerView()
		{}

		// view has been activated
		void activate();
		// show the view content
		void show_content();

		// set the template data
		void set_template_data( cpptempl::data_map &data );
	};
}