#pragma once

namespace action
{
	class UndoableAction
	{
	public:
		virtual wstring name() = 0 ;
		virtual void undo() = 0 ;
		virtual void redo() = 0 ;
	};

	typedef std::shared_ptr<UndoableAction> undo_action_ptr ;
}
