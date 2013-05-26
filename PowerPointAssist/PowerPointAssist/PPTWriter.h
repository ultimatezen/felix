#pragma once
#include "PowerPointImports.h"
#include "app_state.h"
#include "html_parser.h"

class CPPTWriter :
	public writer_interface
{
	PowerPoint::TextRangePtr m_range ;
	app_state					m_properties ;
	html_parser m_parser ;
public:
	CPPTWriter( PowerPoint::TextRangePtr range );
	~CPPTWriter(void);

	void write_text( const wstring &text ) ;
	void apply_linebreak() ;
	void apply_paragraph() ;

	void select_end( long start, int text_len );
	void check_color( PowerPoint::FontPtr font );
	void check_underline( PowerPoint::FontPtr font );
	void check_italic( PowerPoint::FontPtr font );
	void check_bold( PowerPoint::FontPtr font );

	void apply_paragraph_formatting();

	void check_justification( PowerPoint::ParagraphFormatPtr paragraph_format );

	void write_html( const wstring html_text );

};
