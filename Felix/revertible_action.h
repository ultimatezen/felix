#pragma once

#include "undoable_action.h"
#include "memory_local.h"

namespace action
{
	/** Base class for actions that are rolled back by restoring old TM.
	*/
	class ActionRevertible : public UndoableAction
	{
	public:
		mem_engine::memory_pointer m_old ;
		mem_engine::memory_pointer m_new ;

		ActionRevertible(mem_engine::memory_pointer mem) : 
			m_new(mem),
			m_old(new mem_engine::memory_local(mem->get_properties()))
		{
		}

		//! Undoes the action by rolling back the TM
		void undo();

		//! Backs up TM so it can be rolled back
		void back_up_tm(mem_engine::trans_set &records, 
							boost::function<wstring (wstring text)> pred);
	} ;
}