#pragma once

#include "undoable_action.h"

namespace action
{
	class FakeUndoableAction : public UndoableAction
	{
	public:
		DECLARE_SENSING_VAR ;

		FakeUndoableAction(){}

		void undo()
		{
			SENSE("undo") ;
		}
		void redo()
		{
			SENSE("redo") ;
		}
		wstring name()
		{
			return wstring(L"FakeUndoableAction") ;
		}


	} ;
}