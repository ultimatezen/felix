#pragma once

#include "ManagerView.h"

namespace mgrview
{
	class ManagerViewQCSettings : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );

	};
}