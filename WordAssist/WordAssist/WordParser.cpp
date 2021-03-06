// WordParser.cpp: implementation of the WordParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordParser.h"
#include "StringConversions.h"
#include "textstream_reader.h"
#include "logging.h"
#include "logical.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace except ;

static const int WORD_TABLE_MARKER = 7 ;

static const wchar_t LEFT_SINGLE_QUOTATION_MARK = 0x2018 ;
static const wchar_t RIGHT_SINGLE_QUOTATION_MARK = 0x2019 ;
static const wchar_t LEFT_DOUBLE_QUOTATION_MARK = 0x201C ;	
static const wchar_t RIGHT_DOUBLE_QUOTATION_MARK = 0x201D ;

// Non-breaking space = 0x00A0
// Non-breaking hyphen

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
//	, 0x001E	// RECORD SEPARATOR -- Used as non-breaking hyphen in Word (BLEAH!!)
	, 0x001F	// UNIT SEPARATOR

	, L'\0'		// end of array
} ;

CParserBase::CParserBase() :
	m_parse_font_face( TRUE ),
		m_parse_font_color( TRUE ),
		m_parse_font_bold( TRUE ),
		m_parse_font_italic( TRUE ),
		m_parse_font_underline( TRUE ),
		m_parse_font_super_sub( TRUE )
{

}

CParserBase::~CParserBase()
{

}

void CParserBase::set_parse_font_face( BOOL setting )
{
	m_parse_font_face = setting ; 
}
void CParserBase::set_parse_font_color( BOOL setting )
{
	m_parse_font_color = setting ; 
}
void CParserBase::set_parse_font_bold( BOOL setting )
{
	m_parse_font_bold = setting ; 
}
void CParserBase::set_parse_font_italic( BOOL setting )
{
	m_parse_font_italic = setting ; 
}
void CParserBase::set_parse_font_underline( BOOL setting )
{
	m_parse_font_underline = setting ; 
}
void CParserBase::set_parse_font_super_sub( BOOL setting )
{
	m_parse_font_super_sub = setting ;
}


/*
* Constructor.
* 
* \param WordSelection selection
* The word selection we will be parsing.
*/
WordParser::WordParser( WordSelection selection, app_state *params, Abbreviations *abbreviations  ) :
	m_selection( selection ),
	m_eat_whitespace( true ),
	m_skipNumbers(false),
	m_skipJ(NO_SKIP),
	m_select_spaces(true),
	m_ok_endings(abbreviations)
{
	setPrefs(params) ;
}

#ifdef _DEBUG
WordParser::WordParser( ) :
	m_eat_whitespace( true ),
	m_skipNumbers(false),
	m_skipJ(NO_SKIP),
	m_select_spaces(true)
{
}
#endif

WordParser::~WordParser()
{
	try
	{
		word_sel_ptr dummysel ;
		this->m_selection = dummysel ;
		word_range_ptr dummyrange ;
		this->m_original_range = dummyrange ;
	}
	catch (CException& e)
	{
		logging::log_error("Error on destruction of WordParser") ;
		logging::log_exception(e) ;
	}
	catch (...) 
	{
		logging::log_error("Error on destruction of WordParser (unknown exception)") ;
	}
}
/*
* Returns the currently selected Word range as an HTML string
* 
* If we trigger an exception, we fall back to plain text.
*
* \returns
* _bstr_t - The range as HTML
*/
_bstr_t WordParser::segment_as_html( )
{
	try
	{
		m_formatter.init() ;

		m_original_range = m_selection.get_range() ;

		parse_segment( ) ;

		m_original_range.select() ;

		wstring html_text = m_formatter.get_text() ;
		return string2BSTR(html_text) ;
	}
	catch (CSWException& e)
	{
		logging::log_error("Failed to get selection as HTML (structured windows exception)") ;
		logging::log_exception(e) ;
		return this->segment_as_plaintext() ;
	}

}


