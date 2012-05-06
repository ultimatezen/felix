// WideRichEdit.cpp: implementation of the CWideRichEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WideRichEdit.h"
#include "Exceptions.h"
#include "StringConversions.h"
#include "ScintillaEdit.h"
#include "File.h"
#include "MultiLanguage.h"
#include "textstream_reader.h"
#include "DebugUtilities.h"
#include "logging.h"
#include "input_device_file.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace except ;

// Function name	: WideStreamInCallback
DWORD __stdcall WideStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	BANNER( "WideStreamInCallback" ) ;

	wstring *psBuffer = (wstring *)dwCookie;

	ATLASSERT( psBuffer != NULL ) ;
	if (!psBuffer)
	{
		logging::log_warn("psBuffer NULL in WideStreamInCallback") ;
		return 0 ;
	}
	if ( static_cast<size_t>(cb) > ( psBuffer->length() * sizeof( wchar_t ) ) )
		cb = psBuffer->length() * sizeof( wchar_t ) ;

	if ( cb % 2 != 0 ) --cb ;
	if ( cb != 0 ) 
	{
		memcpy( pbBuff, psBuffer->substr(0, cb / sizeof( wchar_t ) ).c_str(), cb ) ;		
	}

	*pcb = cb;

	psBuffer->erase(0, cb / sizeof( wchar_t ) ) ;

	return 0;
}



// Function name	: WideStreamOutCallback
DWORD __stdcall WideStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	BANNER( "WideStreamOutCallback" ) ;

	wstring *psBuffer = (wstring *)dwCookie;

	psBuffer->append( (const wchar_t*)pbBuff, cb / sizeof( wchar_t ) ) ;

	*pcb = cb ;
	return 0;
}

// Function name	: CWideRichEdit::CWideRichEdit
CWideRichEdit::CWideRichEdit()
{
	m_set_text = boost::bind(&CWideRichEdit::SetTextBuffer, this, _1) ;
}


// Function name	: CWideRichEdit::~CWideRichEdit
CWideRichEdit::~CWideRichEdit()
{
}


// Function name	: CWideRichEdit::ReplaceSelection
void CWideRichEdit::ReplaceSelection(wstring &text)
{
	BANNER( "CWideRichEdit::ReplaceSelection" ) ;

	EDITSTREAM es ;

	es.dwCookie = (DWORD)&text ; // Pass the callback function a pointer to the CString
	es.pfnCallback = WideStreamInCallback; // The callback function

	// unicode can't be combined with rtf
	const UINT format = SFF_SELECTION | SF_TEXT | SF_UNICODE ;
	StreamIn(format, es) ;

	if ( es.dwError != 0 )
	{
		CString error_message ;
		error_message += _T("Streamin Error in RichEdit control.\r") ;
		error_message += _T("Failed to replace text selection.\r") ;
		error_message += _T("Attempted to set selection to: ") + CString(text.c_str()) + CString("]") ;
		throw CWinException( error_message, es.dwError ) ;
	}

}




// Function name	: CWideRichEdit::InsertText
// Description	    : 
// Return type		: void 
// Argument         :  int pos
// Argument         : wstring &text
void CWideRichEdit::InsertText( int pos, wstring &text ) 
{
	BANNER( "CWideRichEdit::InsertText" ) ;
	SetSel( pos, pos ) ;
	ReplaceSelection( text ) ;
}

// Function name	: CWideRichEdit::AppendText
// Description	    : 
// Return type		: void 
// Argument         : wstring &text
void CWideRichEdit::AppendText( wstring &text )
{
	BANNER( "CWideRichEdit::AppendText" ) ;

	InsertText( GetTextLengthW(), text ) ;
}


// Function name	: CWideRichEdit::GetSelection
// Description	    : 
// Return type		: wstring 
wstring CWideRichEdit::GetSelection()
{
	BANNER( "CWideRichEdit::GetSelection" ) ;

	wstring text ;

	EDITSTREAM es ;

	es.dwCookie = (DWORD)&text ; // Pass a pointer to the CString to the callback function
	es.pfnCallback = WideStreamOutCallback; // Specify the pointer to the callback function

	StreamOut( SF_UNICODE | SF_TEXT | SFF_SELECTION , es) ;
	if ( es.dwError != 0 )
	{
		CString error_message = _T("Streamout Error in RichEdit control.\rFailed to get text selection.") ;
		throw CWinException( error_message, es.dwError ) ;
	}
	return text ; 
}



BOOL CWideRichEdit::GetModified()
{
	return GetModify() ;
}


void CWideRichEdit::SetSavePoint()
{
	SetModify( FALSE ) ;
}

UINT CWideRichEdit::get_working_encoding()
{
	return m_working_encoding ;
}

void CWideRichEdit::set_working_encoding(UINT encoding)
{
	m_working_encoding = encoding ;
}

int CWideRichEdit::SetText(LPCWSTR text)
{
	ATLASSERT(IsWindow());
	SETTEXTEX ste = { 0 };
	ste.flags = ST_DEFAULT;
	ste.codepage = CP_UNICODE;
	return (int)::SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)text);
}

//! Set text with streamin callback.
void CWideRichEdit::SetText(const wstring &text) 
{
	SetText(text.c_str()) ;
}

int CWideRichEdit::ReplaceSelection(LPCWSTR text)
{
	ATLASSERT(IsWindow());
	SETTEXTEX ste = { 0 };
	ste.flags = ST_SELECTION ;
	ste.codepage = CP_UNICODE;
	return (int)::SendMessage(m_hWnd, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)text);
}

void CWideRichEdit::CollapseSelection(bool end)
{
	CHARRANGE rng ;
	GetSel( rng ) ;
		
	if ( end ) SetSel( rng.cpMax, rng.cpMax ) ;
	else SetSel( rng.cpMin, rng.cpMin ) ;
}

void CWideRichEdit::CharRightExtend()
{
	CHARRANGE rng ;
	GetSel( rng ) ;

	rng.cpMax++ ;

	SetSel( rng ) ;
}

// Function name	: CWideRichEdit::GetText
// Description	    : 
// Return type		: wstring 
wstring CWideRichEdit::GetText( ) 
{
	CString text ;
	this->GetWindowText(text) ;
	return wstring(CT2W(text)) ;
}


int CWideRichEdit::GetTextLengthW()
{
	return GetTextLengthEx( GTL_USECRLF, CP_UNICODE ) ;
}

