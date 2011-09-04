#include "stdafx.h"
#include "action_trim_spaces.h"
#include "RegisterGlossDlg.h" // trim_spaces

namespace action
{
	void TrimSpacesAction::undo()
	{
		using namespace mem_engine;
		m_new->clear_memory() ;
		foreach(mem_engine::record_pointer rec, m_old->get_records() | ad::map_values)
		{
			m_new->add_record(rec) ;
		}
		mem_engine::copy_mem_info(m_old, m_new) ;
	}

	void TrimSpacesAction::redo()
	{
		using namespace mem_engine;
		m_old->clear_memory() ;
		mem_engine::copy_mem_info(m_new, m_old) ;
		mem_engine::trans_set records ;
		foreach(mem_engine::record_pointer rec, m_new->get_records() | ad::map_values)
		{
			m_old->add_record(rec->clone()) ;
			rec->set_source(trim_text(rec->get_source_rich())) ;
			rec->set_trans(trim_text(rec->get_trans_rich())) ;
			rec->set_context(trim_text(rec->get_context_rich())) ;
			records.insert(rec) ;
		}
		m_new->clear_memory() ;
		mem_engine::copy_mem_info(m_old, m_new) ;
		foreach(mem_engine::record_pointer rec, records)
		{
			m_new->add_record(rec) ;
		}
	}
}