/*
* Returns the current selection as plain text.
* We still can't just take the text value from the selection, because
* we have to check for open parens (char code 40). That's what Word
* will tell us we have if it's a symbol.
*/
_bstr_t WordParser::segment_as_plaintext( )
{
	m_original_range = m_selection.get_range() ;
	try
	{
		m_formatter.init() ;
		parse_range_text( m_original_range ) ;

		m_original_range.select() ;

		wstring plain_text = m_formatter.get_text() ;
		boost::replace_all(plain_text, L"&", L"&amp;") ;
		boost::replace_all(plain_text, L">", L"&gt;") ;
		boost::replace_all(plain_text, L"<", L"&lt;") ;
		return string2BSTR(plain_text) ;
	}
	catch (_com_error& e)
	{
		logging::log_error("Failed to retrieve plain text (_com_error)") ;
		logging::log_exception(e) ;
		return m_original_range.get_text() ;
	}
}

/*
* Moves the selection to the right until the current selection is 
* not a whitespace character, or we reach the end of the story.
*/
bool WordParser::eat_whitespace()
{
	try
	{
		WordRange range = m_selection.get_range() ;

		_bstr_t whitespace_chars = (LPCWSTR)whitespace_char_array ;

		range.move_end_while( whitespace_chars ) ;

		long range_start = range.get_start() ;
		long range_end = range.get_end() ;

		// Are we at the end of the story?
		if ( range_start == range_end )
		{
			return false ;
		}

		range.select_range( range_end, range_end ) ;
	}
	catch (...)
	{
		logging::log_error("Failed to eat whitespace") ;
		throw ;
	}

	return true ;
}

/*
* Select the next sentence.
* 
* \returns
* bool -- whether a sentence was successfully selected.
* 
* Will return false if we reach the end of the story.
*/
bool WordParser::select_next_sentence( )
{
	// This little bit is in case we get called recursively ===>
	// It will eat any intervening whitespace, and make sure that
	// we don't clear the variable.
	if ( m_eat_whitespace ) 
	{
		eat_whitespace() ;
	}

	m_eat_whitespace = true ;
	// <=====

	try
	{
		WordRange range = m_selection.get_range() ;

		long old_start =  range.get_start() ;
		ATLASSERT( old_start >= 0 ) ;

		const _bstr_t stop_chars = m_SegStopChars + stop_char_array ;
		range.move_end_until( stop_chars ) ;

		// By this point, we're either at a stop character or the end
		// of the story.

		long old_end = range.get_end() ;
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

		// handle periods differently:
		// - make sure it's not the middle of a number
		// - select trailing space (depends on user setting)
		// TODO: handle "OK" endings like Mr Ms and Mrs
		if ( text == _bstr_t( L"." ) )
		{
			return handle_period(range, old_end, old_start, text);
		}
		// double-byte period -- might be in a number
		else if ( text == _bstr_t( L"．" ) )
		{
			return handle_db_period(range, old_start, old_end);
		}
		else
		{
			if ( ! is_whitespace_char( text )) 
			{
				handle_not_whitespace(range, old_end, text);
			}

			range.select_range( old_start, old_end ) ;
		}

		// Are we at the end of the story?
		if ( m_selection.get_start() == m_selection.get_end() )
		{
			return false ;
		}

		return true ;
	}
	catch (...)
	{
		logging::log_error("Failed in WordParser::select_next_sentence") ;
		throw ;
	}
}

/*
*/
void WordParser::parse_segment( )
{
	font_properties props ;

	if ( ! formatting_enabled() ) 
	{
		parse_range( m_original_range, props ) ;
		return ;
	}

	bool needs_subdivision = false ;

	try
	{
		WordFont font = m_original_range.get_font() ;

		if ( m_parse_font_bold ) 
		{
			if( ! reflect_bold( font, props ) )
				needs_subdivision = true ;
		}
		if ( ! needs_subdivision && m_parse_font_italic ) 
		{
			if( ! reflect_italic( font, props ) )
				needs_subdivision = true ;
		}

		if ( ! needs_subdivision && m_parse_font_super_sub ) 
		{
			if( ! reflect_super_sub( font, props ) )
				needs_subdivision = true ;
		}

		if ( ! needs_subdivision && m_parse_font_underline ) 
		{
			if( ! reflect_underline( font, props ) )
				needs_subdivision = true ;
		}

		if ( ! needs_subdivision && m_parse_font_color ) 
		{
			if( ! reflect_color( font, props ) )
				needs_subdivision = true ;
		}

		if ( ! needs_subdivision && m_parse_font_face ) 
		{
			if( ! reflect_face( font, props ) )
				needs_subdivision = true ;
		}
	}
	catch (CComException& e)
	{
		logging::log_error("WordParser::parse_segment -- CComException") ;
		logging::log_error("Failed to get selection formatting") ;
		logging::log_exception(e) ;
		logging::log_warn("Will parse segment without formatting information") ;
		parse_range( m_original_range, props ) ;
		return ;
	}
	catch (_com_error& e)
	{
		logging::log_error("WordParser::parse_segment -- _com_error") ;
		logging::log_error("Failed to get selection formatting") ;
		logging::log_exception(e) ;
		logging::log_warn("Will parse segment without formatting information") ;
		parse_range( m_original_range, props ) ;
		return ;
	}

	if ( needs_subdivision )
	{
		try
		{
			parse_words( m_original_range, props ) ;
			return ;
		}
		catch (CException& e)
		{
			logging::log_error("Program Exception: Failed to parse words -- falling back to entire segment.") ;
			logging::log_exception(e) ;
			logging::log_warn("Will parse segment without formatting information") ;
		}
		catch (_com_error& e)
		{
			logging::log_error("COM Exception: Failed to parse words -- falling back to entire segment.") ;
			logging::log_exception(e) ;
			logging::log_warn("Will parse segment without formatting information") ;
		}
	}

	parse_range( m_original_range, props ) ;

}

