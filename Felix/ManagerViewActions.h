#pragma once

#include "ManagerView.h"

namespace mgrview
{
	class ManagerViewActions : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		size_t m_item ;
		bool m_is_memory ;
		model_iface_ptr m_model ;

		ManagerViewActions(size_t item, bool is_memory=true) : 
		m_item(item),
			m_is_memory(is_memory),
			ManagerView()
		{}

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );
	};
}