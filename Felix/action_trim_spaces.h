#pragma once

#include "revertible_action.h"
#include "memory_local.h"

namespace action
{
	class ActionTrimSpaces : public ActionRevertible
	{
	public:

		ActionTrimSpaces(mem_engine::memory_pointer mem) : ActionRevertible(mem) {}

		void redo();
		wstring name();


	} ;
}