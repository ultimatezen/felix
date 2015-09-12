#pragma once

//! Quality control rules
//! Used to ensure quality of translation pairs in TM.
namespace qc
{
	//! Base case for rule checker
	class CheckRule
	{
	protected:
		std::vector<std::wstring> m_messages ;
	public:
		//! Checks a rule
		virtual bool check(const std::wstring &source, const std::wstring &target) = 0 ;
		//! Returns the error messages for this QC rule
		//! Uses the temp/swap technique to copy the messages into the vector
		void get_error_msgs(std::vector<std::wstring> &messages)
		{
			std::vector<std::wstring> temp(m_messages) ;
			messages.swap(temp) ;
		}
	};

	//! Pointer to a QC rule
	typedef std::shared_ptr<CheckRule> rule_ptr ;

}