#pragma once

#include "view_interface.h"

// This represents the viewing state in one of the
// windows.
class ViewState
{
public:
	view_interface *m_view ;
	void set_view_interface(view_interface *view)
	{
		m_view = view ;
	}
	virtual void handle_toggle_edit_mode() = 0 ;
};

