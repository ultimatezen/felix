#pragma once

#include "undoable_action.h"
#include "TranslationMemory.h"
#include "record.h"

namespace action
{
	class DeleteEntryAction : public UndoableAction
	{
	public:
		mem_engine::memory_pointer m_mem ;
		mem_engine::record_pointer m_rec ;

		DeleteEntryAction(mem_engine::memory_pointer mem,
						mem_engine::record_pointer rec);

		void undo();
		void redo();

		wstring name();

	} ;
}