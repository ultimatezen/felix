#pragma once

#include "check_rule.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

namespace qc
{
	class AllCapsCheckRule : public CheckRule
	{
	public:
		bool check(const std::wstring source, const std::wstring target);

	};
}