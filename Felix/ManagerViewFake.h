#pragma once

#include "ManagerView.h"
#include "pagination.h"

namespace mgrview
{
	class ManagerViewFake : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;


		ManagerViewFake()
		{}

		void activate()
		{
			SENSE("activate") ;
		}
		void show_content()
		{
			SENSE("show_content") ;
		}

	};
}