/*!
 * For localization
 */
#pragma once

typedef std::map< wstring, wstring > app_string_map ;

/*!
 * \brief
 * Keeps a db of localized strings.
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

typedef std::shared_ptr< CAppStringEntry > app_str_entry_ptr ;