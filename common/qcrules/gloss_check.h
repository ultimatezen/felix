#pragma once

#include "check_rule.h"

namespace qc
{
	typedef std::pair<std::wstring, std::wstring> gloss_pair ;

	//! Checks for glossary matches in source/translation
	//! If a source has a glossary source term, then raises a
	//! QC error if the gloss translation is not in the trans
	class GlossCheckRule : public CheckRule
	{

		std::vector<gloss_pair> m_gloss_matches ;

	public:
		GlossCheckRule(std::vector<gloss_pair> &matches) : m_gloss_matches(matches){}
		bool check(const std::wstring &source, const std::wstring &target);
	};
}