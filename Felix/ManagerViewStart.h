#pragma once

#include "ManagerView.h"

namespace mgrview
{
	wstring make_mem_list(model_iface_ptr mems, 
		const wstring memtype);

	class ManagerViewStart : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		// the view has been activated
		void activate();
		// show view content
		void show_content();

		// set the template data
		void set_template_data( cpptempl::data_map &data );
	};

	class ManagerViewStartMem : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		// the view has been activated
		void activate(){}
		void show_content();

		// set the template data
		void set_template_data( cpptempl::data_map &data );
	};
	class ManagerViewStartGloss : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		// the view has been activated
		void activate(){}
		// show view content
		void show_content();

		// set the template data
		void set_template_data( cpptempl::data_map &data );
	};

}