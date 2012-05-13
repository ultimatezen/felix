#pragma once

#include "ManagerView.h"

namespace mgrview
{
	cpptempl::data_ptr item_bool(bool value) ;
	wstring make_mem_list(model_iface_ptr mems, 
		const wstring memtype);

	class ManagerViewStart : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate();
		void show_content();

		void set_template_data( cpptempl::data_map &data );
	};

	class ManagerViewStartMem : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate(){}
		void show_content();

		void set_template_data( cpptempl::data_map &data );
	};
	class ManagerViewStartGloss : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate(){}
		void show_content();

		void set_template_data( cpptempl::data_map &data );
	};

}