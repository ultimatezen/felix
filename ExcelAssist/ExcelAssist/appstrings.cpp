/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#pragma warning( disable : 4996 ) 

#include "StdAfx.h"
#include "AppStrings.h"

#include "resource.h"
#include "file.h"
#include "AppStringRecordParser.h"

/*!
 * CTOR with language ID.
 */
CAppStrings::CAppStrings( UINT lang_id ) : m_is_trans_mode(true)
{
	load( lang_id )	;
}

/*!
 * Load app strings.
 */
void CAppStrings::load(UINT lang_id )
{
	CString filename = get_file_name(lang_id);

	HINSTANCE hInst ;
#ifdef UNIT_TEST
	hInst = NULL ;
#else
	hInst = _AtlModule.GetResourceInstance()
#endif


	HRSRC hrsrc = FindResource(hInst, filename, RT_HTML);	// リソースに組み込んだ時の名前が「jpg」の場合
	DWORD dwFileSize = SizeofResource(hInst, hrsrc);
	HANDLE hres = LoadResource(hInst, hrsrc);

	// It is not necessary to unlock resources because the system automatically 
	// deletes them when the process that created them terminates.
	char* pcPos = (char*)LockResource(hres);

	string file_text(pcPos, static_cast< size_t >( dwFileSize ));
	wstring wide_text = string2wstring( file_text, CP_UTF8 ) ;

	CAppStringRecordParser rec_parser( wide_text.c_str() ) ;

	while ( rec_parser.had_more_records() ) 
	{
		app_str_entry_ptr entry = rec_parser.get_next_record() ;

		m_appStrings[entry->get_entry(L"id")] = entry ;
	}
}

/*!
 * Retrieves the resource file name for the language ID.
 */
CString CAppStrings::get_file_name(UINT lang_id)
{
	if ( lang_id == LANG_JAPANESE )
	{
		return CString("JAPANESE.HTML") ;
	}
	return CString("ENGLISH.HTML") ;
}


CAppStrings::~CAppStrings(void)
{
}

/*!
 * Retrieves a caption string.
 */
_bstr_t CAppStrings::get_string ( wstring key )
{
	ATLASSERT( m_appStrings.find( key ) != m_appStrings.end( ) ) ;

	if ( m_appStrings.find( key ) == m_appStrings.end( ) ) 
	{
		return _bstr_t(key.c_str()) ;
	}
	_bstr_t text ;
	if (m_is_trans_mode)
	{
		text = m_appStrings[key]->get_entry(L"text").c_str() ;
	}
	else
	{
		text = m_appStrings[key]->get_entry(L"trans").c_str() ;
	}
	return text ;
}
/*!
 * Retrieves a tooltip string.
 */
_bstr_t CAppStrings::get_tt_string ( wstring key )
{
	ATLASSERT( m_appStrings.find( key ) != m_appStrings.end( ) ) ;

	if ( m_appStrings.find( key ) == m_appStrings.end( ) ) 
	{
		return _bstr_t(key.c_str()) ;
	}
	_bstr_t text ;
	if (m_is_trans_mode)
	{
		text = m_appStrings[key]->get_entry(L"tooltip").c_str() ;
	}
	else
	{
		text = m_appStrings[key]->get_entry(L"trans_tt").c_str() ;
	}
	return text ;
}

//////////////////////////////////////////////////////////////////////////
// captions
//////////////////////////////////////////////////////////////////////////

/*!
 * Retrieves the string for "auto translate selection".
 */
_bstr_t CAppStrings::get_auto_trans_sel() 
{
	return get_string( L"auto_trans_sel" ) ;
}
/*!
 * Retrieves the string for "get and next".
 */
_bstr_t CAppStrings::get_get_and_next() 
{
	return get_string( L"get_and_next" ) ;
}

_bstr_t CAppStrings::get_set_and_next() 
{
	return get_string( L"set_and_next" ) ;
}
_bstr_t CAppStrings::get_get() 
{
	return get_string( L"get" ) ;
}
_bstr_t CAppStrings::get_next() 
{
	return get_string( L"next" ) ;
}
_bstr_t CAppStrings::get_prev() 
{
	return get_string( L"prev" ) ;
}
_bstr_t CAppStrings::get_set() 
{
	return get_string( L"set" ) ;
}
_bstr_t CAppStrings::get_lookup() 
{
	return get_string( L"lookup" ) ;
}
_bstr_t CAppStrings::get_lookup_next() 
{
	return get_string( L"lookup_next" ) ;
}
_bstr_t CAppStrings::get_add_gloss() 
{
	return get_string( L"add_gloss" ) ;
}
_bstr_t CAppStrings::get_add_memory() 
{
	return get_string( L"add_memory" ) ;
}
_bstr_t CAppStrings::get_switch_lang() 
{
	return get_string( L"switch_lang" ) ;
}
_bstr_t CAppStrings::get_auto_sheet() 
{
	return get_string( L"auto_trans_sheet" ) ;
}
_bstr_t CAppStrings::get_auto_workbook() 
{
	return get_string( L"auto_trans_all" ) ;
}
_bstr_t CAppStrings::get_properties() 
{
	return get_string( L"properties" ) ;
}

// Tooltips
_bstr_t CAppStrings::get_auto_trans_sel_tt() 
{
	return get_tt_string( L"auto_trans_sel" ) ;
}
_bstr_t CAppStrings::get_get_and_next_tt() 
{
	return get_tt_string( L"get_and_next" ) ;
}
_bstr_t CAppStrings::get_set_and_next_tt() 
{
	return get_tt_string( L"set_and_next" ) ;
}
_bstr_t CAppStrings::get_get_tt() 
{
	return get_tt_string( L"get" ) ;
}
_bstr_t CAppStrings::get_next_tt() 
{
	return get_tt_string( L"next" ) ;
}
_bstr_t CAppStrings::get_prev_tt() 
{
	return get_tt_string( L"prev" ) ;
}
_bstr_t CAppStrings::get_set_tt() 
{
	return get_tt_string( L"set" ) ;
}
_bstr_t CAppStrings::get_lookup_tt() 
{
	return get_tt_string( L"lookup" ) ;
}
_bstr_t CAppStrings::get_lookup_next_tt() 
{
	return get_tt_string( L"lookup_next" ) ;
}
_bstr_t CAppStrings::get_add_gloss_tt() 
{
	return get_tt_string( L"add_gloss" ) ;
}
_bstr_t CAppStrings::get_add_memory_tt() 
{
	return get_tt_string( L"add_memory" ) ;
}
_bstr_t CAppStrings::get_switch_lang_tt() 
{
	return get_tt_string( L"switch_lang" ) ;
}
_bstr_t CAppStrings::get_auto_sheet_tt() 
{
	return get_tt_string( L"auto_trans_sheet" ) ;
}
_bstr_t CAppStrings::get_auto_workbook_tt() 
{
	return get_tt_string( L"auto_trans_all" ) ;
}
_bstr_t CAppStrings::get_properties_tt() 
{
	return get_tt_string( L"properties" ) ;
}

