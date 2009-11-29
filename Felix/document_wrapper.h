#pragma once

#include "element_wrapper.h"


class doc3_wrapper
{
public:
	virtual ~doc3_wrapper(){}
	virtual element_wrapper_ptr get_element_by_id(wstring id) = 0 ;
};

typedef boost::shared_ptr<doc3_wrapper> doc3_wrapper_ptr ;

class doc3_wrapper_html : public doc3_wrapper
{
	html::document3_ptr m_document ;
public:
	void set_document(html::document3_ptr doc)
	{
		m_document = doc ;
	}
	element_wrapper_ptr get_element_by_id(wstring id)
	{
		return make_element_wrapper(m_document->getElementById(id.c_str())) ;
	}
};


inline doc3_wrapper_ptr make_doc3_wrapper(html::document3_ptr doc)
{
	doc3_wrapper_html *wrapper = new doc3_wrapper_html() ;
	wrapper->set_document(doc) ;
	return doc3_wrapper_ptr(wrapper) ;
}