#pragma once

#include "undoable_action.h"
#include "TranslationMemory.h"
#include "record.h"
#include "FelixModelInterface.h"

namespace action
{
	class ActionDeleteMatches : public UndoableAction
	{
	public:
		typedef std::vector<mem_engine::search_match_ptr> match_vec ;
		model_iface_ptr m_controller ;
		match_vec	m_matches ;

		ActionDeleteMatches(model_iface_ptr controller,
							match_vec &matches);

		void undo();
		void redo();

		wstring name();

	} ;
}