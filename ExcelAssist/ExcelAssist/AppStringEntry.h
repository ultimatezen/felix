/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "stringex.h"
#include <map>

typedef std::map< wstring, wstring > app_string_map ;

/*!
 * \brief
 * Write brief comment for CAppStringEntry here.
 * 
 * Write detailed description for CAppStringEntry here.
 * 
 * \remarks
 * Write remarks for CAppStringEntry here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CAppStringEntry
{
public:
	CAppStringEntry(void);
	CAppStringEntry(const CAppStringEntry &entry );

	void set_entry( const wstring &key, const wstring &val ) ;
	const wstring get_entry( const wstring &key ) const ;

	CAppStringEntry& operator=( const CAppStringEntry &rhs ) ;
private:
	
	app_string_map m_appStrings ;
};

typedef boost::shared_ptr< CAppStringEntry > app_str_entry_ptr ;