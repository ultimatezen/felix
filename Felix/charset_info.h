/*!
	@file charset_info.h
	@brief interface for the charset_info class.
	@date 2005/06/25
	Time: 15:11:27
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include <set>

/**
	@class charset_info  
	@brief Information about a charset.
 */
class charset_info  
{
	typedef std::pair< string, unsigned int > code_pair ;
	typedef std::set< code_pair > code_set ;
	typedef code_set::iterator code_iterator ;

	code_set codes ;

public:
	unsigned int cp_from_lang_str( const string lang ) const;
	unsigned int cp_from_charset( const BYTE charset ) const;
	charset_info();
	virtual ~charset_info();

};

