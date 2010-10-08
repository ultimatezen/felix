#include "stdafx.h"
#include "action_trim_spaces.h"
#include "RegisterGlossDlg.h" // trim_spaces

namespace action
{
	void TrimSpacesAction::undo()
	{
		m_new->clear_memory() ;
		foreach(mem_engine::record_pointer rec, m_old->get_records())
		{
			m_new->add_record(rec) ;
		}
	}

	void TrimSpacesAction::redo()
	{
		m_old->clear_memory() ;
		mem_engine::trans_set records ;
		foreach(mem_engine::record_pointer rec, m_new->get_records())
		{
			m_old->add_record(rec->clone()) ;
			rec->set_source(trim_text(rec->get_source_rich())) ;
			rec->set_trans(trim_text(rec->get_trans_rich())) ;
			rec->set_context(trim_text(rec->get_context_rich())) ;
			records.insert(rec) ;
		}
		m_new->clear_memory() ;
		foreach(mem_engine::record_pointer rec, records)
		{
			m_new->add_record(rec) ;
		}
	}
}