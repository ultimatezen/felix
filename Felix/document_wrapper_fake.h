#pragma once

#include "document_wrapper.h"
#include "element_wrapper_fake.h"

class doc3_wrapper_fake : public doc3_wrapper
{
public:
	std::map<wstring, element_wrapper_ptr> m_elements ;
	void add_element(wstring key, element_wrapper_ptr element)
	{
		m_elements[key] = element ;
	}
	element_wrapper_ptr get_element_by_id(wstring id)
	{
		if (m_elements.find(id) == m_elements.end())
		{
			element_wrapper_ptr id_element(new element_wrapper_fake()) ;
			add_element(id, id_element) ;
		}
		return m_elements[id] ;
	}
};

inline doc3_wrapper_ptr get_fake_search_doc()
{
	doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
	return doc3_wrapper_ptr(wrapper) ;
}

