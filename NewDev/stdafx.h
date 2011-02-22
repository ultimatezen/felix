// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define UNIT_TEST

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <boost/format.hpp>
using boost::format;
using boost::wformat ;
typedef boost::basic_format< TCHAR > tformat;


#include <iostream>
#include <ostream>
#include <sstream>

// Allow streaming of wstring's to std::ostream
namespace std {
	inline std::ostream& operator<<(std::ostream& out, const std::wstring& value)
	{
		for (size_t i = 0 ; i < value.length() ; ++i)
		{
			out << static_cast<char>(value[i]); 
		}
		return out;
	}
}

// TODO: reference additional headers your program requires here
