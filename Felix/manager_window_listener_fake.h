#pragma once

#include "ManagerView.h"

class ManagerWindowListenerFake : public mgrview::FelixManagerWindowListener
{
public:
	wstring m_message ;
	wstring get_message()
	{
		return m_message ;
	}
};
