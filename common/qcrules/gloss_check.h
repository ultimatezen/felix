#pragma once

#include "check_rule.h"

#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

namespace qc
{
	typedef std::pair<std::wstring, std::wstring> gloss_pair ;

	class GlossCheckRule : public CheckRule
	{

		std::vector<gloss_pair> m_gloss_matches ;

	public:
		GlossCheckRule(std::vector<gloss_pair> &matches) : m_gloss_matches(matches){}
		bool check(const std::wstring source, const std::wstring target);
	};
}