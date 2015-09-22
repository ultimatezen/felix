/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include "StdAfx.h"
#include "AppStringEntry.h"

/*!
 * \brief
 * Default constructor
  */
CAppStringEntry::CAppStringEntry(void)
{
}

/*!
 * \brief
 * Copy constructor
 * 
 * Pass in another CAppStrinEntry, and copy the strings
 */
CAppStringEntry::CAppStringEntry(const CAppStringEntry &entry )
{
	app_string_map tmp( entry.m_appStrings ) ;
	tmp.swap( m_appStrings) ;
}

/*!
 */
CAppStringEntry& CAppStringEntry::operator=( const CAppStringEntry &rhs )
{
	app_string_map tmp( rhs.m_appStrings ) ;
	tmp.swap( m_appStrings) ;

	return *this ;
}

/*!
 */
void CAppStringEntry::set_entry( const wstring &key, const wstring &val )
{
	m_appStrings[key] = val ;
}
/*!
 */
const wstring CAppStringEntry::get_entry( const wstring &key ) const
{
	app_string_map::const_iterator pos = m_appStrings.find( key ) ;

	if ( m_appStrings.end() == pos ) 
	{
		return wstring() ;
	}

	return pos->second ;
}

