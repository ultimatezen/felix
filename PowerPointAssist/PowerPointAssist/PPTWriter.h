#pragma once
#include "html_writer.h"
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


	void write_html( const wstring html_text );

};
