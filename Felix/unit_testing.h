#pragma once

#include <iostream>
#include <ostream>
#include <sstream>

// Allow streaming of wstring's to std::ostream
namespace std 
{
	inline std::ostream& operator<<(std::ostream& out, const std::wstring& value)
	{
		out << string2string(value);
		return out;
	}
	inline std::ostream& operator<<(std::ostream& out, const std::pair<wstring, wstring>& value)
	{
		out << "<" << value.first << ", " << value.second << ">" ;
		return out;
	}
}



