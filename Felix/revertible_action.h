#pragma once

#include "undoable_action.h"
#include "memory_local.h"

namespace action
{
	// Base class for actions that are reverted by restoring old TM
	class RevertibleAction : public UndoableAction
	{
	public:
		mem_engine::memory_pointer m_old ;
		mem_engine::memory_pointer m_new ;

		RevertibleAction(mem_engine::memory_pointer mem) : 
			m_new(mem),
			m_old(new mem_engine::memory_local(mem->get_properties()))
		{
		}

		void undo();

		void perform_action(mem_engine::trans_set &records, 
							boost::function<wstring (wstring text)> pred);
	} ;
}