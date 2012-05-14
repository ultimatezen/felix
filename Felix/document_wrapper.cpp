#include "stdafx.h"
#include "document_wrapper.h"


void doc3_wrapper_html::set_document( html::document3_ptr doc )
{
	m_document = doc ;
}

element_wrapper_ptr doc3_wrapper_html::get_element_by_id( wstring id )
{
	MSHTML::IHTMLElementPtr element = m_document->getElementById(id.c_str());

	MSHTML::IHTMLElement2Ptr element2 ;
	element->QueryInterface(IID_IHTMLElement2, (void**)&element2) ;

	return make_element_wrapper(element2) ;
}

doc3_wrapper_ptr make_doc3_wrapper( html::document3_ptr doc )
{
	doc3_wrapper_html *wrapper = new doc3_wrapper_html() ;
	wrapper->set_document(doc) ;
	return doc3_wrapper_ptr(wrapper) ;
}

wstring doc3_wrapper_html::element_value( const wstring key )
{
	return get_element_by_id(key)->get_attribute(L"value") ;
}

bool doc3_wrapper_html::element_checked(const wstring key)
{
	const wstring ischecked = get_element_by_id(key)->get_attribute(L"checked") ;
	return (ischecked == L"true") ;
}