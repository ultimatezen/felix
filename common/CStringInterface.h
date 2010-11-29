/*!
	@file CStringInterface.h
	@brief Streaming operators for CString (<<)
	@date 2005/06/25
	Time: 15:14:13
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

 // string stuff
#ifndef _WTL_NO_CSTRING
#define _WTL_NO_CSTRING
#endif

#include <atlstr.h>

#include <string>
typedef std::basic_string< TCHAR > tstring ;

inline ATL::CString &operator << ( ATL::CString &lhs, const ATL::CString &rhs )
{
	lhs += rhs ;
	return lhs ;
}

inline ATL::CString &operator << ( ATL::CString &lhs, LPCTSTR rhs )
{
	lhs += rhs ;
	return lhs ;
}

inline ATL::CString &operator << ( ATL::CString &lhs, const tstring &rhs )
{
	lhs += rhs.c_str() ;
	return lhs ;
}

