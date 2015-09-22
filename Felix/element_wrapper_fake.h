#pragma once

#include "element_wrapper.h"

class element_wrapper_fake : public element_wrapper
{

public:
	wstring m_inner_text ;
	wstring m_tag ; 
	std::map<wstring, wstring> m_attributes ;
	wstring m_id ;
	wstring m_focus ;

	void set_inner_text(const wstring text)
	{
		m_inner_text = text ;
	}
	void set_tag(const wstring tag)
	{
		m_tag = tag ;
	}
	void set_attribute(const wstring key, const wstring val)
	{
		m_attributes[key] = val ;
	}
	wstring get_inner_text()
	{
		return m_inner_text ;
	}

	wstring get_tag()
	{
		return m_tag ;
	}
	wstring get_id()
	{
		return m_id ;
	}
	wstring get_attribute(const wstring key)
	{
		return m_attributes[key]  ;
	}
	void focus(void)
	{
		m_focus += L"focus()" ;
	}
};
