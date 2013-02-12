#include "stdafx.h"
#include "action_strip_tags.h"
#include "tag_stripper.h"
#include "logging.h"

namespace action
{
	void ActionStripTags::redo()
	{
		using namespace mem_engine;

		mem_engine::trans_set records ;
		this->perform_action(records, strip_tags_only) ;
		m_new->clear_memory() ;
		try
		{
			mem_engine::copy_mem_info(m_old, m_new) ;
			FOREACH(mem_engine::record_pointer rec, records)
			{
				try
				{
					m_new->add_record(rec) ;
				}
				catch (except::CBadRecordException& e)
				{
					string msg(CT2A(e.what())) ;
					logging::log_warn(string("Failed to save stripped record: ") + msg) ;
				}
			}
		}
		catch (...)
		{
			// restore memory
			m_new = m_old ;
			throw ;
		}
	}

	wstring ActionStripTags::name()
	{
		return R2WSTR(IDS_ACTION_STRIP_TAGS) ;
	}
}