#pragma once

namespace action
{
	class UndoableAction
	{
	public:
		virtual void undo() = 0 ;
		virtual void redo() = 0 ;
	};

	typedef boost::shared_ptr<UndoableAction> undo_action_ptr ;
}
