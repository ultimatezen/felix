#include "stdafx.h"
#include "action_strip_tags.h"
#include "tag_stripper.h"

namespace action
{
	void StripTagsAction::undo()
	{
		m_new->clear_memory() ;
		foreach(mem_engine::record_pointer rec, m_old->get_records())
		{
			m_new->add_record(rec) ;
		}
		mem_engine::copy_mem_info(m_old, m_new) ;
	}

	void StripTagsAction::redo()
	{
		m_old->clear_memory() ;
		mem_engine::copy_mem_info(m_new, m_old) ;
		mem_engine::trans_set records ;
		foreach(mem_engine::record_pointer rec, m_new->get_records())
		{
			m_old->add_record(rec->clone()) ;
			rec->set_source(strip_tags_only(rec->get_source_rich())) ;
			rec->set_trans(strip_tags_only(rec->get_trans_rich())) ;
			rec->set_context(strip_tags_only(rec->get_context_rich())) ;
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