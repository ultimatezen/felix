// WordParser.h: interface for the WordParser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "WordAutomationObject.h"
#include "html_formatter.h"

#include "font_properties.h"

#include "Parser.h"

#include "Broadcaster.h"

class WordParser  : public Parser
{
	WordSelection	m_selection ;
	WordRange		m_original_range ;
	html_formatter	m_formatter ;
	bool			m_eat_whitespace ;

	CStringW		m_SegStopChars ;

public:
	bool select_to_maru();
	bool is_right_quote_selected( const _bstr_t &text );
	bool reflect_bold( WordFont &font, font_properties &props );
	bool reflect_italic( WordFont &font, font_properties &props ) ;
	bool reflect_super_sub( WordFont &font, font_properties &props ) ;
	bool reflect_underline( WordFont &font, font_properties &props ) ;
	bool reflect_color( WordFont &font, font_properties &props ) ;
	bool reflect_face( WordFont &font, font_properties &props ) ; ;

	bool is_whitespace_char( LPCWSTR text );
	bool table_selected( const _bstr_t &text );
	bool formatting_enabled();
	
	void parse_range_text( WordRange &range );
	void parse_segment( );
	void parse_words( WordRange &segment, const font_properties &props );
	void parse_word( WordRange &word, const font_properties &props );
	void parse_characters( WordRange &word, const font_properties &props );
	void parse_character( WordRange &character, const font_properties &props );
	void parse_range( WordRange &range, const font_properties &props  );

	bool select_next_sentence( );
	_bstr_t segment_as_plaintext( );
	_bstr_t segment_as_html( );
	WordParser( WordSelection selection );
	virtual ~WordParser();

private:
	bool eat_whitespace();

};

