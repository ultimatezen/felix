#pragma once

#include "revertible_action.h"
#include "memory_local.h"

namespace action
{
	class ActionStripTags : public ActionRevertible
	{
	public:

		ActionStripTags(mem_engine::memory_pointer mem) : ActionRevertible(mem) {}

		void redo();
		wstring name();

	} ;
}