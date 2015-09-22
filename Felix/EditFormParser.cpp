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
		return element_value(L"creator");
	}

	wstring EditFormParser::default_context()
	{
		return element_value(L"default_context");
	}

	wstring EditFormParser::field()
	{
		return element_value(L"field");
	}

	wstring EditFormParser::created_on()
	{
		return element_value(L"created_on");
	}

	wstring EditFormParser::source_language()
	{
		return element_value(L"source_language");
	}

	wstring EditFormParser::target_language()
	{
		return element_value(L"target_language");
	}

	wstring EditFormParser::client()
	{
		return element_value(L"client");
	}

	bool EditFormParser::is_memory()
	{
		return element_checked(L"mem_type_tm");
	}
	bool EditFormParser::is_active()
	{
		return element_checked(L"is_active");
	}
	bool EditFormParser::locked()
	{
		return element_checked(L"locked");
	}

	wstring EditFormParser::element_value( const wstring key )
	{
		return m_doc->element_value(key) ;
	}

	bool EditFormParser::element_checked(const wstring key)
	{
		return m_doc->element_checked(key) ;
	}
}