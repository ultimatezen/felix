#pragma once

/**
Support for undo/redo

Group of classes that provide undable/redoable actions.
*/
namespace action
{
	/**
	Base class for undoable/redoable actions.
	*/
	class UndoableAction
	{
	public:
		//! Returns the name for display in UI
		virtual wstring name() = 0 ;
		//! Undoes the action
		virtual void undo() = 0 ;
		//! Performs/re-performs the action
		virtual void redo() = 0 ;
	};

	typedef std::shared_ptr<UndoableAction> undo_action_ptr ;
}
