#include "stdafx.h"
#include "gloss_check.h"


namespace qc
{
	bool GlossCheckRule::check( const std::wstring &, const std::wstring &target )
	{
		for(gloss_pair gloss: m_gloss_matches)
		{
			if (! boost::contains(target, gloss.second))
			{
				std::wstring message = (boost::wformat(L"Gloss Check: Word '%s' in source but '%s' not in target") % gloss.first % gloss.second).str() ;
				m_messages.push_back(message) ;
			}
		}
		return m_messages.empty();
	}
}