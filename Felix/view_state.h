#pragma once

#include "view_interface.h"
#include "FelixModelInterface.h"

// This represents the viewing state in one of the
// windows.
class ViewState
{
public:
	view_interface *m_view ;
	FelixModelInterface *m_model ;
	void set_view_interface(view_interface *view)
	{
		m_view = view ;
	}
	void set_model(FelixModelInterface *model)
	{
		m_model = model ;
	}
	virtual void handle_toggle_edit_mode() = 0 ;
	virtual void show_content() = 0 ;
};

