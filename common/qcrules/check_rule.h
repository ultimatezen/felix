#pragma once

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace qc
{

	class CheckRule
	{
	protected:
		std::vector<std::wstring> m_messages ;
	public:
		virtual bool check(const std::wstring source, const std::wstring target) = 0 ;
		void get_error_msgs(std::vector<std::wstring> &messages)
		{
			std::vector<std::wstring> temp(m_messages) ;
			messages.swap(temp) ;
		}
	};

	typedef boost::shared_ptr<CheckRule> rule_ptr ;

}