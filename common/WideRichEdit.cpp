// WideRichEdit.cpp: implementation of the CWideRichEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WideRichEdit.h"
#include "Exceptions.h"
#include "StringConversions.h"
#include "ScintillaEdit.h"
#include "ConvertUTF.h"
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



UINT CWideRichEdit::detect_encoding(const CString &location, input_device_ptr input)
{
	static const size_t HTML_HEAD_LEN = 1024 ;
	static const size_t XML_HEAD_LEN = 256 ;

	const size_t fsize = input->get_size(location) ;
	
	// check for bom
	const file::file::BYTE_ORDER_MARK bom = input->get_file_bom(location) ;
	
	LPSTR ftext = input->create_view_char(location) ;
	
	file::name fname( location ) ;

	CString ext = fname.extension_part() ;

	ext.MakeLower() ;

	if ( ext == _T(".html") || ext == _T(".htm") )
	{
		string html_head( ftext, HTML_HEAD_LEN ) ;

		boost::to_lower( html_head ) ;

		textstream_reader< char > html_reader ;
		
		html_reader.set_buffer( html_head.c_str() ) ;

		while ( html_reader.find( "<meta", true ) )
		{
			const string meta_str = html_reader.getline('>') ;
			textstream_reader< char > meta_reader ;
			meta_reader.set_buffer( meta_str.c_str() ) ;
			if ( meta_reader.find( "charset", true  ) )
			{
				meta_reader.find( "=", true ) ;
				meta_reader.eat_whitespace() ;
				const string charset_str = meta_reader.getline('\"') ;
				m_working_encoding = sci_encoding_from_encoding_string( charset_str ) ;
				return m_working_encoding ;
			}
			
		}

	}
	else if ( ext == _T(".xml") )
	{
		string xml_head( ftext, XML_HEAD_LEN ) ;
		
		boost::to_lower( xml_head ) ;
		
		textstream_reader< char > xml_reader ;
		
		xml_reader.set_buffer( xml_head.c_str() ) ;
		
		if( xml_reader.find( "encoding" ) )
		{
			xml_reader.find( "\"", true ) ;
			xml_reader.eat_whitespace() ;
			const string charset_str = xml_reader.getline('\"') ;
			m_working_encoding = sci_encoding_from_encoding_string( charset_str ) ;
			return m_working_encoding ;
		}
	}
		
		
	switch( bom )
	{
	case file::file::LE_BOM:
		m_working_encoding = CP_UNICODE ;
		break ;
	case file::file::BE_BOM:
		m_working_encoding = CP_UNICODE_BE ;
		break ;
	case file::file::UTF8_BOM:
		m_working_encoding = CP_UTF8 ;
		break ;
	case file::file::UTF7_BOM:
		m_working_encoding = CP_UTF7 ;
		break ;
	default:
		try
		{
			CMultiLanguage multi ;
		
			DWORD flag = 0 ;
			
			if ( ext == _T(".html") || ext == _T(".htm") || ext == _T(".xml") )
			{
				flag |= MLDETECTCP_HTML ;
			}
			
			const DetectEncodingInfo detect = multi.detect_encoding( ftext, fsize, flag ) ;
			
			m_working_encoding = detect.nCodePage ;
			
			break ;
			
		}
		catch ( CComException &e ) 
		{
			e.notify_user( CString( _T("Failed to auto-detect input character encoding") ) ) ;
			m_working_encoding = CP_ACP ;
		}
	}

	return m_working_encoding ;

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


void CWideRichEdit::set_correct_lexer(const CString &location)
{
	if ( location.IsEmpty() ) return ;
	
	file::CPath path( location ) ;
	
	CString ext = path.FindExtension( ) ;
	ext.MakeLower() ;
	
	ATLASSERT( ext[0] == _T('.') ) ;
	
	if ( ext == _T(".html") || ext == _T(".htm") ) 
	{
		m_lexer = ( SCLEX_HTML ) ;
	}
	else if ( ext == _T(".xml") ) 
	{
		m_lexer = ( SCLEX_XML ) ;	
	}
	else if ( ext == _T(".css") )
	{
		m_lexer = ( SCLEX_CSS ) ;
	}
	else if ( ext == _T(".cpp") || ext == _T(".cxx") || ext == _T(".h")  || ext == _T(".c")  )
	{
		m_lexer = ( SCLEX_CPP ) ;
	}
	else if ( ext == _T(".pl") || ext == _T(".cgi") )
	{
		m_lexer = ( SCLEX_PERL ) ;
	}
	else if ( ext == _T(".mak") )
	{
		m_lexer = ( SCLEX_MAKEFILE ) ;
	}
	else if ( ext == _T(".bat") )
	{
		m_lexer = ( SCLEX_BATCH ) ;
	}
	else if ( ext == _T(".bas") || ext == _T("fm") )
	{
		m_lexer = ( SCLEX_VB ) ;
	}
	else if ( ext == _T(".vbs")  )
	{
		m_lexer = ( SCLEX_VBSCRIPT ) ;
	}
	else
	{
		m_lexer = ( SCLEX_AUTOMATIC );
	}
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

