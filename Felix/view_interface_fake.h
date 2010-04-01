#pragma once

#include "view_interface.h"

class view_interface_fake : public view_interface
{

public:
	DECLARE_SENSING_VAR ;

	bool m_is_edit_mode ;

	view_interface_fake() : m_is_edit_mode(false)
	{
	}

	bool is_edit_mode()
	{
		SENSE("is_edit_mode") ;
		return m_is_edit_mode ;
	}
	void put_edit_mode( bool setting )
	{
		SENSE("put_edit_mode") ;
		m_is_edit_mode = setting ;
	}
	void ensure_document_complete()
	{
		SENSE("ensure_document_complete") ;
	}
	void ensure_navigation_complete()
	{
		SENSE("ensure_navigation_complete") ;
	}
	void navigate(LPCTSTR url)
	{
		url ;
		SENSE("navigate") ;
		SENSE(string2string(wstring(url))) ;
	}
};

