#pragma once

#include "revertible_action.h"
#include "memory_local.h"

namespace action
{
	class TrimSpacesAction : public RevertibleAction
	{
	public:

		TrimSpacesAction(mem_engine::memory_pointer mem) : RevertibleAction(mem) {}

		void redo();
		wstring name();


	} ;
}