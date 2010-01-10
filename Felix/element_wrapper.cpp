#include "stdafx.h"
#include "element_wrapper.h"

void element_wrapper_html::set_element( MSHTML::IHTMLElementPtr element )
{
	m_element = element ;
}

wstring element_wrapper_html::get_inner_text()
{
	SENSE("get_inner_text") ;
#ifdef UNIT_TEST
	return wstring(L"text") ;
#endif

	return BSTR2wstring(m_element->innerHTML) ;
}

wstring element_wrapper_html::get_tag()
{
	SENSE("get_tag") ;
#ifdef UNIT_TEST
	return wstring(L"tag") ;
#endif

	return BSTR2wstring(m_element->tagName) ;
}

wstring element_wrapper_html::get_id()
{
	SENSE("get_id") ;
#ifdef UNIT_TEST
	return wstring(L"id") ;
#endif

	return BSTR2wstring( m_element->id ) ;
}

wstring element_wrapper_html::get_attribute( const wstring key )
{
	SENSE("get_attribute") ;
	SENSE(string2string(key)) ;
#ifdef UNIT_TEST
	return wstring(L"attribute") ;
#endif

	const _bstr_t bkey = key.c_str() ;
	const CComVariant var_href = m_element->getAttribute( bkey, 0 ) ;
	ATLASSERT( var_href.vt == VT_BSTR ) ;
	return BSTR2wstring(var_href.bstrVal) ;
}

void element_wrapper_html::set_attribute( const wstring key, const wstring val )
{
	SENSE("set_attribute") ;
	SENSE(string2string(key)) ;
	SENSE(string2string(val)) ;
#ifdef UNIT_TEST
	return ;
#endif
	const _bstr_t bkey = key.c_str() ;
	const CComVariant var_val(val.c_str()) ;
	m_element->setAttribute( bkey, var_val, 0 ) ;
}

void element_wrapper_html::set_inner_text( const wstring text )
{
	SENSE("set_inner_text") ;
	SENSE(string2string(text)) ;
#ifdef UNIT_TEST
	return ;
#endif
	m_element->innerHTML = string2BSTR(text) ;
}
