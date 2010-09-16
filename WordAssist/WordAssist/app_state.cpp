// app_state.cpp: implementation of the app_state class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app_state.h"
#include "file.h"
#include <shlobj.h>
#include "Path.h"
#include "logging.h"
#include "Exceptions.h"

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
app_state::app_state() : 
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
	m_really_wants_no_delims(FALSE)
{
	setDefaults() ;
}

void app_state::set_seg_defaults()
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
	m_toolbar_visible = TRUE ;

	set_seg_defaults() ;

	ZeroMemory(m_manual_url, MANUAL_URL_SIZE  * sizeof(TCHAR)) ;
	_tcscpy_s( m_manual_url, _T("http://felix-cat.com/media/manuals/felix/5.html") ) ;

	// preferred GUI language
	LANGID lid = ::GetUserDefaultLangID() ;
	WORD preflang = (WORD)PRIMARYLANGID( lid ) ;

	if ( preflang == LANG_JAPANESE )
	{
		m_preferred_gui_lang = PREF_LANG_JAPANESE ;
	}
	else
	{
		m_preferred_gui_lang = PREF_LANG_ENGLISH ;
	}

	m_classic_if = FALSE ;

	m_font_face = TRUE ;
	m_font_color = TRUE ;
	m_font_bold = TRUE ;
	m_font_italic = TRUE ;
	m_font_underline = TRUE ;
	m_font_super_sub = TRUE ;

	m_segmentation_type = SEG_TYPE_WORD_DOC ;
	m_skipNumbers = TRUE ;
	m_skipJ = NO_SKIP ;

	m_freshInstall = TRUE ;

	m_select_spaces = TRUE ;
	m_use_trans_hist = FALSE ;
	m_shortcuts_active = TRUE ;
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
	CopyMemory( this, &rhs, sizeof(app_state) ) ;
	if (! rhs.m_segChars[0])
	{
		if (m_really_wants_no_delims)
		{
			ZeroMemory(m_segChars, SEG_CHAR_SIZE  * sizeof(TCHAR)) ;
		}
	}
	else if (CString(m_segChars) == _T("NONE"))
	{
		m_really_wants_no_delims = TRUE ;
		m_segChars[0] = 0 ;
	}
	else if (CString(m_segChars) == _T("DEFAULT"))
	{
		m_really_wants_no_delims = FALSE ;
		this->set_seg_defaults() ;
	}
	else
	{
		m_really_wants_no_delims = FALSE ;
		_tcscpy_s( m_segChars, rhs.m_segChars ) ;
	}
	TRACE( m_segChars ) ;
}

void Abbreviations::load(const wstring &text)
{
	std::vector<wstring> tokens ;
	boost::split(tokens, text, boost::is_space()) ;
	if (! tokens.empty())
	{
		m_abbreviations.clear() ;
		foreach(wstring token, tokens)
		{
			if (! token.empty())
			{
				m_abbreviations.insert(token) ;
			}
		}
	}
}

Abbreviations::Abbreviations()
{
	std::vector<wstring> tokens ;
	tokens += L"Mr.", L"Mrs.", L"Ms.", L"Dr.", L"e.g.", L"i.e." ;
	foreach(wstring token, tokens)
	{
		m_abbreviations.insert(token) ;
	}
}

wstring get_config_text(CString filename)
{
	TCHAR szPath[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, // hwndOwner
		CSIDL_LOCAL_APPDATA,		  // nFolder
		(HANDLE)NULL,				  // hToken (-1 means "default user")
		SHGFP_TYPE_CURRENT,			  // dwFlags 
		szPath) ;
	if (! SUCCEEDED(hr))
	{
		logging::log_error("Failed to get local app data folder") ;
		except::CComException e(hr) ;
		logging::log_exception(e) ;
		return wstring();
	}

	const fs::wpath pathname = fs::wpath(static_cast<LPCTSTR>(szPath)) 
		/ _T("Felix") 
		/ static_cast<LPCTSTR>(filename) ;
	CString abbrev_file(pathname.string().c_str()) ;
	if (! file::CPath(abbrev_file).FileExists())
	{
		return wstring() ;
	}
	file::view abbrev_view ;
	string raw_text(static_cast<LPCSTR>(abbrev_view.create_view_readonly(abbrev_file))) ;
	return string2wstring(raw_text, CP_UTF8) ;
}
