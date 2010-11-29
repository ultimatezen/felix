// word_writer.h: interface for the word_writer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORD_WRITER_H__62356A3F_9D69_4401_9932_CF5B30465A3D__INCLUDED_)
#define AFX_WORD_WRITER_H__62356A3F_9D69_4401_9932_CF5B30465A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "html_writer.h"
#include "WordAutomationObject.h"

class word_writer : public html_writer  
{
	WordSelection	m_selection ;
	WordRange		m_original_range ;
public:
	BOOL	m_write_font_face ;
	BOOL	m_write_font_color ;
	BOOL	m_write_font_bold ;
	BOOL	m_write_font_italic ;
	BOOL	m_write_font_underline ;
	BOOL	m_write_font_super_sub ;
	
public:
	bool formatting_enabled();
	void set_write_font_face( BOOL setting ) ;
	void set_write_font_color( BOOL setting ) ;
	void set_write_font_bold( BOOL setting ) ;
	void set_write_font_italic( BOOL setting ) ;
	void set_write_font_underline( BOOL setting ) ;
	void set_write_font_super_sub( BOOL setting ) ;

public:
	void default_formatting_for_selection();
	// plain text
	void write_plaintext( const wstring &text );
	void apply_formatting( WordFont &font );
	void restore_formatting_values();
	void apply_linebreak();
	void apply_paragraph();
	void write_text( const wstring &text );
	word_writer( const WordSelection &selection );
	virtual ~word_writer();

};

#endif // !defined(AFX_WORD_WRITER_H__62356A3F_9D69_4401_9932_CF5B30465A3D__INCLUDED_)