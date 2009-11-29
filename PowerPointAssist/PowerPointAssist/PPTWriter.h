#pragma once
#include "html_writer.h"
#include "PowerPointImports.h"
#include "app_state.h"

class CPPTWriter :
	public html_writer
{
	PowerPoint::TextRangePtr m_range ;
	app_state					m_properties ;
public:
	CPPTWriter( PowerPoint::TextRangePtr range );
	~CPPTWriter(void);

	void write_text( const wstring &text ) ;
	void apply_linebreak() ;
	void apply_paragraph() ;
};
