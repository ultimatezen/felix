#pragma once

#include "html_formatter.h"
#include "PowerPointImports.h"
#include "app_state.h"

class CPPTFormatter
{
	html_formatter				m_formatter ;
	PowerPoint::TextRangePtr	m_range ;
	app_state					m_properties ;

public:
	void set_paragraph_formatting();
	wstring formatRangeText( ) ;

	CPPTFormatter( PowerPoint::TextRangePtr range );
	~CPPTFormatter(void);
};
