// word_writer.cpp: implementation of the word_writer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "word_writer.h"
#include "tag_stripper.h"
#include "logging.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace except ;

word_writer::word_writer( const WordSelection &selection ) : 
	m_selection( selection ),
	m_write_font_face( TRUE ),
	m_write_font_color( TRUE ),
	m_write_font_bold( TRUE ),
	m_write_font_italic( TRUE ),
	m_write_font_underline( TRUE ),
	m_write_font_super_sub( TRUE ),
	m_parser(this)
{
	if ( m_selection.is_instantiated())
	{
		default_formatting_for_selection() ;
	}
	init_state();
}

word_writer::~word_writer()
{

}

void word_writer::write_html( const wstring html_text )
{
	m_parser.write_html(html_text) ;
}

void word_writer::set_write_font_face( BOOL setting )
{
	m_write_font_face = setting ; 
}
void word_writer::set_write_font_color( BOOL setting )
{
	m_write_font_color = setting ; 
}
void word_writer::set_write_font_bold( BOOL setting )
{
	m_write_font_bold = setting ; 
}
void word_writer::set_write_font_italic( BOOL setting )
{
	m_write_font_italic = setting ; 
}
void word_writer::set_write_font_underline( BOOL setting )
{
	m_write_font_underline = setting ; 
}
void word_writer::set_write_font_super_sub( BOOL setting )
{
	m_write_font_super_sub = setting ;
}
bool word_writer::formatting_enabled()
{
	if ( m_write_font_face ) return true ; 
	if ( m_write_font_color ) return true ; 
	if ( m_write_font_bold ) return true ; 
	if ( m_write_font_italic ) return true ; 
	if ( m_write_font_underline ) return true ; 
	if ( m_write_font_super_sub ) return true ;
	
	return false ;
}
void word_writer::write_text(const wstring &text)
{
	try
	{
		m_selection.put_text( wstring( ) ) ;
		WordRange range = m_selection.get_range() ;
		m_original_range = range ;

		range.collapse_end() ;
		range.put_text( text.c_str() ) ;

		if ( formatting_enabled() ) 
		{
			WordFont font = range.get_font() ;
			apply_formatting( font ) ;
		}

		range.collapse_end() ;
		range.select() ;
	}
	catch (CException& e)
	{
		ATLTRACE(e.format_message_for_message_box()) ;
		ATLASSERT(FALSE && "Error writing HTML") ;	
		logging::log_warn("Error writing rich text; falling back to plain text") ;
		logging::log_exception(e) ;
		m_original_range.select() ;
		this->write_plaintext(text) ;
	}
	catch (_com_error& ce)
	{
		logging::log_warn("COM Error writing rich text; falling back to plain text") ;
		logging::log_exception(ce) ;
		m_original_range.select() ;
		this->write_plaintext(text) ;
	}

}

void word_writer::apply_paragraph()
{
	m_selection.type_paragraph() ;
}

void word_writer::apply_linebreak()
{
	m_selection.type_text( _bstr_t(L"\n" ) ) ;
}

void word_writer::restore_formatting_values()
{
	if ( formatting_enabled() ) 
	{
		m_parser.restore_original_states() ;
		
		WordRange word_range = m_selection.get_range() ;
		
		WordFont font = word_range.get_font() ;
		
		apply_formatting( font ) ;
	}
}

void word_writer::apply_formatting(WordFont &font)
{
	// bold
	if ( m_write_font_bold ) 
	{
		if ( m_parser.get_bold() )
		{
			font.put_bold_on() ;
		}
		else
		{
			font.put_bold_off() ;
		}
	}
	
	// italic
	if ( m_write_font_italic )
	{
		if ( m_parser.get_italic() )
		{
			font.put_italic_on() ;
		}
		else
		{
			font.put_italic_off() ;
		}
	}
	
	// underline
	if ( m_write_font_underline )
	{
		if ( m_parser.get_underline() )
		{
			font.put_underline_on() ;
		}
		else
		{
			font.put_underline_off() ;
		}
	}
	
	// fore color
	if ( m_write_font_color )
	{
		COLORREF color = m_parser.get_fore_color() ;
		font.set_rgb_color( color ) ;
	}

	// subscript/superscript
	if ( m_write_font_super_sub )
	{
		html_processing::VERT_ALIGNMENT valign = m_parser.get_v_align() ;
		font.set_v_align( valign ) ;
	}

	// font face
	if ( m_write_font_face )
	{
		wstring font_face = m_parser.get_font_face() ;
		wstring font_label( font_face ) ;
		font.set_face( font_label ) ;
	}
}

void word_writer::write_plaintext(const wstring &text)
{
	m_selection.put_text( wstring( ) ) ;

	default_formatting_for_selection() ;

	WordRange word_range = m_selection.get_range() ;

	wstring plain_text( strip_tags( text ) ) ;
	word_range.put_text( plain_text.c_str() ) ;
	word_range.collapse_end() ;
	word_range.select() ;

	WordFont font = word_range.get_font() ;
	apply_formatting( font ) ;
}

void word_writer::default_formatting_for_selection()
{
	WordRange word_range = m_selection.get_range() ;
	
	WordFont font = word_range.get_font() ;
	
	// set default values
	m_parser.init_bold( font.is_bold_on() ) ;
	m_parser.init_italic( font.is_italic_on() ) ;
	m_parser.init_underline( font.is_underline_on() ) ;
	m_parser.init_fore_color( CColorRef( font.get_rgb_color() ) ) ;
	m_parser.init_font_face( wstring( font.get_face() ) ) ;
}
