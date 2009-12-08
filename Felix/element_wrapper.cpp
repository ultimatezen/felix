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

wstring element_wrapper_html::get_attribute( const wstring key )
{
	const _bstr_t bkey = key.c_str() ;
	const CComVariant var_href = m_element->getAttribute( bkey, 0 ) ;
	ATLASSERT( var_href.vt == VT_BSTR ) ;
	return BSTR2wstring(var_href.bstrVal) ;
}

void element_wrapper_html::set_attribute( const wstring key, const wstring val )
{
	const _bstr_t bkey = key.c_str() ;
	const CComVariant var_val(val.c_str()) ;
	m_element->setAttribute( bkey, var_val, 0 ) ;
}

void element_wrapper_html::set_inner_text( const wstring text )
{
	m_element->innerHTML = string2BSTR(text) ;
}