#pragma once

#include "ExcelImport.h"

namespace excel
{

	class characters
	{
	public:
		Excel::CharactersPtr m_chars ;

		~characters()
		{

		}

		void set_chars(Excel::CharactersPtr chars)
		{
			m_chars = chars ;
		}

		virtual void SetText(_bstr_t text)
		{
			m_chars->Text = text ;
		}
		virtual _bstr_t GetText()
		{
			return m_chars->Text ;
		}
	};

	typedef std::shared_ptr< characters > chars_ptr ;
}