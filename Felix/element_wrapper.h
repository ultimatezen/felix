#pragma once
#include "HtmlDocument.h"		// html::collection_ptr

class element_wrapper
{
public:
	virtual ~element_wrapper(){}
	virtual wstring get_inner_text() = 0 ;
	virtual void set_inner_text(const wstring text) = 0 ;
	virtual wstring get_tag() = 0 ;
	virtual wstring get_attribute(const wstring key) = 0 ;
	virtual void set_attribute(const wstring key, const wstring val) = 0 ;
	virtual wstring get_id() = 0 ;
	virtual void focus() = 0 ;
};

typedef std::shared_ptr<element_wrapper> element_wrapper_ptr ;

class element_wrapper_html : public element_wrapper
{
	MSHTML::IHTMLElementPtr m_element ;

public:
	DECLARE_SENSING_VAR ;

	void set_element(MSHTML::IHTMLElementPtr element);
	wstring get_inner_text();
	void set_inner_text (const wstring text);
	wstring get_tag();
	wstring get_id();
	wstring get_attribute(const wstring key);
	void set_attribute(const wstring key, const wstring val);
	void focus (void);
};



inline element_wrapper_ptr make_element_wrapper(MSHTML::IHTMLElementPtr element)
{
	element_wrapper_html *wrapper = new element_wrapper_html() ;
	wrapper->set_element(element) ;
	return element_wrapper_ptr(wrapper) ;
}