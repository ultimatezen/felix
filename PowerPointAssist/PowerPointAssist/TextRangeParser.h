#pragma once

#include "PowerPointImports.h"
#include "globalApp.h"
#include "app_state.h"
#include "abbreviations.h"

class CTextRangeParser
{
public:
	TextRangePtr	m_range ;
	TextRangePtr	m_currentSegment ;

	long m_start;
	app_state		*m_properties ;
	Abbreviations	*m_abbreviations ;

	CTextRangeParser(Abbreviations *abbreviations);
	~CTextRangeParser(void);
	void setProperties(app_state *properties)
	{
		m_properties = properties ;
	}

	_bstr_t getSelectedText( bool isPlainText = false );
	bool selectNext(void);
	void setRange(TextRangePtr range) ;
	bool selectForSet(void);
	TextRangePtr getSetRange(void);
	bool extendSelection(void) ;
	void collapseSelection( );
	TextRangePtr eatWhitespace(TextRangePtr& range);

	_bstr_t selectionToPlainText();

	_bstr_t selectionToRichText();
	void setRangeFromSelection(void);
};
