/*!
	@file CStringContainer.h
	@brief Interface for CString container
	@date 2004/12/28
	Time: 22:51:12
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

// string stuff
#ifndef _WTL_NO_CSTRING
#define _WTL_NO_CSTRING
#endif
#include <atlstr.h> // where CString lives

#include <set> 

/** Compare CString's with no case. */
struct nocase_cstring_compare : std::binary_function< CString, CString, bool >
{
	bool operator()( const CString &lhs, const CString &rhs )
	{
		return ( lhs.CompareNoCase( rhs ) < 0 ) ;
	}
} ;

typedef std::set< CString, nocase_cstring_compare > cstring_set ;
