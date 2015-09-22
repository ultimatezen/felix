// WordParser.cpp: implementation of the WordParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordParser.h"
#include "StringConversions.h"
#include "textstream_reader.h"
#include "app_state.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define WORD_TABLE_MARKER 7

#define LEFT_SINGLE_QUOTATION_MARK		0x2018
#define RIGHT_SINGLE_QUOTATION_MARK		0x2019	
#define LEFT_DOUBLE_QUOTATION_MARK		0x201C	
#define RIGHT_DOUBLE_QUOTATION_MARK		0x201D	

static const wchar_t whitespace_char_array[] =
{
	  0x0001	// START OF HEADING
	, 0x0002	// START OF TEXT
	, 0x0003	// END OF TEXT
	, 0x0004	// END OF TRANSMISSION
	, 0x0005	// ENQUIRY
	, 0x0006	// ACKNOWLEDGE
	, 0x0007	// BELL
	, 0x0008	// BACKSPACE
	, 0x0009	// HORIZONTAL TABULATION
	, 0x000A	// LINE FEED
	, 0x000B	// VERTICAL TABULATION
	, 0x000C	// FORM FEED
	, 0x000D	// CARRIAGE RETURN
	, 0x000E	// SHIFT OUT
	, 0x000F	// SHIFT IN
	, 0x0010	// DATA LINK ESCAPE
	, 0x0011	// DEVICE CONTROL ONE
	, 0x0012	// DEVICE CONTROL TWO
	, 0x0013	// DEVICE CONTROL THREE
	, 0x0014	// DEVICE CONTROL FOUR
	, 0x0015	// NEGATIVE ACKNOWLEDGE
	, 0x0016	// SYNCHRONOUS IDLE
	, 0x0017	// END OF TRANSMISSION BLOCK
	, 0x0018	// CANCEL
	, 0x0019	// END OF MEDIUM
	, 0x001A	// SUBSTITUTE
	, 0x001B	// ESCAPE
	, 0x001C	// FILE SEPARATOR
	, 0x001D	// GROUP SEPARATOR
	, 0x001E	// RECORD SEPARATOR
	, 0x001F	// UNIT SEPARATOR
	, 0x0020	// SPACE

	, 0x3000	// IDEOGRAPHIC SPACE

	, L'\0'		// end of array
} ;

static const wchar_t stop_char_array[] =
{
	  0x0001	// START OF HEADING
	, 0x0002	// START OF TEXT
	, 0x0003	// END OF TEXT
	, 0x0004	// END OF TRANSMISSION
	, 0x0005	// ENQUIRY
	, 0x0006	// ACKNOWLEDGE
	, 0x0007	// BELL
	, 0x0008	// BACKSPACE
	, 0x0009	// HORIZONTAL TABULATION
	, 0x000A	// LINE FEED
	, 0x000B	// VERTICAL TABULATION
	, 0x000C	// FORM FEED
	, 0x000D	// CARRIAGE RETURN
	, 0x000E	// SHIFT OUT
	, 0x000F	// SHIFT IN
	, 0x0010	// DATA LINK ESCAPE
	, 0x0011	// DEVICE CONTROL ONE
	, 0x0012	// DEVICE CONTROL TWO
	, 0x0013	// DEVICE CONTROL THREE
	, 0x0014	// DEVICE CONTROL FOUR
	, 0x0015	// NEGATIVE ACKNOWLEDGE
	, 0x0016	// SYNCHRONOUS IDLE
	, 0x0017	// END OF TRANSMISSION BLOCK
	, 0x0018	// CANCEL
	, 0x0019	// END OF MEDIUM
	, 0x001A	// SUBSTITUTE
	, 0x001B	// ESCAPE
	, 0x001C	// FILE SEPARATOR
	, 0x001D	// GROUP SEPARATOR
	, 0x001E	// RECORD SEPARATOR
	, 0x001F	// UNIT SEPARATOR

	, 0x0021	// EXCLAMATION MARK
	, 0x002E	// FULL STOP
	, 0x003F	// QUESTION MARK

	, 133		// dot dot dot
	, 8230		// dot dot dot (unicode)

	, 0x3002	// IDEOGRAPHIC FULL STOP
	, 0xFF61	// HALFWIDTH IDEOGRAPHIC FULL STOP

	, 0xFF0E	// FULLWIDTH FULL STOP
	, 0xFF1F	// FULLWIDTH QUESTION MARK
	, 0xFF01	// FULLWIDTH EXCLAMATION MARK

	, L'\0'		// end of array
} ;

