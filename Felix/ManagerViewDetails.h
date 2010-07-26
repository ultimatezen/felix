#pragma once

#include "ManagerView.h"

namespace mgrview
{
	class ManagerViewDetails : public ManagerView
	{
	public:
		DECLARE_SENSING_VAR ;

		void activate();
		void show_content();

	};
}