/*
*/
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

/*
*/
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

	if ( ! needs_subdivision && m_parse_font_italic ) 
	{
		if( ! reflect_italic( font, word_props ) )
			needs_subdivision = true ;
	}

	if ( ! needs_subdivision && m_parse_font_super_sub ) 
	{
		if( ! reflect_super_sub( font, word_props ) )
			needs_subdivision = true ;
	}

	if ( ! needs_subdivision && m_parse_font_underline ) 
	{
		if( ! reflect_underline( font, word_props ) )
			needs_subdivision = true ;
	}

	if ( ! needs_subdivision && m_parse_font_color ) 
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

/*
*/
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

/*!
* \brief
*/
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

/*
*/
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

/*
*/
bool WordParser::formatting_enabled()
{
	return logical::any(!! m_parse_font_face,
		!! m_parse_font_color,
		!! m_parse_font_bold,
		!! m_parse_font_italic,
		!! m_parse_font_underline,
		!! m_parse_font_super_sub );
}

/*
*/
void WordParser::parse_range_text(WordRange &range)
{
	try
	{
		wstring text = BSTR2wstring(range.get_text()) ;

		textstream_reader< wchar_t > reader ;
		reader.set_buffer( text.c_str() ) ;

		wstring chunk ;
		size_t char_pos = 0 ;

		while ( reader.empty() == false )
		{
			m_formatter.add_text( reader.getline(L'(', false ) ) ;

			if ( reader.current_is( L'(' ) )
			{
				try
				{
					char_pos = reader.get_offset( char_pos ) ;
					WordCharacters characters = range.get_characters() ;
					WordRange this_character = characters.item( static_cast< long >( char_pos+1 ) ) ;
					wstring unicode_text( this_character.get_unicode_char() );
					m_formatter.add_text( unicode_text ) ;
				}
				catch (CException& e)
				{
					logging::log_error("Failed to get Unicode character at '(' position") ;
					logging::log_exception(e) ;
					m_formatter.add_text( L"(" ) ;
				}
				catch (_com_error& e)
				{
					logging::log_error("Failed to get Unicode character at '(' position") ;
					logging::log_exception(e) ;
					m_formatter.add_text( L"(" ) ;
				}

				reader.advance() ;
			}
		}
	}
	catch (...)
	{
		logging::log_error("Failure in WordParser::parse_range_text") ;
		throw ;
	}

}

/*
*/
bool WordParser::table_selected( const _bstr_t &text)
{
	long len = text.length() ;

	if ( len < 2 ) 
	{
		return false ;
	}

	return ((LPCWSTR)text)[len-1] == WORD_TABLE_MARKER ;
}

/*
* Returns whether the first character in the string is a whitespace character
* 
* If the string is NULL, returns false
*/
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

/*
*/
bool WordParser::reflect_bold(WordFont &font, font_properties &props)
{
	if ( props.is_bold_established() ) 
	{
		return true ;
	}

	return make_bold_setting(font.is_bold(), props);
}