//static const wchar_t maru_char_array[] =
//{
//	0x0021	// EXCLAMATION MARK
//	, 0x002E	// FULL STOP
//	, 0x003F	// QUESTION MARK
//
//	, 0x3002	// IDEOGRAPHIC FULL STOP
//	, 0xFF61	// HALFWIDTH IDEOGRAPHIC FULL STOP
//
//	, 0xFF0E	// FULLWIDTH FULL STOP
//	, 0xFF1F	// FULLWIDTH QUESTION MARK
//	, 0xFF01	// FULLWIDTH EXCLAMATION MARK
//
//	, L'\0'		// end of array
//} ;


WordParser::WordParser( WordSelection selection ) :
	m_selection( selection ),
	m_eat_whitespace( true )
{
	app_state params ;
	params.read_from_registry() ;
	m_SegStopChars = CT2OLE( params.m_segChars ) ;
	
}

WordParser::~WordParser()
{

}


_bstr_t WordParser::segment_as_html( )
{
	m_formatter.init() ;
	
	m_original_range = m_selection.get_range() ;
	
	parse_segment( ) ;

	m_original_range.select() ;

	wstring html_text = m_formatter.get_text() ;

	return _bstr_t( html_text.c_str() ) ;
}


_bstr_t WordParser::segment_as_plaintext( )
{
	m_original_range = m_selection.get_range() ;
	
	parse_range_text( m_original_range ) ;

	m_original_range.select() ;
	
	wstring plain_text = m_formatter.get_text() ;
	
	return _bstr_t( plain_text.c_str() ) ;
}

bool WordParser::eat_whitespace()
{
	WordRange range = m_selection.get_range() ;

	_bstr_t whitespace_chars = (LPCWSTR)whitespace_char_array ;

	range.move_end_while( whitespace_chars ) ;

	long range_start = range.get_start() ;
	long range_end = range.get_end() ;

	if ( range_start == range_end )
	{
		return false ;
	}

	range.select_range( range_end, range_end ) ;

	return true ;
}

bool WordParser::select_next_sentence( )
{
	if ( m_eat_whitespace == true ) 
	{
		eat_whitespace() ;
	}

	m_eat_whitespace = true ;

	WordRange range = m_selection.get_range() ;

	long old_start = 0 ;
	old_start = range.get_start() ;
	ATLASSERT( old_start >= 0 ) ;

	_bstr_t stop_chars = stop_char_array ;
	range.move_end_until( stop_chars ) ;
	
	long old_end = 0 ;
	old_end = range.get_end() ;
	ATLASSERT( old_end >= 0 ) ;

	range.put_start( old_end ) ;
	range.put_end( old_end+1 ) ;

	long story_len = m_selection.get_story_length() ;

	if ( old_end >= story_len ) 
	{
		return false ;
	}

	_bstr_t text = range.get_text() ;

	if ( table_selected( text ) ) 
	{
		range.select_range( old_start, old_end ) ;
		return true ;
	}

	if ( text == _bstr_t( L"." ) )
	{
		old_end++ ;

		range.put_start( old_end ) ;
		range.put_end( old_end+1 ) ;
		
		_bstr_t text = range.get_text() ;
		
		if ( table_selected( text ) ) 
		{
			range.select_range( old_start, old_end ) ;
			return true ;
		}

		ATLASSERT( text.length() == 1 ) ;

		if ( is_right_quote_selected( text ) )
		{
			range.select_range( old_start, old_end + 1 ) ;
		}
		else if ( ! is_whitespace_char( text ) ) 
		{
			old_end++ ;
			
			range.select_range( old_end, old_end ) ;

			m_eat_whitespace = false ;
			select_next_sentence( ) ;

			WordRange selection_range = m_selection.get_range() ;
			selection_range.put_start( old_start ) ;
			selection_range.select() ;
		}
		else
		{
			range.select_range( old_start, old_end ) ;
		}
	}
	else
	{
		if ( ! is_whitespace_char( text )) 
		{
			old_end++ ;

			range.put_start( old_end ) ;
			range.put_end( old_end+1 ) ;
			_bstr_t text = range.get_text() ;
			if ( is_right_quote_selected( text ) ) 
			{
				old_end++ ;
			}
		}
		
		range.select_range( old_start, old_end ) ;
	}

	if ( m_selection.get_start() == m_selection.get_end() )
	{
		return false ;
	}
	
	return true ;
}

