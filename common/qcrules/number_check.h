#pragma once

#include "check_rule.h"
#include <boost/format.hpp>

namespace qc
{

	class NumberCheckRule : public CheckRule
	{
	public:
		bool check(const std::wstring source, const std::wstring target);

		void check_number( std::wstring &number, const std::wstring target );

	};

}