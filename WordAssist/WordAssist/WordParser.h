// WordParser.h: interface for the WordParser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "logging.h"
#include "WordAutomationObject.h"
#include "html_formatter.h"
#include "office_seg.h"

#include "font_properties.h"

#include "app_state.h"
#include "Parser.h"

class CParserBase : public OfficeSeg
{

public:
	BOOL	m_parse_font_face ;
	BOOL	m_parse_font_color ;
	BOOL	m_parse_font_bold ;
	BOOL	m_parse_font_italic ;
	BOOL	m_parse_font_underline ;
	BOOL	m_parse_font_super_sub ;

	CParserBase();
	virtual ~CParserBase();

	void set_parse_font_face( BOOL setting ) ;
	void set_parse_font_color( BOOL setting ) ;
	void set_parse_font_bold( BOOL setting ) ;
	void set_parse_font_italic( BOOL setting ) ;
	void set_parse_font_underline( BOOL setting ) ;
	void set_parse_font_super_sub( BOOL setting ) ;

};

class WordParser  : public CParserBase
{
	WordSelection	m_selection ;
	WordRange		m_original_range ;
	html_formatter	m_formatter ;
	bool			m_eat_whitespace ;
	Abbreviations	*m_ok_endings ;

public:
	_bstr_t			m_SegStopChars ;
	bool			m_skipNumbers ;
	int				m_skipJ ;
	bool			m_select_spaces ;

public:
	void setPrefs( app_state *prefs );
	// format toolbar combo selections
	bool OnPreferencesChanged( boost::any &Param );
	bool select_to_maru();
	bool is_right_quote_selected( const _bstr_t &text );
	bool reflect_bold( WordFont &font, font_properties &props );

	bool make_bold_setting( MSOffice::MsoTriState tri_setting, font_properties &props );
	bool reflect_italic( WordFont &font, font_properties &props ) ;

	bool make_italic_setting( MSOffice::MsoTriState tri_setting, font_properties &props );
	bool reflect_super_sub( WordFont &font, font_properties &props ) ;

	bool make_subscript_setting( MSOffice::MsoTriState tri_setting, font_properties &props);
	bool make_superscript_setting( MSOffice::MsoTriState tri_setting, font_properties &props);
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

	wchar_t get_last_char() 
	{
		wstring text = this->get_seg_text() ;
		if (text.empty())
		{
			return wchar_t(0) ;
		}
		size_t last = text.size() -1 ;
		return text[last] ;
	}
	wstring get_seg_text() 
	{
		return BSTR2wstring(this->segment_as_plaintext()) ;
	}


	bool handle_db_period( WordRange &range, long &old_start, long &old_end );
	void handle_not_whitespace( WordRange &range, long &old_end, _bstr_t &text );
	long handle_wdigit( WordRange &range, long &old_end, long &old_start );
	bool handle_period( WordRange &range, long &old_end, long old_start, _bstr_t &text );

	void advance_end(  WordRange &range, long &old_end, _bstr_t &text, _bstr_t &next_char);
	_bstr_t segment_as_plaintext( );
	_bstr_t segment_as_html( );
	WordParser( WordSelection selection, app_state *params, Abbreviations *abbreviations );
#ifdef _DEBUG
	WordParser( ) ;

	void load_abbreviations( const wstring & abbreviations );
#endif
	void load_params();
	~WordParser();
private:
	bool eat_whitespace();

};

