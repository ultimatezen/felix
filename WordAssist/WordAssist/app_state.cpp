// app_state.cpp: implementation of the app_state class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app_state.h"
#include "file.h"

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
	setDefaults() ;
}

void app_state::set_seg_defaults()
{
	// Segment-ending characters
	ZeroMemory(m_data.m_segChars, SEG_CHAR_SIZE  * sizeof(TCHAR)) ;
	CString segChars = CW2T( maru_char_array ) ;
	// automatically calculates size of destination
	_tcscpy_s( m_data.m_segChars, segChars ) ;
	ATLASSERT( _tcslen( m_data.m_segChars) == static_cast<size_t>(segChars.GetLength())) ;
}

/*!
* Sets default values.
*/
void app_state::setDefaults()
{
	m_data.m_toolbar_visible = TRUE ;

	set_seg_defaults() ;

	ZeroMemory(m_data.m_manual_url, MANUAL_URL_SIZE  * sizeof(TCHAR)) ;
	_tcscpy_s( m_data.m_manual_url, _T("http://felix-cat.com/media/manuals/felix/5.html") ) ;

	// preferred GUI language
	LANGID lid = ::GetUserDefaultLangID() ;
	WORD preflang = (WORD)PRIMARYLANGID( lid ) ;

	if ( preflang == LANG_JAPANESE )
	{
		m_data.m_preferred_gui_lang = PREF_LANG_JAPANESE ;
	}
	else
	{
		m_data.m_preferred_gui_lang = PREF_LANG_ENGLISH ;
	}

	m_data.m_classic_if = FALSE ;

	m_data.m_font_face = TRUE ;
	m_data.m_font_color = TRUE ;
	m_data.m_font_bold = TRUE ;
	m_data.m_font_italic = TRUE ;
	m_data.m_font_underline = TRUE ;
	m_data.m_font_super_sub = TRUE ;

	m_data.m_segmentation_type = SEG_TYPE_WORD_DOC ;
	m_data.m_skipNumbers = TRUE ;
	m_data.m_skipJ = NO_SKIP ;

	m_data.m_freshInstall = TRUE ;

	m_data.m_select_spaces = TRUE ;
	m_data.m_use_trans_hist = FALSE ;
	m_data.m_shortcuts_active = TRUE ;

	m_data.m_raise_felix = FALSE ;
}

/*!
 * \brief
 * copy constructor.
 */
app_state::app_state( const app_state &rhs )
{
	BANNER( "app_state::app_state( const app_state &rhs )" ) ;
	this->_internal_copy(rhs) ;
}

/*!
 * \brief
 * assignment operator.
 */
app_state &app_state::operator=( const app_state &rhs )
{
	BANNER( "app_state::operator=( const app_state &rhs )" ) ;

	this->_internal_copy(rhs) ;
	return *this ;
}

void app_state::_internal_copy ( const app_state &rhs )
{
	CopyMemory( &(this->m_data), &(rhs.m_data), sizeof(app_state_data) ) ;
	if (! rhs.m_data.m_segChars[0])
	{
		if (m_data.m_really_wants_no_delims)
		{
			ZeroMemory(m_data.m_segChars, SEG_CHAR_SIZE  * sizeof(TCHAR)) ;
		}
	}
	else if (CString(m_data.m_segChars) == _T("NONE"))
	{
		m_data.m_really_wants_no_delims = TRUE ;
		m_data.m_segChars[0] = 0 ;
	}
	else if (CString(m_data.m_segChars) == _T("DEFAULT"))
	{
		m_data.m_really_wants_no_delims = FALSE ;
		this->set_seg_defaults() ;
	}
	else
	{
		m_data.m_really_wants_no_delims = FALSE ;
		_tcscpy_s( m_data.m_segChars, rhs.m_data.m_segChars ) ;
	}
	TRACE( m_data.m_segChars ) ;
}


app_state::app_state_data::app_state_data(): 
	m_toolbar_visible(TRUE),

	m_preferred_gui_lang(PREF_LANG_ENGLISH),
	m_classic_if(FALSE),

	m_font_face(TRUE),
	m_font_color(TRUE),
	m_font_bold(TRUE),
	m_font_italic(TRUE),
	m_font_underline(TRUE),
	m_font_super_sub(TRUE),

	m_segmentation_type(SEG_TYPE_WORD_DOC),
	m_skipNumbers(TRUE),
	m_skipJ(NO_SKIP),
	m_select_spaces(TRUE),

	m_freshInstall(TRUE),

	m_use_trans_hist(FALSE),
	m_shortcuts_active(TRUE),
	m_really_wants_no_delims(FALSE),

	m_raise_felix(FALSE)
{
	ZeroMemory(m_segChars, SEG_CHAR_SIZE * sizeof(TCHAR)) ;
	ZeroMemory(m_manual_url, MANUAL_URL_SIZE * sizeof(TCHAR)) ;
}
