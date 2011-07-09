#include "stdafx.h"
#include "action_delete_matches.h"

namespace action
{
	using namespace mem_engine ;

	DeleteMatchesAction::DeleteMatchesAction(model_iface_ptr controller,
		match_vec &matches) : 
		m_controller(controller),
		m_matches(matches)
	{

	}

	void DeleteMatchesAction::undo()
	{
		foreach(search_match_ptr match, m_matches)
		{
			memory_pointer mem = m_controller->get_memory_by_id(match->get_memory_id()) ;
			mem->add_record(match->get_record()) ;
		}
	}

	void DeleteMatchesAction::redo()
	{
		foreach(search_match_ptr match, m_matches)
		{
			memory_pointer mem = m_controller->get_memory_by_id(match->get_memory_id()) ;
			mem->erase(match->get_record()) ;
		}
	}

	wstring DeleteMatchesAction::name()
	{
		return R2WSTR(IDS_ACTION_DELETE_MATCHES) ;
	}
}