void WordParser::parse_segment( )
{
	font_properties props ;

	if ( ! formatting_enabled() ) 
	{
		parse_range( m_original_range, props ) ;
		return ;
	}
	
	WordFont font = m_original_range.get_font() ;
	
	bool needs_subdivision = false ;

	if ( m_parse_font_bold ) 
	{
		if( ! reflect_bold( font, props ) )
			needs_subdivision = true ;
	}

	if ( m_parse_font_italic ) 
	{
		if( ! reflect_italic( font, props ) )
			needs_subdivision = true ;
	}

	if ( m_parse_font_super_sub ) 
	{
		if( ! reflect_super_sub( font, props ) )
			needs_subdivision = true ;
	}

	if ( m_parse_font_underline ) 
	{
		if( ! reflect_underline( font, props ) )
			needs_subdivision = true ;
	}

	if ( m_parse_font_color ) 
	{
		if( ! reflect_color( font, props ) )
			needs_subdivision = true ;
	}

	if ( m_parse_font_face ) 
	{
		if( ! reflect_face( font, props ) )
			needs_subdivision = true ;
	}
	
	if ( needs_subdivision )
	{
		parse_words( m_original_range, props ) ;
		return ;
	}

	parse_range( m_original_range, props ) ;
	
}

void WordParser::parse_words(WordRange &segment, const font_properties &props )
{
	WordWords words = segment.get_words() ;
	
	long num_words = words.get_count() ;
	
	if ( num_words == 0 )
	{
		return ;
	}
	
	for ( long i = 1 ; i <= num_words ; ++i )
	{
		WordRange this_word = words.item( i ) ;
		parse_word( this_word, props ) ;
	}
	
}

void WordParser::parse_word(WordRange &word, const font_properties &props )
{
	font_properties word_props(props) ;

	WordFont font = word.get_font() ;
	
	bool needs_subdivision = false ;
	
	if ( m_parse_font_bold ) 
	{
		if( ! reflect_bold( font, word_props ) )
			needs_subdivision = true ;
	}
	
	if ( m_parse_font_italic ) 
	{
		if( ! reflect_italic( font, word_props ) )
			needs_subdivision = true ;
	}
	
	if ( m_parse_font_super_sub ) 
	{
		if( ! reflect_super_sub( font, word_props ) )
			needs_subdivision = true ;
	}
	
	if ( m_parse_font_underline ) 
	{
		if( ! reflect_underline( font, word_props ) )
			needs_subdivision = true ;
	}
	
	if ( m_parse_font_color ) 
	{
		if( ! reflect_color( font, word_props ) )
			needs_subdivision = true ;
	}

	
	if ( needs_subdivision )
	{
		parse_characters( word, word_props ) ;
		return ;
	}
	
	parse_range( word, word_props ) ;

}

void WordParser::parse_characters(WordRange &word, const font_properties &props )
{
	WordCharacters characters = word.get_characters() ;
	
	long num_chars = characters.get_count() ;
	
	if ( num_chars == 0 )
	{
		return ;
	}
	
	for ( long i = 1 ; i <= num_chars ; ++i )
	{
		WordRange this_character = characters.item( i ) ;
		
		parse_character( this_character, props ) ;
	}

}

