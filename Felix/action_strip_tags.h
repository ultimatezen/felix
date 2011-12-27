#pragma once

#include "revertible_action.h"
#include "memory_local.h"

namespace action
{
	class StripTagsAction : public RevertibleAction
	{
	public:

		StripTagsAction(mem_engine::memory_pointer mem) : RevertibleAction(mem) {}

		void redo();
		wstring name();

	} ;
}