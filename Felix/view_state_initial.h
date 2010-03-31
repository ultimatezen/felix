#pragma once

#include "view_state.h"

class ViewStateInitial : public ViewState
{
public:
	void handle_toggle_edit_mode()
	{
		m_view->put_edit_mode( ! m_view->is_edit_mode() ) ;
	}
};