void WordParser::parse_character(WordRange &character, const font_properties &props )
{
	font_properties char_props(props) ;

	WordFont font = character.get_font() ;
	
	if ( m_parse_font_bold ) 
	{
		reflect_bold( font, char_props );
	}
	
	if ( m_parse_font_italic ) 
	{
		reflect_italic( font, char_props ) ;
	}
	
	if ( m_parse_font_super_sub ) 
	{
		reflect_super_sub( font, char_props ) ;
	}
	
	if ( m_parse_font_underline ) 
	{
		reflect_underline( font, char_props ) ;
	}
	
	if ( m_parse_font_color ) 
	{
		reflect_color( font, char_props ) ;
	}

	parse_range( character, char_props ) ;

}

void WordParser::parse_range(WordRange &range, const font_properties &props )
{
	if ( m_parse_font_bold ) 
	{
		bool is_bold = props.get_bold() ;
		m_formatter.set_bold( is_bold ) ;
	}
	
	if ( m_parse_font_italic ) 
	{
		bool is_italic = props.get_italic() ;
		m_formatter.set_italic( is_italic ) ;
	}
	
	if ( m_parse_font_super_sub ) 
	{
		bool is_super = props.get_superscript() ;
		m_formatter.set_superscript( is_super ) ;
		
		bool is_sub = props.get_subscript() ;
		m_formatter.set_subscript( is_sub ) ;
	}
	
	if ( m_parse_font_underline ) 
	{
		bool is_underline = props.get_underline() ;
		m_formatter.set_underline( is_underline ) ;
	}
	
	if ( m_parse_font_color ) 
	{
		CColorRef color( props.get_font_color() ) ;
		if ( color.is_black() ) 
		{
			m_formatter.clear_fore_color() ;
		}
		else
		{
			m_formatter.set_fore_color( color ) ;
		}
	}
	if ( m_parse_font_face ) 
	{
		wstring font_face( props.get_font_face() );

		m_formatter.set_font_name( font_face ) ;
	}

	parse_range_text( range ) ;
	
}

bool WordParser::formatting_enabled()
{
	if ( m_parse_font_face ) return true ; 
	if ( m_parse_font_color ) return true ; 
	if ( m_parse_font_bold ) return true ; 
	if ( m_parse_font_italic ) return true ; 
	if ( m_parse_font_underline ) return true ; 
	if ( m_parse_font_super_sub ) return true ;
	
	return false ;
}

void WordParser::parse_range_text(WordRange &range)
{
	_bstr_t text = range.get_text() ;
	
	textstream_reader< wchar_t > reader ;
	reader.set_buffer( (LPCWSTR) text ) ;
	
	wstring chunk ;
	size_t char_pos = 0 ;
	
	while ( reader.empty() == false )
	{
		reader.getline( chunk, L'(', false ) ;
		wstring chunk_label( chunk ) ;
		m_formatter.add_text( chunk_label ) ;
		
		if ( reader.current_is( L'(' ) )
		{
			char_pos = reader.get_offset( char_pos ) ;
			WordCharacters characters = range.get_characters() ;
			WordRange this_character = characters.item( static_cast< long >( char_pos+1 ) ) ;
			wstring unicode_text( this_character.get_unicode_char() );
			m_formatter.add_text( unicode_text ) ;
			
			reader.advance() ;
		}
	}
}

bool WordParser::table_selected( const _bstr_t &text)
{
	long len = text.length() ;

	if ( len < 2 ) 
	{
		return false ;
	}

	return ((LPCWSTR)text)[len-1] == WORD_TABLE_MARKER ;
}

bool WordParser::is_whitespace_char(LPCWSTR text)
{
	if ( ! text ) 
	{
		return false ;
	}

	textstream_reader< wchar_t > whitespace_reader ;
	whitespace_reader.set_buffer( (LPCWSTR)whitespace_char_array ) ;
	
	wchar_t c = text[0] ;

	return whitespace_reader.jump_to_first_of( c ) ;
}

