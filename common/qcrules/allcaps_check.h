#pragma once

#include "check_rule.h"

namespace qc
{
	class AllCapsCheckRule : public CheckRule
	{
	public:
		bool check(const std::wstring &source, const std::wstring &target);

	};
}