#include "stdafx.h"
#include "action_delete_entry.h"

namespace action
{
	DeleteEntryAction::DeleteEntryAction( mem_engine::memory_pointer mem, 
										  mem_engine::record_pointer rec ) : 
		m_mem(mem),
		m_rec(rec)
	{

	}

	void DeleteEntryAction::undo()
	{
		m_mem->add_record(m_rec) ;
	}

	void DeleteEntryAction::redo()
	{
		m_mem->erase(m_rec) ;
	}

	wstring DeleteEntryAction::name()
	{
		return R2WSTR(IDS_ACTION_DELETE_ENTRY) ;
	}
}
