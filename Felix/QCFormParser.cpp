#include "stdafx.h"
#include "QCFormParser.h"

bool mgrview::QCFormParser::check_numbers()
{
	return m_doc->element_checked(L"numbers") ;
}

bool mgrview::QCFormParser::check_all_caps()
{
	return m_doc->element_checked(L"allcaps") ;
}

bool mgrview::QCFormParser::check_gloss()
{
	return m_doc->element_checked(L"gloss") ;
}

bool mgrview::QCFormParser::live_check()
{
	return m_doc->element_checked(L"live") ;
}

void mgrview::QCFormParser::checked_glosses( const std::vector<size_t> &memids, std::vector<size_t> &existids )
{
	existids.clear() ;
	FOREACH(size_t memid, memids)
	{
		if (is_id_checked(memid))
		{
			existids.push_back(memid) ;
		}
	}
}

bool mgrview::QCFormParser::is_id_checked( size_t memid )
{
	wstring idname = boost::lexical_cast<wstring>(memid) ;
	return m_doc->element_checked(idname);
}