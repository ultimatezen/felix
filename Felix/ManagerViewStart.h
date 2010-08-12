#pragma once

#include "ManagerView.h"
#include "TextTemplate.h"

namespace mgrview
{
	wstring make_mem_list(FelixModelInterface *mems, 
		const wstring memtype);

	class ManagerViewStart : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate();
		void show_content();
	};

	class ManagerViewStartMem : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate(){}
		void show_content();
	};
	class ManagerViewStartGloss : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate(){}
		void show_content();
	};

}