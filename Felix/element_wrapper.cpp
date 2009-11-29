#include "stdafx.h"
#include "element_wrapper.h"

void element_wrapper_html::set_element( MSHTML::IHTMLElementPtr element )
{
	m_element = element ;
}

wstring element_wrapper_html::get_inner_text()
{
	return BSTR2wstring(m_element->innerHTML) ;
}

wstring element_wrapper_html::get_tag()
{
	return BSTR2wstring(m_element->tagName) ;
}

wstring element_wrapper_html::get_id()
{
	return BSTR2wstring( m_element->id ) ;
}

wstring element_wrapper_html::get_attribute( wstring key )
{
	_bstr_t bkey = key.c_str() ;
	CComVariant var_href = m_element->getAttribute( bkey, 0 ) ;
	ATLASSERT( var_href.vt == VT_BSTR ) ;
	return wstring( (LPCWSTR)var_href.bstrVal, SysStringLen( var_href.bstrVal) )  ;
}

void element_wrapper_html::set_attribute( wstring key, wstring val )
{
	_bstr_t bkey = key.c_str() ;
	CComVariant var_val(val.c_str()) ;
	m_element->setAttribute( bkey, var_val, 0 ) ;
}

void element_wrapper_html::set_inner_text( wstring text )
{
	m_element->innerHTML = string2BSTR(text) ;
}