#pragma once

#include "document_wrapper.h"

class doc3_wrapper_fake : public doc3_wrapper
{
	std::map<wstring, element_wrapper_ptr> m_elements ;
public:
	void add_element(wstring key, element_wrapper_ptr element)
	{
		m_elements[key] = element ;
	}
	element_wrapper_ptr get_element_by_id(wstring id)
	{
		return m_elements[id] ;
	}
};

inline doc3_wrapper_ptr get_fake_search_doc()
{
	doc3_wrapper_fake *wrapper = new doc3_wrapper_fake() ;
	try
	{
		// search stuff
		element_wrapper_ptr searchbox(new element_wrapper_fake()) ;
		wrapper->add_element(L"searchbox", searchbox) ;

		// universal stuff
		element_wrapper_ptr filterbox(new element_wrapper_fake()) ;
		element_wrapper_ptr searchresults(new element_wrapper_fake()) ;

		wrapper->add_element(L"filterbox", filterbox) ;
		wrapper->add_element(L"searchresults", searchresults) ;

		// replace stuff
		element_wrapper_ptr replacefrom_box(new element_wrapper_fake()) ;
		element_wrapper_ptr replaceto_box(new element_wrapper_fake()) ;
		element_wrapper_ptr replacelinks(new element_wrapper_fake()) ;

		wrapper->add_element(L"replacelinks", replacelinks) ;
		wrapper->add_element(L"replacefrom", replacefrom_box) ;
		wrapper->add_element(L"replaceto", replaceto_box) ;
	}
	catch (...)
	{
		if (wrapper)
		{
			delete wrapper ;
		}
		throw ;
	}

	return doc3_wrapper_ptr(wrapper) ;
}

