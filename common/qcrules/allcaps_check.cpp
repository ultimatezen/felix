#include "stdafx.h"
#include "allcaps_check.h"


namespace qc
{


	bool AllCapsCheckRule::check( const std::wstring source, const std::wstring target )
	{
		m_messages.clear() ;
		boost::wregex caps(L"\\b[A-Z]{2,}\\b") ;
		boost::wsmatch what ;
		if (boost::regex_search(source, what, caps))
		{
			for (size_t i = 0 ; i < what.size() ; ++i)
			{
				std::wstring token = what[i] ;
				if (! boost::contains(target, token))
				{
					std::wstring message = (boost::wformat(L"ALL CAPS Check: Word %s in source but not in target") % token).str() ;
					m_messages.push_back(message) ;
				}
			}
		}
		return m_messages.empty();
	}
}