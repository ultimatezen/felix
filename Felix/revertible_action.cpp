#include "stdafx.h"
#include "revertible_action.h"
#include "boost/function.hpp"

namespace action
{
	void ActionRevertible::undo()
	{
		using namespace mem_engine;
		m_new->clear_memory() ;
		FOREACH(mem_engine::record_pointer rec, m_old->get_records() | ad::map_values)
		{
			m_new->add_record(rec) ;
		}
		mem_engine::copy_mem_info(m_old, m_new) ;
	}

	void ActionRevertible::back_up_tm(mem_engine::trans_set &records,
											boost::function<wstring (wstring text)> pred )
	{
		using namespace mem_engine;
		m_old->clear_memory() ;
		mem_engine::copy_mem_info(m_new, m_old) ;
		FOREACH(mem_engine::record_pointer rec, m_new->get_records() | ad::map_values)
		{
			m_old->add_record(rec->clone()) ;
			rec->set_source(pred(rec->get_source_rich())) ;
			rec->set_trans(pred(rec->get_trans_rich())) ;
			rec->set_context(pred(rec->get_context_rich())) ;
			records.insert(rec) ;
		}
	}
}