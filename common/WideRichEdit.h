/*!
	@file WideRichEdit.h
	@brief interface for the CWideRichEdit class.
	@date 2005/06/25
	Time: 15:34:56
	@author Ryan Ginstrom
 */

#pragma once

#include <atlbase.h>			// must be included before atlapp.h
#include <atlapp.h>				// must be included before atlctrls.h
#include "atlctrls.h"
#include "StringEx.h"
#include "StringConversions.h"
#include "input_device.h"

#if _RICHEDIT_VER	< 0x0300
#error CWideRichEdit requires Rich Edit version 3.0 or higher.
#endif

/**
	@class CCommandEdit 
	@brief Edit class handling keyboard shortcuts for cut/paste etc.
 */
class CCommandEdit : public CWindowImpl<CCommandEdit, CEditT<TWindow> >,
					public CEditCommands<CCommandEdit>
{
	public:
	
	BEGIN_MSG_MAP(CCommandEdit)
		CHAIN_MSG_MAP_ALT(CEditCommands<CCommandEdit>, 1)
	END_MSG_MAP()
} ;

/**
	@class CWideRichEdit 
	@brief Wrapper for RichEdit control.
	Also handles keyboard shortcuts for cut/paste etc.
 */
class CWideRichEdit : public CWindowImpl<CWideRichEdit, CRichEditCtrlT<TWindow> >,
                      public CRichEditCommands<CWideRichEdit>
{
protected:
	ULONG m_lexer ;
	ULONG m_working_encoding ;

public:
	boost::function< int( LPCWSTR ) > m_set_text ;

#ifndef _UNICODE
	LONG FindText(DWORD dwFlags, FINDTEXTEXW *ft) const
	{
		ATLASSERT(IsWindow());
		return (LONG)::SendMessage(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM)ft);
	}
#endif

	DECLARE_WND_SUPERCLASS(NULL, TEXT("RichEdit20W"))

	int GetTextLengthW();

	ULONG GetLexer() { return m_lexer ; }

	// getting text
	wstring GetText( ) ;
	wstring GetSelection() ;

	void CharRightExtend();
	void CollapseSelection( bool end = true );
	int ReplaceSelection( LPCWSTR text );
	void set_working_encoding( UINT encoding );
	UINT get_working_encoding();
	void SetSavePoint();

	BOOL GetModified();
	void SelectAll()
	{
		this->SetSel(0, -1) ;
	}
	CWideRichEdit();
	virtual ~CWideRichEdit();

	// setting text

	int SetTextBuffer(LPCWSTR text)
	{
		return SetText(text) ;
	}

	int SetText(LPCWSTR text);
	void SetText(const wstring &text) ;

	void ReplaceSelection( wstring &text );

	void InsertText( int pos, wstring &text ) ;
	void AppendText( wstring &text ) ;


	virtual BOOL PreTranslateMessage(MSG*)
	{
		return FALSE ;
	}

	// ==========================
	// message map
	// ==========================

	BEGIN_MSG_MAP(CWideRichEdit)
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CWideRichEdit>, 1)
	END_MSG_MAP()
};

/** Streamin callback function.
 */
DWORD __stdcall WideStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb) ;
/** Streamout callback function.
*/
DWORD __stdcall WideStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb) ;
