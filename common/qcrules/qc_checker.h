#pragma once
#include "check_rule.h"

namespace qc
{
	//! Container for QC rules
	class QcChecker : public CheckRule
	{
		std::vector<rule_ptr> m_rules ; //! The container of rules
	public:
		QcChecker(std::vector<rule_ptr> &rules) : m_rules(rules) {}
		bool check(const std::wstring &source, const std::wstring &target);
	};
}