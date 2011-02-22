#pragma once
#include "check_rule.h"

#include <vector>
#include <string>

#include <boost/foreach.hpp>
using namespace boost::foreach ;

namespace qc
{
	class QcChecker : public CheckRule
	{
		std::vector<rule_ptr> m_rules ;
	public:
		QcChecker(std::vector<rule_ptr> &rules) : m_rules(rules) {}
		bool check(const std::wstring source, const std::wstring target);
	};
}