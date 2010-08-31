#include "stdafx.h"
#include "EditFormParser.h"

namespace mgrview
{

	EditFormParser::EditFormParser( doc3_wrapper_ptr doc )
	{
		m_doc = doc ;
	}

	wstring EditFormParser::creator()
	{
		return m_doc->get_element_by_id(L"creator")->get_inner_text() ;
	}
	wstring EditFormParser::field()
	{
		return m_doc->get_element_by_id(L"field")->get_inner_text() ;
	}

	wstring EditFormParser::created_on()
	{
		return m_doc->get_element_by_id(L"created_on")->get_inner_text() ;
	}

	wstring EditFormParser::source_language()
	{
		return m_doc->get_element_by_id(L"source_language")->get_inner_text() ;
	}

	wstring EditFormParser::target_language()
	{
		return m_doc->get_element_by_id(L"target_language")->get_inner_text() ;
	}

	wstring EditFormParser::client()
	{
		return m_doc->get_element_by_id(L"client")->get_inner_text() ;
	}

	bool EditFormParser::is_memory()
	{
		wstring ischecked = m_doc->get_element_by_id(L"mem_type_tm")->get_attribute(L"checked") ;
		return (ischecked == L"true") ;
	}

	bool EditFormParser::locked()
	{
		wstring ischecked = m_doc->get_element_by_id(L"locked")->get_attribute(L"checked") ;
		return (ischecked == L"true") ;
	}
}