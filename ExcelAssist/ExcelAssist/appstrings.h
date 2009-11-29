/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma once

#include "stringex.h"
#include <map>
#include "AppStringEntry.h"
#include "file.h"

/*!
 * \brief
 * Write brief comment for CAppStrings here.
 * 
 * Write detailed description for CAppStrings here.
 * 
 * \remarks
 * Write remarks for CAppStrings here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class CAppStrings
{
	typedef std::map< wstring, app_str_entry_ptr > string_map ;

	string_map m_appStrings ;
	bool m_is_trans_mode ;

public:
	CAppStrings(UINT lang_id = LANG_ENGLISH );
	~CAppStrings(void);
	void set_trans_mode(bool mode)
	{
		m_is_trans_mode = mode ;
	}

	void load(UINT lang_id = LANG_ENGLISH) ;

	_bstr_t get_auto_trans_sel() ;
	_bstr_t get_get_and_next() ;
	_bstr_t get_set_and_next() ;
	_bstr_t get_get			() ;
	_bstr_t get_next		() ;
	_bstr_t get_prev		() ;
	_bstr_t get_set			() ;
	_bstr_t get_lookup		() ;
	_bstr_t get_lookup_next	() ;
	_bstr_t get_add_gloss	() ;
	_bstr_t get_add_memory	() ;
	_bstr_t get_switch_lang	() ;
	_bstr_t get_auto_sheet	() ;
	_bstr_t get_auto_workbook	() ;
	_bstr_t get_properties	() ;

	_bstr_t get_auto_trans_sel_tt() ;
	_bstr_t get_get_and_next_tt() ;
	_bstr_t get_set_and_next_tt() ;
	_bstr_t get_get_tt() ;
	_bstr_t get_next_tt() ;
	_bstr_t get_prev_tt() ;
	_bstr_t get_set_tt() ;
	_bstr_t get_lookup_tt() ;
	_bstr_t get_lookup_next_tt() ;
	_bstr_t get_add_gloss_tt() ;
	_bstr_t get_add_memory_tt() ;
	_bstr_t get_switch_lang_tt() ;
	_bstr_t get_auto_sheet_tt() ;
	_bstr_t get_auto_workbook_tt() ;
	_bstr_t get_properties_tt	() ;

	_bstr_t get_string( wstring key ) ;
	_bstr_t get_tt_string( wstring key ) ;

private:
	CString get_file_name(UINT lang_id);
};

/**
@class app_str
@brief Singleton class for mainframe.
*/
/*!
 * \brief
 * Write brief comment for app_str here.
 * 
 * Write detailed description for app_str here.
 * 
 * \remarks
 * Write remarks for app_str here.
 * 
 * \see
 * Separate items with the '|' character.
 */
class app_str
{
	// Operations
public:

	/*!
	 * \brief
	 * Get the MainFrame.
	 * 
	 * \param lang_id
	 * Description of parameter lang_id.
	 * 
	 * \returns
	 * Write description of return value here.
	 * 
	 * \throws <exception class>
	 * Description of criteria for throwing this exception.
	 * 
	 * Write detailed description for get here.
	 * 
	 * \remarks
	 * Write remarks for get here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	static CAppStrings& get( UINT lang_id = LANG_ENGLISH ) 
	{ 
		static CAppStrings app_strings ;
		static CAppStrings app_strings_j( LANG_JAPANESE ) ;

		if ( lang_id == LANG_JAPANESE) 
		{
			return app_strings_j ;
		}
		return app_strings ; 
	}

};
