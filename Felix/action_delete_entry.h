#pragma once

#include "undoable_action.h"
#include "TranslationMemory.h"
#include "record.h"

namespace action
{
	//! Deletes a specified record.
	class ActionDeleteEntry : public UndoableAction
	{
	public:
		mem_engine::memory_pointer m_mem ;
		mem_engine::record_pointer m_rec ;

		ActionDeleteEntry(mem_engine::memory_pointer mem,
						mem_engine::record_pointer rec);

		void undo();
		void redo();

		wstring name();

	} ;
}