#pragma once

#include "WindowsMessage.h"

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
		return 0L ;
	}

};