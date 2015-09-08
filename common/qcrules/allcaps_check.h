#pragma once

#include "check_rule.h"

namespace qc
{
	//! Checks whether all-caps words in source appear in trans
	class AllCapsCheckRule : public CheckRule
	{
	public:
		bool check(const std::wstring &source, const std::wstring &target);

	};
}