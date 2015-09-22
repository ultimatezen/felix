#pragma once

#include "ManagerView.h"
#include "BrowseView.h"
#include "pagination.h"

namespace mgrview
{
	// Used to browse QC results in manager view
	class ManagerViewQCBrowse : public BrowseView
	{
	public:
		DECLARE_SENSING_VAR ;

		bool m_is_memory ;

		model_iface_ptr m_model ;


		ManagerViewQCBrowse(size_t item,
			size_t page=1);

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );

	};
}