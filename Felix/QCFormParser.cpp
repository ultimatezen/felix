#include "stdafx.h"
#include "QCFormParser.h"

bool mgrview::QCFormParser::check_numbers()
{
	wstring ischecked = m_doc->get_element_by_id(L"numbers")->get_attribute(L"checked") ;
	return (ischecked == L"true") ;
}

bool mgrview::QCFormParser::check_all_caps()
{
	wstring ischecked = m_doc->get_element_by_id(L"allcaps")->get_attribute(L"checked") ;
	return (ischecked == L"true") ;
}

bool mgrview::QCFormParser::check_gloss()
{
	wstring ischecked = m_doc->get_element_by_id(L"gloss")->get_attribute(L"checked") ;
	return (ischecked == L"true") ;
}

bool mgrview::QCFormParser::live_check()
{
	wstring ischecked = m_doc->get_element_by_id(L"live")->get_attribute(L"checked") ;
	return (ischecked == L"true") ;
}

void mgrview::QCFormParser::checked_glosses( const std::vector<size_t> &memids, std::vector<size_t> &existids )
{
	existids.clear() ;
	foreach(size_t memid, memids)
	{
		wstring idname = boost::lexical_cast<wstring>(memid) ;
		wstring ischecked = m_doc->get_element_by_id(idname)->get_attribute(L"checked") ;
		if (ischecked == L"true")
		{
			existids.push_back(memid) ;
		}

	}
}