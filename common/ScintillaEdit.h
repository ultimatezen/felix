/*!
	@file ScintillaEdit.h
	@brief interface for the CScintillaEdit class.
	@date 2005/06/25
	Time: 15:31:07
	@author Ryan Ginstrom
 */

#pragma once

#include "Exceptions.h"
#include "MessageMapDefines.h"
#include "file.h"
#include "SciLexer.h"
#include "Scintilla_encodings.h"

#define SCINTILLA_FILE_FILTER	TEXT("HTML Files (*.htm, *.html)\0*.htm;*.html\0") \
								TEXT("XML files (*.xml)\0*.xml\0")\
								TEXT("C++ files (*.c;*.cpp;*.cxx;*.h;*.tlh;*.rc)\0*.c;*.cpp;*.cxx;*.h;*.tlh;*.rc\0")\
								TEXT("Text files (*.txt)\0*.txt\0")\
								TEXT("Perl files (*.pl)\0*.pl\0")\
								TEXT("CSS files (*.css)\0*.css\0")\
								TEXT("All files (*.*)\0*.*\0")

UINT get_textfile_encoding( const file::BYTE_ORDER_MARK bom, LPSTR ftext, const size_t fsize, bool is_tagged = false )  ;

/**
	@class CScintillaEdit 
	@brief Wrapper for Scintilla edit control.
 */
class CScintillaEdit 
	: public CScintillaWindowImpl< CScintillaEdit, CWindow >,
	public CRichEditCommands<CScintillaEdit>
{
	VISIBLE_TO_TESTS
	
	typedef CScintillaWindowImpl<CScintillaEdit, CWindow> baseClass;
	enum { SCINTILLA_MARGIN_LINENUM, SCINTILLA_MARGIN_SELECTION, SCINTILLA_MARGIN_FOLD };

public:
	DECLARE_SENSING_VAR ;

	void set_up_standard_syntax_coloring();

	UINT get_working_encoding();

	UINT m_working_encoding;
	void set_working_encoding( const UINT codepage );
	void set_correct_lexer( const CString &location );
	DECLARE_WND_CLASS(NULL)
	
	BOOL PreTranslateMessage(MSG*)
	{
		return FALSE;
	}
	CScintillaEdit() : m_working_encoding(CP_UTF8) {}
	
	BEGIN_MSG_MAP(CScintillaEdit)
		CHAIN_MSG_MAP(baseClass)
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CScintillaEdit>, 1)
	END_MSG_MAP()
		
	int HandleNotify(LPARAM lParam)
	{
		return baseClass::HandleNotify(lParam);
	}

	wstring GetText()
	{
		const int len = GetTextLength() ;
		boost::scoped_array<char> buff(new char[len+1]) ;
		const int num_copied = baseClass::GetText( len+1, buff.get() ) ;
		ATLASSERT(len == num_copied) ;
		buff[num_copied] = 0 ;

		const string text(buff.get(), num_copied) ;
		return string2wstring(text) ;
	}
	wstring GetSelection()
	{
		const int len = GetSelectionEnd() - GetSelectionStart() ;
		boost::scoped_array<char> sel_text_buffer(new char[len+1]) ;
		GetSelText( sel_text_buffer.get() ) ;

		const string text(sel_text_buffer.get()) ;
		return string2wstring(text) ;
	}

	void CollapseSelection(bool end = true )
	{
		CharacterRange rng ;
		GetSel( rng ) ;

		if ( end ) 
		{
			SetSel( rng.cpMax, rng.cpMax ) ;
		}
		else 
		{
			SetSel( rng.cpMin, rng.cpMin ) ;
		}
	}
	void SetText( const string text )
	{
		baseClass::SetText( text.c_str() ) ;
	}
	void SetText( const wstring text )
	{
		if (text.empty())
		{
			baseClass::ClearAll() ;
		}
		else
		{
			baseClass::SetText( string2string(text).c_str() ) ;
		}
	}
	void ReplaceSelection( const wstring text )
	{
		if ( text.empty() )
		{
			baseClass::ReplaceSel( "" ) ;
		}
		else
		{
			baseClass::ReplaceSel( string2string(text).c_str() ) ;
		}
	}

	void SetAStyle(int style, COLORREF fore = RGB( 0, 0, 0 ) /* black */ , COLORREF back = RGB(0xff,0xff,0xff) /* white */ , int size = 0, const char *face = NULL ) 
	{
		StyleSetFore( style, fore ) ;
		StyleSetBack( style, back ) ;

		if (size >= 1)
			StyleSetSize( style, size ) ;
		if (face) 
			StyleSetFont( style, face ) ;
	}

};

