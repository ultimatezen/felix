#pragma once

#include "undoable_action.h"
#include "memory_local.h"

namespace action
{
	class TrimSpacesAction : public UndoableAction
	{
	public:
		mem_engine::memory_pointer m_old ;
		mem_engine::memory_pointer m_new ;

		TrimSpacesAction(mem_engine::memory_pointer mem) : 
			m_new(mem),
			m_old(new mem_engine::memory_local(mem->get_mem_properties()))
		{}

		void undo();
		void redo();
		wstring name()
		{
			return R2WSTR(IDS_ACTION_TRIM_SPACES) ;
		}


	} ;
}