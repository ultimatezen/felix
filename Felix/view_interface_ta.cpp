#include "StdAfx.h"
#include ".\view_interface_ta.h"

view_interface_ta::view_interface_ta(void)
{
}

view_interface_ta::~view_interface_ta(void)
{
	ATLASSERT ( m_view.IsWindow() == FALSE ) ; 
}
