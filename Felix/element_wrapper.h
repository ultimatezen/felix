#pragma once
#include "HtmlDocument.h"		// html::collection_ptr

class element_wrapper
{
public:
	virtual ~element_wrapper(){}
	virtual wstring get_inner_text() = 0 ;
	virtual void set_inner_text(wstring text) = 0 ;
	virtual wstring get_tag() = 0 ;
	virtual wstring get_attribute(wstring key) = 0 ;
	virtual void set_attribute(wstring key, wstring val) = 0 ;
	virtual wstring get_id() = 0 ;
};

typedef boost::shared_ptr<element_wrapper> element_wrapper_ptr ;

class element_wrapper_html : public element_wrapper
{
	MSHTML::IHTMLElementPtr m_element ;

public:
	void set_element(MSHTML::IHTMLElementPtr element);
	wstring get_inner_text();
	void set_inner_text(wstring text);
	wstring get_tag();
	wstring get_id();
	wstring get_attribute(wstring key);
	void set_attribute(wstring key, wstring val);
};



inline element_wrapper_ptr make_element_wrapper(MSHTML::IHTMLElementPtr element)
{
	element_wrapper_html *wrapper = new element_wrapper_html() ;
	wrapper->set_element(element) ;
	return element_wrapper_ptr(wrapper) ;
}