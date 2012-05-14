#pragma once

#include "element_wrapper.h"


class doc3_wrapper
{
public:
	virtual ~doc3_wrapper(){}
	virtual element_wrapper_ptr get_element_by_id(wstring id) = 0 ;
	virtual bool element_checked(const wstring key) = 0 ;
	virtual wstring element_value(const wstring key) = 0 ;
};

typedef boost::shared_ptr<doc3_wrapper> doc3_wrapper_ptr ;

class doc3_wrapper_html : public doc3_wrapper
{
	html::document3_ptr m_document ;
public:
	void set_document(html::document3_ptr doc);
	element_wrapper_ptr get_element_by_id(wstring id);
	bool element_checked(const wstring key);
	wstring element_value(const wstring key);
};


doc3_wrapper_ptr make_doc3_wrapper(html::document3_ptr doc);