bool WordParser::reflect_bold(WordFont &font, font_properties &props)
{
	if ( props.is_bold_established() ) 
	{
		return true ;
	}

	MSOffice::MsoTriState tri_setting = font.is_bold() ;
	if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
	{
		bool is_bold = (tri_setting != MSOffice::msoFalse ) ;
		props.set_bold( is_bold ) ;
		return true ;
	}

	return false ;
}
bool WordParser::reflect_italic(WordFont &font, font_properties &props)
{
	if ( props.is_italic_established() ) 
	{
		return true ;
	}

	MSOffice::MsoTriState tri_setting = font.is_italic() ;
	if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
	{
		bool is_italic = (tri_setting != MSOffice::msoFalse ) ;
		props.set_italic( is_italic ) ;
		return true ;
	}
	
	return false ;
	
}
bool WordParser::reflect_super_sub(WordFont &font, font_properties &props)
{

	bool retval = true ;

	if ( ! props.is_superscript_established() ) 
	{
		MSOffice::MsoTriState tri_setting = font.is_superscript() ;
		if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
		{
			bool is_superscript = (tri_setting != MSOffice::msoFalse ) ;
			props.set_superscript( is_superscript ) ;
		}
		else
		{
			retval = false ;
		}
	}
	
	if ( ! props.is_subscript_established() ) 
	{
		MSOffice::MsoTriState tri_setting = font.is_subscript() ;
		if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
		{
			bool is_subscript = (tri_setting != MSOffice::msoFalse ) ;
			props.set_subscript( is_subscript ) ;
			return true ;
		}
		else
		{
			retval = false ;
		}
	}
	
	return retval ;
	
}
bool WordParser::reflect_underline(WordFont &font, font_properties &props)
{
	if ( props.is_underline_established() ) 
	{
		return true ;
	}

	int underline_setting = font.get_underline() ;
	if ( underline_setting != 9999999 )
	{
		bool is_underline = ( underline_setting != MSWord::wdUnderlineNone ) ;
		
		props.set_underline( is_underline ) ;

		return true ;
	}
	
	return false ;
	
}
bool WordParser::reflect_color(WordFont &font, font_properties &props)
{
	if ( props.is_font_color_established() ) 
	{
		return true ;
	}

	COLORREF color = font.get_rgb_color() ;
	if ( color != 9999999 )
	{
		CColorRef color_ref(color) ;
		props.set_font_color( color_ref ) ;	
		return true ;
	}
	
	return false ;

}
bool WordParser::reflect_face(WordFont &font, font_properties &props)
{
	if ( props.is_font_face_established() ) 
	{
		return true ;
	}

	wstring font_face( font.get_face() ) ;
	if ( ! font_face.empty() )
	{
		props.set_font_face( font_face ) ;
		return true ;
	}
	
	return false ;
}

bool WordParser::is_right_quote_selected(const _bstr_t &text)
{
	LPCWSTR raw_text = (LPCWSTR)text ;
	if ( ! raw_text ) 
	{
		return false ;
	}

	if ( raw_text[0] == L'\"' ) 
	{
		return true ;
	}
	if ( raw_text[0] == RIGHT_SINGLE_QUOTATION_MARK ) 
	{
		return true ;
	}
	if ( raw_text[0] == RIGHT_DOUBLE_QUOTATION_MARK ) 
	{
		return true ;
	}

	return false ;
}

bool WordParser::select_to_maru()
{
	eat_whitespace() ;

	WordRange range = m_selection.get_range() ;

	long old_start = range.get_start() ;

	_bstr_t stop_chars = m_SegStopChars ;
	range.move_end_until( stop_chars ) ;
	
	long old_end = range.get_end() ;
	old_end++ ;

	range.select_range( old_start, old_end ) ;

	if ( m_selection.get_start() == m_selection.get_end() )
	{
		return false ;
	}
	
	return true ;

}
