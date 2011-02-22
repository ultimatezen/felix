#pragma once

#include "check_rule.h"

#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <boost/foreach.hpp>
using namespace boost::foreach ;

namespace qc
{
	class GlossCheckRule : public CheckRule
	{
		typedef std::pair<std::wstring, std::wstring> gloss_pair ;

		std::vector<gloss_pair> m_gloss_matches ;

	public:
		void clear_gloss_matches()
		{
			m_gloss_matches.clear() ;
		}
		void add_gloss_match(const std::wstring source, const std::wstring target)
		{
			m_gloss_matches.push_back(gloss_pair(source, target)) ;
		}
		bool check(const std::wstring source, const std::wstring target)
		{
			BOOST_FOREACH(gloss_pair gloss, m_gloss_matches)
			{
				if (! boost::contains(target, gloss.second))
				{
					std::wstring message = (boost::wformat(L"Gloss Check: Word '%s' in source but '%s' not in target") % gloss.first % gloss.second).str() ;
					m_messages.push_back(message) ;
				}
			}
			return m_messages.empty();
		}

	};
}