bool WordParser::make_bold_setting( MSOffice::MsoTriState tri_setting, font_properties &props )
{
	if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
	{
		bool is_bold = (tri_setting != MSOffice::msoFalse ) ;
		props.set_bold( is_bold ) ;
		return true ;
	}

	return false ;
}
/*
*/
bool WordParser::reflect_italic(WordFont &font, font_properties &props)
{
	if ( props.is_italic_established() ) 
	{
		return true ;
	}

	return make_italic_setting(font.is_italic(), props);
}


bool WordParser::make_italic_setting( MSOffice::MsoTriState tri_setting, font_properties &props )
{
	if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
	{
		bool is_italic = (tri_setting != MSOffice::msoFalse ) ;
		props.set_italic( is_italic ) ;
		return true ;
	}

	return false ;
}
/*
* Set the superscript/subscript property for the current range's font
*/
bool WordParser::reflect_super_sub(WordFont &font, font_properties &props)
{
	bool retval = true ;

	if ( ! props.is_superscript_established() ) 
	{
		if (! make_superscript_setting(font.is_superscript(), props))
		{
			retval = false ;
		}
	}

	if ( ! props.is_subscript_established() ) 
	{
		if (! make_subscript_setting(font.is_subscript(), props))
		{
			retval = false ;
		}
	}

	return retval ;
}

bool WordParser::make_superscript_setting( MSOffice::MsoTriState tri_setting, font_properties &props )
{
	if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
	{
		props.set_superscript( tri_setting != MSOffice::msoFalse ) ;
		return true ;
	}
	return false ;
}
bool WordParser::make_subscript_setting( MSOffice::MsoTriState tri_setting, font_properties &props )
{
	if ( tri_setting == MSOffice::msoTrue || tri_setting == MSOffice::msoFalse )
	{
		props.set_subscript( tri_setting != MSOffice::msoFalse ) ;
		return true ;
	}
	return false ;
}

/*
* 
*/
bool WordParser::reflect_underline(WordFont &font, font_properties &props)
{
	if ( props.is_underline_established() ) 
	{
		return true ;
	}

	int underline_setting = font.get_underline() ;
	if ( underline_setting != MSWord::wdUndefined )
	{
		bool is_underline = ( underline_setting != MSWord::wdUnderlineNone ) ;

		props.set_underline( is_underline ) ;

		return true ;
	}

	return false ;

}
/*
*/
bool WordParser::reflect_color(WordFont &font, font_properties &props)
{
	if ( props.is_font_color_established() ) 
	{
		return true ;
	}

	COLORREF color = font.get_rgb_color() ;
	if ( color != MSWord::wdUndefined )
	{
		CColorRef color_ref(color) ;
		props.set_font_color( color_ref ) ;	
		return true ;
	}

	return false ;

}
/*
*/
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

/*
*/
bool WordParser::is_right_quote_selected(const _bstr_t &text)
{
	LPCWSTR raw_text = (LPCWSTR)text ;
	if ( ! raw_text ) 
	{
		return false ;
	}

	if ( raw_text[0] == L'"' ) 
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
	// handle case where period is inside end parens:
	// `... some text.)`
	else if (raw_text[0] == L')')
	{
		return true ;
	}
	return false ;
}

/*
* Select to the next stop char (ignoring whitespace etc.)
*/
bool WordParser::select_to_maru()
{
	eat_whitespace() ;

	WordRange range = m_selection.get_range() ;

	long old_start = range.get_start() ;

	range.move_end_until( m_SegStopChars ) ;

	long old_end = range.get_end() ;
	old_end++ ;

	range.select_range( old_start, old_end ) ;

	return ( m_selection.get_start() < m_selection.get_end() ) ;

}

/*
* Sets the app preferences we are interested in.
*/
void WordParser::setPrefs( app_state *prefs )
{
	m_SegStopChars = CT2OLE( prefs->m_data.m_segChars ) ;
	m_skipNumbers = !! prefs->m_data.m_skipNumbers ;
	m_skipJ = prefs->m_data.m_skipJ ;
	m_select_spaces = !! prefs->m_data.m_select_spaces ;

	this->set_parse_font_face( prefs->get_font_face() ) ;
	this->set_parse_font_color( prefs->get_font_color() ) ;
	this->set_parse_font_bold( prefs->get_font_bold() ) ;
	this->set_parse_font_italic( prefs->get_font_italic() ) ;
	this->set_parse_font_underline( prefs->get_font_underline() ) ;
	this->set_parse_font_super_sub( prefs->get_font_super_sub() ) ;

}



