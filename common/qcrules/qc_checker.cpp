#include "stdafx.h"
#include "qc_checker.h"

namespace qc
{

	bool QcChecker::check( const std::wstring &source, const std::wstring &target )
	{
		m_messages.clear() ;
		for(rule_ptr rule: m_rules)
		{
			if (! rule->check(source, target))
			{
				std::vector<std::wstring> messages ;
				rule->get_error_msgs(messages) ;
				std::copy(messages.begin(), messages.end(), std::back_inserter(m_messages)) ;
			}
		}
		return m_messages.empty() ;
	}
}