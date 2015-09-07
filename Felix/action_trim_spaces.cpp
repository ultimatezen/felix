#include "stdafx.h"
#include "action_trim_spaces.h"
#include "RegisterGlossDlg.h" // trim_spaces

namespace action
{
	//! Trims the spaces from all records in the TM.
	//! Backs up TM so it can be undone.
	void ActionTrimSpaces::redo()
	{
		using namespace mem_engine;
		mem_engine::trans_set records ;
		this->back_up_tm(records, trim_text) ;

		m_new->clear_memory() ;
		mem_engine::copy_mem_info(m_old, m_new) ;
		FOREACH(mem_engine::record_pointer rec, records)
		{
			m_new->add_record(rec) ;
		}
	}

	//! Returns string value of resource ID (for localization)
	wstring ActionTrimSpaces::name()
	{
		return R2WSTR(IDS_ACTION_TRIM_SPACES) ;
	}
}