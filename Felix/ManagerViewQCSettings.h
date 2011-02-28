#pragma once

#include "ManagerView.h"
#include "Felix_properties.h"
namespace mgrview
{
	class ManagerViewQCSettings : public ManagerView
	{
		app_props::props_ptr m_props ;
	public:
		DECLARE_SENSING_VAR ;
		ManagerViewQCSettings(app_props::props_ptr props) :
			m_props(props),
			ManagerView()
		{}

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );

	};
}