#include "stdafx.h"
#include "number_check.h"


namespace qc
{
	bool NumberCheckRule::check( const std::wstring source, const std::wstring target )
	{
		m_messages.clear() ;

		std::wstring number ;

		for(size_t i = 0 ; i < source.size() ; ++i)
		{
			wchar_t c = source[i] ;
			if (iswdigit(c))
			{
				number += c ;
			}
			else if (! number.empty() && (c == L'.' || c == L','))
			{
				number += c ;
			}
			else if (! number.empty())
			{
				check_number(number, target);
				number.clear() ;
			}
		}

		check_number(number, target);
		return m_messages.empty() ;
	}

	void NumberCheckRule::check_number( std::wstring &number, const std::wstring target )
	{
		if (number.empty())
		{
			return ;
		}
		// get rid of trailing period/comma
		size_t last = number.size() -1 ;
		if (! iswdigit(number[last]))
		{
			number = number.substr(0, last) ;
		}
		if (! boost::icontains(target, number))
		{
			std::wstring message = (boost::wformat(L"Number Check: Number %s in source but not in target") % number).str() ;
			m_messages.push_back(message) ;
		}
	}
}