bool WordParser::handle_period(WordRange &range, long &old_end, long old_start, _bstr_t &text)
{
	try
	{
		old_end++ ;
		range.select_range(max(0, old_end-5), old_end) ;
		const wstring text_so_far = BSTR2wstring(range.get_text()) ;
		FOREACH(wstring word, m_ok_endings->m_abbreviations)
		{
			if (boost::ends_with(text_so_far, word))
			{
				old_end++ ;

				range.select_range( old_end, old_end ) ;

				m_eat_whitespace = false ;
				select_next_sentence( ) ;

				WordRange selection_range = m_selection.get_range() ;
				selection_range.put_start( old_start ) ;
				selection_range.select() ;

				return ( m_selection.get_start() < m_selection.get_end() ) ;
			}
		}

		_bstr_t next_char = range.next_char() ;

		if ( table_selected( next_char ) ) 
		{
			range.select_range( old_start, old_end ) ;
			return true ;
		}

		ATLASSERT( next_char.length() == 1 ) ;
		// handle case where period is at end of quotes:
		// `... some text."`
		if ( is_right_quote_selected( next_char ) ) 
		{
			advance_end(range, old_end, text, next_char);
		}
		else if (! is_whitespace_char( next_char )) 
		{
			old_end++ ;

			range.select_range( old_end, old_end ) ;

			m_eat_whitespace = false ;
			select_next_sentence( ) ;

			WordRange selection_range = m_selection.get_range() ;
			selection_range.put_start( old_start ) ;
			selection_range.select() ;

			return ( m_selection.get_start() < m_selection.get_end() ) ;
		}
		if (m_select_spaces && next_char == _bstr_t(L" "))
		{
			range.select_range( old_start, old_end+1 ) ;
		}
		else
		{
			range.select_range( old_start, old_end ) ;
		}
		return ( m_selection.get_start() < m_selection.get_end() ) ;
	}
	catch (...)
	{
		logging::log_error("Error in WordParser::handle_period") ;
		throw;
	}
}


long WordParser::handle_wdigit( WordRange &range, long &old_end, long old_start )
{
	try
	{
		old_end++ ;

		range.select_range( old_end, old_end ) ;

		m_eat_whitespace = false ;
		select_next_sentence( ) ;

		WordRange selection_range = m_selection.get_range() ;
		selection_range.put_start( old_start ) ;
		selection_range.select() ;
		return old_end;
	}
	catch (...)
	{
		logging::log_error("Error in WordParser::handle_wdigit") ;
		throw;
	}
}

void WordParser::handle_not_whitespace( WordRange &range, long &old_end, _bstr_t &text )
{
	try
	{
		old_end++ ;
		range.put_end(old_end) ;

		_bstr_t next_char = range.next_char() ;
		if ( is_right_quote_selected( next_char ) ) 
		{
			advance_end(range, old_end, text, next_char);
		}

		if (m_select_spaces && 
			(text == _bstr_t(L"!") || 
			text == _bstr_t(L"?") ||
			is_right_quote_selected(text)))
		{
			if (next_char == _bstr_t(L" "))
			{
				old_end++ ;
			}
		}
	}
	catch (...)
	{
		logging::log_error("Error in WordParser::handle_not_whitespace") ;
		throw;
	}
}

bool WordParser::handle_db_period( WordRange &range, long old_start, long &old_end )
{
	try
	{
		_bstr_t next_char = range.next_char() ;
		if ( table_selected( next_char ) ) 
		{
			range.select_range( old_start, old_end ) ;
			return true ;
		}

		wstring next_char_w = BSTR2wstring(next_char);
		if (! next_char_w.empty() && str::is_fullwidth_number(next_char_w[0]))
		{
			old_end = handle_wdigit(range, old_end, old_start);

			return ( m_selection.get_start() < m_selection.get_end() ) ;
		}

		range.select_range( old_start, old_end+1 ) ;
		return ( m_selection.get_start() < m_selection.get_end() ) ;
	}
	catch (...)
	{
		logging::log_error("Error in WordParser::handle_db_period") ;
		throw;
	}
}

void WordParser::advance_end( WordRange &range, long &old_end, _bstr_t &text, _bstr_t &next_char )
{
	old_end++ ;
	text = next_char ;
	next_char = range.next_char() ;
}
