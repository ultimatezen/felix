#include "stdafx.h"
#include "action_trim_spaces.h"
#include "RegisterGlossDlg.h" // trim_spaces

namespace action
{
	void ActionTrimSpaces::redo()
	{
		using namespace mem_engine;
		mem_engine::trans_set records ;
		this->perform_action(records, trim_text) ;

		m_new->clear_memory() ;
		mem_engine::copy_mem_info(m_old, m_new) ;
		FOREACH(mem_engine::record_pointer rec, records)
		{
			m_new->add_record(rec) ;
		}
	}

	wstring ActionTrimSpaces::name()
	{
		return R2WSTR(IDS_ACTION_TRIM_SPACES) ;
	}
}