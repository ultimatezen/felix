#pragma once

#include "WindowsMessage.h"

//! Mix-in class for tracking whether a window is active
class ActiveWindow
{
public:
	bool m_is_active ;

	ActiveWindow() : m_is_active(false)
	{

	}
	bool we_are_active()
	{
		return m_is_active ;
	}
	LRESULT on_activate( WindowsMessage &message )
	{
		if (message.w_low() == WA_INACTIVE)
		{
			m_is_active = false ;
		}
		else
		{
			m_is_active = true ;
		}
		// Return 1 if you don't process the message.
		return 1L ;
	}

};