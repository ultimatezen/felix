// app_state.cpp: implementation of the app_state class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app_state.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static const wchar_t maru_char_array[] =
{
	0x0021	// EXCLAMATION MARK
	, 0x002E	// FULL STOP
	, 0x003F	// QUESTION MARK

	, 0x3002	// IDEOGRAPHIC FULL STOP
	, 0xFF61	// HALFWIDTH IDEOGRAPHIC FULL STOP

	, 0xFF0E	// FULLWIDTH FULL STOP
	, 0xFF1F	// FULLWIDTH QUESTION MARK
	, 0xFF01	// FULLWIDTH EXCLAMATION MARK

	, L'\0'		// end of array
} ;

/*!
 * CTOR. Sets default values.
 */
app_state::app_state() 
{
	BANNER( "app_state::app_state()" ) ;
	setDefaults() ;
}

app_state::app_data::app_data() : 
	m_toolbar_visible(TRUE),
	m_preferred_gui_lang(PREF_LANG_ENGLISH),
	m_font_color(TRUE),
	m_font_bold(TRUE),
	m_font_italic(TRUE),
	m_font_underline(TRUE),
	m_skipNumbers(TRUE),
	m_skipJ(NO_SKIP),
	m_freshInstall(TRUE),
	m_select_spaces(TRUE),
	m_navigation_type(NAV_TYPE_ASK),
	m_shortcuts_active(TRUE)
{
	set_segchars_to_defaults() ;
}

void app_state::app_data::set_segchars_to_defaults()
{
	// Segment-ending characters
	ZeroMemory(m_segChars, SEG_CHAR_SIZE  * sizeof(TCHAR)) ;
	CString segChars = CW2T( maru_char_array ) ;
	// automatically calculates size of destination
	_tcscpy_s( m_segChars, segChars ) ;
	ATLASSERT( _tcslen( m_segChars) == static_cast<size_t>(segChars.GetLength())) ;
}
	/*!
	* Sets default values.
	*/
void app_state::setDefaults()
{
	BANNER( "app_state::setDefaults" ) ;
	m_data.m_toolbar_visible = TRUE ;

	m_data.set_segchars_to_defaults() ;

	// preferred GUI language
	LANGID lid = ::GetUserDefaultLangID() ;

	if ( (WORD)PRIMARYLANGID( lid ) == LANG_JAPANESE )
	{
		m_data.m_preferred_gui_lang = PREF_LANG_JAPANESE ;
	}
	else
	{
		m_data.m_preferred_gui_lang = PREF_LANG_ENGLISH ;
	}

	m_data.m_font_color = TRUE ;
	m_data.m_font_bold = TRUE ;
	m_data.m_font_italic = TRUE ;
	m_data.m_font_underline = TRUE ;
	m_data.m_skipNumbers = TRUE ;
	m_data.m_skipJ = NO_SKIP ;
	m_data.m_freshInstall = TRUE ;
	m_data.m_select_spaces = TRUE ;
	m_data.m_navigation_type = NAV_TYPE_ASK ;
	m_data.m_shortcuts_active = TRUE ;
}


app_state::app_state( const app_state &rhs )
{
	BANNER( "app_state::app_state( const app_state &rhs )" ) ;

	CopyMemory( &m_data, &(rhs.m_data), sizeof(app_data) ) ;
	_tcscpy_s( m_data.m_segChars, rhs.m_data.m_segChars ) ;
	TRACE( m_data.m_segChars ) ;
}


app_state &app_state::operator=( const app_state &rhs )
{
	BANNER( "app_state::operator=( const app_state &rhs )" ) ;

	CopyMemory( &m_data, &(rhs.m_data), sizeof(app_data) ) ;
	_tcscpy_s( m_data.m_segChars, rhs.m_data.m_segChars ) ;
	TRACE( m_data.m_segChars ) ;

	return *this ;
}


