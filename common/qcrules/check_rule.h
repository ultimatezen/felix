#pragma once

//! Quality control rules
namespace qc
{
	//! Base case for rule checker
	class CheckRule
	{
	protected:
		std::vector<std::wstring> m_messages ;
	public:
		// Checks a rule
		virtual bool check(const std::wstring &source, const std::wstring &target) = 0 ;
		void get_error_msgs(std::vector<std::wstring> &messages)
		{
			std::vector<std::wstring> temp(m_messages) ;
			messages.swap(temp) ;
		}
	};

	typedef std::shared_ptr<CheckRule> rule_ptr ;

}