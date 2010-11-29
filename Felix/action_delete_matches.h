#pragma once

#include "undoable_action.h"
#include "TranslationMemory.h"
#include "record.h"
#include "FelixModelInterface.h"

namespace action
{
	class DeleteMatchesAction : public UndoableAction
	{
	public:
		typedef std::vector<mem_engine::search_match_ptr> match_vec ;
		FelixModelInterface *m_controller ;
		match_vec	m_matches ;

		DeleteMatchesAction(FelixModelInterface *controller,
							match_vec &matches);

		void undo();
		void redo();

		wstring name();

	} ;
}