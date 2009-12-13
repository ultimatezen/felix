/*!
		@brief Export data to various other formats (TRADOS) 
		@author Ryan Ginstrom
*/

#include "StdAfx.h"
#include "memory_local.h"
#include "data_exporter.h"
#include "charset_info.h"	// charset_info
#include "system_message.h"
#include <boost/algorithm/string.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// Function name	: TradosDataExporter::TradosDataExporter
// Description	    : 
// Return type		: 
// Argument         : std::set< wstring > &fonts
TradosDataExporter::TradosDataExporter( std::set< wstring > &fonts, CProgressListener *listener  )
	: m_p_depth( 0 ), 
	m_fonts( false ), // no default entry
	m_listener(listener),
	TRADOS_PREAMBLE
	(    
		"<RTF Preamble>\r\n"
		"<FontTable>\r\n"
	),

	TRADOS_STYLE_SHEET
	(
		"<StyleSheet>\r\n"
		"{\\stylesheet \r\n"
		"{\\St \\s0 {\\StN Normal}}\r\n"
		"{\\St \\cs1 {\\StB \\v\\f1\\fs24\\sub\\cf12 }{\\StN tw4winMark}}\r\n"
		"{\\St \\cs2 {\\StB \\cf4\\fs40\\f1 }{\\StN tw4winError}}\r\n"
		"{\\St \\cs3 {\\StB \\f1\\cf11\\lang1024 }{\\StN tw4winPopup}}\r\n"
		"{\\St \\cs4 {\\StB \\f1\\cf10\\lang1024 }{\\StN tw4winJump}}\r\n"
		"{\\St \\cs5 {\\StB \\f1\\cf15\\lang1024 }{\\StN tw4winExternal}}\r\n"
		"{\\St \\cs6 {\\StB \\f1\\cf6\\lang1024 }{\\StN tw4winInternal}}\r\n"
		"{\\St \\cs7 {\\StB \\cf2 }{\\StN tw4winTerm}}\r\n"
		"{\\St \\cs8 {\\StB \\f1\\cf13\\lang1024 }{\\StN DO_NOT_TRANSLATE}}\r\n"
		"{\\St \\cs9 \\additive {\\StN Default Paragraph Font}}}\r\n"
		"</RTF Preamble>"
	),
	m_old_codepage( CP_ACP ),
	m_memory(new memory_engine::memory_local())
{

	// our default...

	m_fonts.add_font( _T("MS UI Gothic") ) ;

	textstream_reader< char > font_table_buffer( 
	"{\\f1 \\fmodern\\fprq1 \\fcharset0 Courier New;}"
	"{\\f2 \\fswiss\\fprq2 \\fcharset0 Arial;}"
	"{\\f3 \\fcharset128 MS UI Gothic;}"
	"{\\f3 \\froman\\fprq2 {\\*\\panose 02020603050405020304}\\fcharset0 Times New Roman;}"
	"{\\f4 \\froman\\fprq1 {\\*\\panose 02020609040205080304}{\\*\\falt MS Mincho}\\fcharset128 \\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9;}"
	"{\\f5 \\froman\\fprq2 {\\*\\panose 02040604050505020304}\\fcharset0 Century;}"
	"{\\f6 \\froman\\fprq1 {\\*\\panose 02020609040205080304}\\fcharset128 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9;}"
	"{\\f7 \\froman\\fprq2 \\fcharset238 Times New Roman CE;}"
	"{\\f8 \\froman\\fprq2 \\fcharset204 Times New Roman Cyr;}"
	"{\\f9 \\froman\\fprq2 \\fcharset161 Times New Roman Greek;}"
	"{\\f10 \\froman\\fprq2 \\fcharset162 Times New Roman Tur;}"
	"{\\f11 \\froman\\fprq2 \\fcharset177 Times New Roman (Hebrew);}"
	"{\\f12 \\froman\\fprq2 \\fcharset178 Times New Roman (Arabic);}"
	"{\\f13 \\froman\\fprq2 \\fcharset186 Times New Roman Baltic;}"
	"{\\f14 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset0 MS Mincho Western;}"
	"{\\f15 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset238 MS Mincho CE;}"
	"{\\f16 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset204 MS Mincho Cyr;}"
	"{\\f17 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset161 MS Mincho Greek;}"
	"{\\f18 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset162 MS Mincho Tur;}"
	"{\\f19 \\froman\\fprq1 {\\*\\falt MS Mincho}\\fcharset186 MS Mincho Baltic;}"
	"{\\f20 \\froman\\fprq1 \\fcharset0 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Western;}"
	"{\\f21 \\froman\\fprq1 \\fcharset238 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 CE;}"
	"{\\f22 \\froman\\fprq1 \\fcharset204 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Cyr;}"
	"{\\f23 \\froman\\fprq1 \\fcharset161 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Greek;}"
	"{\\f24 \\froman\\fprq1 \\fcharset162 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Tur;}"
	"{\\f25 \\froman\\fprq1 \\fcharset186 @\\'82\\'6c\\'82\\'72 \\'96\\'be\\'92\\'a9 Baltic;}"
	"{\\f26 \\fmodern\\fprq2 {\\*\\panose 020b0600070205080204}\\fcharset128 @MS UI Gothic;}"
	"{\\f27 \\fswiss\\fprq2 \\fcharset238 Arial CE;}"
	"{\\f28 \\fswiss\\fprq2 \\fcharset204 Arial Cyr;}"
	"{\\f29 \\fswiss\\fprq2 \\fcharset161 Arial Greek;}"
	"{\\f30 \\fswiss\\fprq2 \\fcharset162 Arial Tur;}"
	"{\\f31 \\fswiss\\fprq2 \\fcharset177 Arial (Hebrew);}"
	"{\\f32 \\fswiss\\fprq2 \\fcharset178 Arial (Arabic);}"
	"{\\f33 \\fswiss\\fprq2 \\fcharset186 Arial Baltic;}"
	"{\\f34 \\fmodern\\fprq2 \\fcharset0 MS UI Gothic Western;}"
	"{\\f35 \\fmodern\\fprq2 \\fcharset238 MS UI Gothic CE;}"
	"{\\f36 \\fmodern\\fprq2 \\fcharset204 MS UI Gothic Cyr;}"
	"{\\f37 \\fmodern\\fprq2 \\fcharset161 MS UI Gothic Greek;}"
	"{\\f38 \\fmodern\\fprq2 \\fcharset162 MS UI Gothic Tur;}"
	"{\\f39 \\fmodern\\fprq2 \\fcharset186 MS UI Gothic Baltic;}"
	"{\\f40 \\fmodern\\fprq2 \\fcharset0 @MS UI Gothic Western;}"
	"{\\f41 \\fmodern\\fprq2 \\fcharset238 @MS UI Gothic CE;}"
	"{\\f42 \\fmodern\\fprq2 \\fcharset204 @MS UI Gothic Cyr;}"
	"{\\f43 \\fmodern\\fprq2 \\fcharset161 @MS UI Gothic Greek;}"
	"{\\f44 \\fmodern\\fprq2 \\fcharset162 @MS UI Gothic Tur;}"
	"{\\f45 \\fmodern\\fprq2 \\fcharset186 @MS UI Gothic Baltic;}}  " 
	) ;

	font_table_buffer.eat_whitespace() ;
	while ( font_table_buffer.current_is( '{' ) )
	{
		// eat the {
		ATLASSERT( font_table_buffer.current_is( '{' ) ) ;
		font_table_buffer.advance() ;

		rtf::my_font font ;				// the font

		// we know that the first tag is font num
		// just jump past it
		font_table_buffer.find( " ", true ) ;

		while ( ! font_table_buffer.current_is( '}' ) )
		{
			font_table_buffer.jump_to_first_of( char('\\'), true ) ;
			string tag ;
			ATLVERIFY(font_table_buffer.getline(tag, " \\{", false )) ;

			// check for font family
			if ( tag == "fdecorative" )
				font.set_family( FF_DECORATIVE ) ;
			else if ( tag == "fmodern" )
				font.set_family( FF_MODERN ) ;
			else if ( tag == "froman" )
				font.set_family( FF_ROMAN ) ;
			else if ( tag == "fscript" )
				font.set_family( FF_SCRIPT ) ;
			else if ( tag == "fswiss" )
				font.set_family( FF_SWISS ) ;
			// check for pitch 
			else if ( tag == "fprq1" )
				font.set_pitch( FIXED_PITCH ) ;
			else if ( tag == "fprq2" )
				font.set_pitch( VARIABLE_PITCH ) ;
			else if ( str::left(tag, 8 ) == "fcharset" )
			{
				font.set_charset( static_cast< unsigned char >(string2long( tag.substr(8) ) ) ) ;
				font_table_buffer.advance() ;
				tag = font_table_buffer.getline(char(';'), true ) ;
				font.set_name( CA2CT( unescape_string( tag ).c_str() ) ) ;

			}


			if( font_table_buffer.current_is( '{' ) )
			{
				font_table_buffer.jump_to_first_of( char('}'), true ) ; 
			}
		}

		font.lookup_font() ;

		m_fonts.add_font( font ) ;


		// clear the font info
		font.init_font_info() ;

		// eat the }
		ATLASSERT( font_table_buffer.current_is( '}' ) ) ;

		font_table_buffer.advance() ; 

		// advance to next brace

		font_table_buffer.eat_whitespace() ;
	}

	foreach(const wstring font, fonts)
	{
		m_fonts.add_font(string2tstring(font)) ;
	}

	m_colors[L"black"		] = "0" ;
	m_colors[L"#000000"	] = "1" ;
	m_colors[L"blue"		] = "2" ;
	m_colors[L"#0000ff"	] = "2" ;
	m_colors[L"cyan"		] = "3" ;
	m_colors[L"#00ffff"	] = "3" ;
	m_colors[L"green"		] = "4" ;
	m_colors[L"#008000"	] = "4" ;
	m_colors[L"magenta"	] = "5" ;
	m_colors[L"#ff00ff"	] = "5" ;
	m_colors[L"red"		] = "6" ;
	m_colors[L"#ff0000"	] = "6" ;
	m_colors[L"yellow"	] = "7" ;
	m_colors[L"#ffff00"	] = "7" ;
	m_colors[L"white"		] = "8" ;
	m_colors[L"#ffffff"	] = "8" ;
	m_colors[L"dk_blue"	] = "9" ;
	m_colors[L"#00008b"	] = "9" ;
	m_colors[L"dk_cyan"	] = "10";
	m_colors[L"#008b8b"	] = "10" ;
	m_colors[L"dk_green"	] = "11";
	m_colors[L"#006400"		] = "11" ;
	m_colors[L"dk_magenta"	] = "12";
	m_colors[L"#8b008b"		] = "12" ;
	m_colors[L"dk_red"		] = "13";
	m_colors[L"#8b0000"		] = "13" ;
	m_colors[L"dk_yellow"	] = "14";
	m_colors[L"dk_gray"		] = "15";
	m_colors[L"#a9a9a9"		] = "15" ;
	m_colors[L"lt_gray"		] = "16";
	m_colors[L"#d3d3d3"		] = "16" ;
}


void TradosDataExporter::export_trados( memory_pointer mem )
{
	m_memory = mem ;

	m_listener->OnProgressInit( m_memory->get_location(), 0, m_memory->size() ) ;

	if ( ! write_preamble() )
	{
		THROW_WIN_EXCEPTION( R2T( IDS_MSG_EXPORT_FAILED ) ) ;
	}

	int num_missed = 0 ;
	size_t count = 0 ;

	foreach(record_pointer rec, mem->get_records())
	{
		try
		{
			write_record( rec ) ;

			count++ ;

			m_listener->OnProgressWriteUpdate( count ) ;

		}
		catch ( CException &e )
		{
			if ( IDNO == e.notify_user( IDS_PROMPT_WRITE_FAILED, MB_YESNO ) )
			{
				m_listener->OnProgressDoneWrite(0) ;
				throw CException(e) ;
			}
			num_missed++ ;
		}
	}
	ATLASSERT( count == mem->size() - num_missed ) ;

	m_listener->OnProgressDoneWrite( count ) ;
}



// Function name	: TradosDataExporter::~TradosDataExporter
// Description	    : 
// Return type		: 
TradosDataExporter::~TradosDataExporter()
{
	if (m_file.is_open())
	{
		m_file.write_eof() ;
		m_file.close() ;
	}

}

void TradosDataExporter::set_source( tstring source ) 
{ 
	m_source_lang = string2string( source ) ;
	m_source_tag = "<Seg L=" + m_source_lang + ">" ; 
}

void TradosDataExporter::set_target( tstring target ) 
{ 

	m_target_lang = string2string( target ) ;
	m_target_tag = "<Seg L=" + m_target_lang + ">" ; 
}

bool TradosDataExporter::open_destination( const CString &destination )
{
	m_file.close() ;
	return m_file.open( destination ) ;
}

bool TradosDataExporter::write_preamble()
{

	if ( m_file.is_open() == false ) 
	{
		THROW_WIN_EXCEPTION( system_message( IDS_FILE_NOT_OPEN_FOR_WRITING, m_file.file_name() ) )  ;
	}

	write_string( TRADOS_PREAMBLE ) ;
	write_string( m_fonts.get_table( ) ) ;
	write_string( TRADOS_STYLE_SHEET ) ;

	return true ;
}



// Function name	: TradosDataExporter::write_record
// Description	    : 
// Return type		: bool 
// Argument         : record_pointer record

bool TradosDataExporter::write_record( record_pointer record )
{
	if ( m_file.is_open() == false ) 
	{
		THROW_WIN_EXCEPTION( system_message( IDS_FILE_NOT_OPEN_FOR_WRITING, m_file.file_name() ) )  ;
	}

	write_string( "\r\n<TrU>\r\n" ) ;
	write_created( record->get_created() ) ;
	write_modified( record->get_modified() ) ;
	write_spillage( record ) ;
	write_source( record->get_source_rich() ) ;
	write_trans( record->get_trans_rich() ) ;
	write_string( "</TrU>" ) ;

	return true ;
}



// Function name	: TradosDataExporter::write_spillage
// Description	    : 
// Return type		: bool 
// Argument         : record_pointer record

bool TradosDataExporter::write_spillage( record_pointer record )
{

	std::map< wstring, wstring >::iterator pos ;

	for ( pos = record->data_begin() ; pos != record->data_end() ; ++pos )
	{
		wstring tag = pos->first ;

		if ( tag == L"CrU" || tag == L"CrT" )
		{
			write_string( "<" + string2string( tag, m_old_codepage ) + ">" ) ;

			write_string( string2string( pos->second, m_old_codepage ) + "\r\n" ) ;
		}
	}

	return true ;

}



// Function name	: TradosDataExporter::write_created
// Description	    : 
// Return type		: bool 
// Argument         : const misc_wrappers::date &date

bool TradosDataExporter::write_created( const misc_wrappers::date &date )
{
	write_string( "<CrD>" ) ;

	string date_string = internal_date_to_trados_date ( date ) + "\r\n" ;

	write_string( date_string ) ;

	return true ;
}



// Function name	: TradosDataExporter::write_modified
// Description	    : 
// Return type		: bool 
// Argument         : const misc_wrappers::date &date

bool TradosDataExporter::write_modified( const misc_wrappers::date &date )
{
	write_string( "<ChD>" ) ;
	
	const string date_string = internal_date_to_trados_date ( date ) + "\r\n" ;
	
	write_string( date_string ) ;

	return true ;
}



// Function name	: internal_date_to_trados_date
string TradosDataExporter:: internal_date_to_trados_date( const misc_wrappers::date &date )
{
	CString date_str ;

	const int two_digits = 10 ;
	const int four_digits = 1000 ;
	// day
	if ( date.wDay < two_digits )
	{
		date_str += _T("0") ;
	}
	date_str += int_arg(date.wDay) ;
	// month
	if ( date.wMonth < two_digits )
	{
		date_str += _T("0") ;
	}
	date_str += int_arg(date.wMonth) ;
	// year
	if ( date.wYear < four_digits )
	{
		date_str += _T("0") ;
	}
	date_str += int_arg(date.wYear, false) ;

	// separator
	date_str += _T(", ") ;

	// hour
	if ( date.wHour < two_digits )
	{
		date_str += _T("0") ;
	}
	date_str += int_arg(date.wHour) ;
	date_str += _T(":") ;

	// minute
	if ( date.wMinute < two_digits )
	{
		date_str += _T("0") ;
	}
	date_str += int_arg(date.wMinute) ;
	date_str += _T(":") ;

	// second
	if ( date.wSecond < two_digits )
	{
		date_str += _T("0") ;
	}
	date_str += int_arg(date.wSecond) ;
	
	string trados_date = CT2A( date_str ) ;

	return trados_date ;
}



// Function name	: TradosDataExporter::write_trans
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &trans
bool TradosDataExporter::write_trans( const wstring &trans )
{

	write_string( m_target_tag ) ;

	// pop off our remaining codepages
	while ( m_codepages.empty() == false )
		m_codepages.pop() ;

	charset_info info ;

	m_old_codepage = info.cp_from_lang_str( m_target_lang ) ;

	m_codepages.push( m_old_codepage ) ;
	
	return write_rich_part( trans ) ;

}



// Function name	: TradosDataExporter::write_source
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &source
bool TradosDataExporter::write_source( const wstring &source )
{

	write_string( m_source_tag ) ;

	charset_info info ;

	// pop off our remaining codepages

	while ( m_codepages.empty() == false )
		m_codepages.pop() ;

	m_old_codepage = info.cp_from_lang_str( m_source_lang ) ;
	
	m_codepages.push( m_old_codepage ) ;

	return write_rich_part( source ) ;

}

// Function name	: TradosDataExporter::write_rich_part
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &rich_part
bool TradosDataExporter::write_rich_part( const wstring &rich_part )
{

	write_rich_text( rich_part ) ;

	write_string( "\r\n" ) ;

	return true ;
}



// Function name	: TradosDataExporter::replace_tags
// Description	    : 
// Return type		: bool 
// Argument         : wstring &text
const wstring TradosDataExporter::replace_tags( const wstring &text )
{
	return strip_tags( text ) ;
}

// Function name	: TradosDataExporter::write_plain_text
// Description	    : 
// Return type		: bool 
// Argument         : wstring &text
bool TradosDataExporter::write_plain_text( const wstring &raw_text )
{
	BANNER( "TradosDataExporter::write_plain_text" ) ;

	const wstring text = replace_tags( raw_text ) ;

	const wchar_t *text_stream = text.c_str() ;

	const size_t len = text.length() ;

	const unsigned int current_codepage = m_codepages.top() ;

	// go the slog route...
	UINT old_codepage = _getmbcp() ;
	_setmbcp( current_codepage ) ;
	
	string out ;
	const size_t BUFLEN = 10 ;
	char output_buffer[BUFLEN] ;
	
	size_t end_pos = 0 ;

	BOOL default_char_was_used = FALSE ;

	for ( size_t i=0 ; i<len ; ++i )
	{
		CLEAR_WINERRORS ;

		end_pos = ::WideCharToMultiByte(
			current_codepage,			// code page 
			0 /* 0x00000400 */,			// performance and mapping flags -- = WC_NO_BEST_FIT_CHARS 
			text_stream + i,			// address of wide-character string
			1,							// number of characters in string
			output_buffer,				// address of buffer for new string
			BUFLEN,						// size of buffer
			NULL,						// address of default for unmappable characters
			&default_char_was_used		// address of flag set when default char used
			);
		
		ATLASSERT( ERROR_SUCCESS == ::GetLastError() ) ;

		if ( ! does_char_roundtrip( text_stream + i ) )
		{
			if ( default_char_was_used )
			{
				out += create_unicode_escape( text_stream[i], output_buffer[0] ) ;
			}
			else
				out += create_unicode_escape( text_stream[i], '?' ) ;
		}
		else if ( _ismbblead( (BYTE) output_buffer[0] ) )
		{
			output_buffer[end_pos] = char(0) ;

			out += output_buffer ;
		}
		else
		{
			BYTE the_char = (unsigned char)output_buffer[0] ;
			
			if ( the_char < 0x20 )
			{
				out += escape_char( the_char ) ;
			}
			else if ( the_char == 0x5C || the_char == 0x7B || the_char == 0x7D )
			{
				out += escape_char( the_char ) ;
			}
			else if ( the_char < 0x80 )
			{
				out += output_buffer[0] ;
			}
			else
			{
				out += create_unicode_escape( text_stream[i], '?' ) ;
			}
		} // else
	} // for ( size_t i=0 ; i<len ; ++i )

	_setmbcp( old_codepage ) ;
	write_string( out ) ;

	return true ;
}



// Function name	: TradosDataExporter::write_formatting_tag
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &wide_tag
bool TradosDataExporter::write_formatting_tag( const wstring &wide_tag )
{

	wstring tag( wide_tag ) ;

	str::make_lower( tag ) ;

	if ( tag == L"p" || str::left( tag, 2 ) == L"p " ) // ignore these
	{
		m_p_depth = 1 ;

		write_string( "\\line " ) ;

		return true ;
	}

	if ( tag == L"div" || tag == L"br" || str::left( tag, 4 ) == L"div ") // make this into a newline
	{
		write_string( "\\line " ) ;

		return true ;
	}

	// open tag
	write_string( "{" ) ;

	// bold
	if ( tag == L"b" || tag == L"strong" )
	{
		write_string( "\\b " )  ;

		return true ;
	}
	// italic
	else if ( tag == L"i" || tag == L"em" )
	{
		write_string( "\\i " ) ;

		return true ;
	}
	// underline
	else if ( tag == L"u" )
	{

		write_string( "\\ul " ) ;

		return true ;
	}
	// superscript
	else if ( tag == L"sup" )
	{
		write_string( "\\super " ) ;

		return true ;
	}

	// subscript
	else if ( tag == L"sub" )
	{
		write_string( "\\sub " ) ;

		return true ;
	}
	// span
	// <span style="formatting">
	else if ( str::left( tag, 4 ) == L"span" )
	{
		return write_span( tag ) ;
	}

	// font
	// <font face="font_name">
	else if ( str::left( tag, 4 ) == L"font" )
	{
		return write_font( tag ) ;
	}

	ATLASSERT( "Unknown tag encountered" && false ) ;
	write_string( "\\cf0 " ) ; // dummy tag
	return false ; // there is no tag to close off
}


// Function name	: TradosDataExporter::write_font
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &font_info
bool TradosDataExporter::write_font( const wstring &font_info )
{

	// because we will be popping a codepage when we leave the
	// font tag, no matter what.
	// If we get a face tag, we'll replace this with the actual face font.
	UINT codepage = m_codepages.top() ;
	m_codepages.push( codepage ) ;

	textstream_reader< wchar_t > reader ;

	reader.set_buffer( font_info.c_str() ) ;

	reader.find( L" ", true ) ;

	wstring attribute, value ;

	while ( reader.empty() == false )
	{
		reader.eat_whitespace() ;

		attribute = reader.getline(L'=', true ) ;

		if ( reader.peek() == wchar_t(L'\"') ) // we have quotes
		{
			reader.advance() ;
			reader.getline(value, L'\"', true ) ;
		}
		else 
		{
			reader.getline(value, L" >", true ) ;
		}

		if ( attribute == L"face" )
		{
			write_font_face( value ) ;
		}
		else 
		{
			write_key_value_pair( attribute, value ) ;
		}
	}
	return true ;
}



// Function name	: TradosDataExporter::write_font_face
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &value
bool TradosDataExporter::write_font_face( const wstring &value )
{

	wstring mincho = resource2wstring( IDS_MINCHO, _Module.GetResourceInstance() ) ;

	size_t font_index ;

	if ( value.find( mincho ) != wstring::npos && mincho.empty() == false )
		 font_index = m_fonts.add_font( _T("MS UI Gothic") ) ;
	else
		font_index = m_fonts.add_font( string2tstring( value, m_codepages.top() ) ) ;

	rtf::font_table_entry entry = m_fonts.get_font_entry( font_index ) ;

	BYTE charset = entry.m_font.get_charset() ;

	charset_info info ;
	
	unsigned int codepage = info.cp_from_charset( charset ) ;
	
	if ( codepage != 0 )
	{
		m_codepages.pop() ;
		m_codepages.push( codepage ) ;
	}

	string entry_code = string2string(entry.m_code) ;
	write_string( entry_code ) ;

	return true ;
}

// Function name	: TradosDataExporter::write_rich_text
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &rich_text
bool TradosDataExporter::write_rich_text( const wstring &rich_text )
{
	BANNER( "TradosDataExporter::write_rich_text" ) ;

	// reset the paragraph depth
	m_p_depth = 0 ;

	textstream_reader< wchar_t > reader ;
	reader.set_buffer( rich_text.c_str() ) ;

	wstring local_bit( rich_text ) ;

	str::trim( local_bit ) ;

	wstring left_bit = str::left( local_bit, 3 ) ;

	if ( left_bit == L"<p>" || left_bit == L"<p " )
	{
		reader.find( L">", true ) ; // eat past that baby...
	}

	while( reader.empty() == false )
	{
		const wstring text = reader.getline(L'<', true ) ;

		if ( text.empty() == false ) 
		{
			write_plain_text( text ) ;
		}

		const wstring tag = reader.getline( L'>', true ) ;

		if ( tag.empty() == false ) 
		{
			if ( tag[0] == wchar_t(L'/') )
			{
				wstring tag_part = tag.substr(1) ;

				str::make_lower( tag_part ) ;

				if ( tag_part == L"font" )
				{
					ATLASSERT( m_codepages.empty() == false ) ;
					
					m_codepages.pop() ;

					ATLASSERT( m_codepages.empty() == false ) ;

					if ( m_codepages.empty() )
					{
						m_codepages.push( m_old_codepage ) ;
					}

				}

				if ( tag_part != L"p" && tag_part != L"div" ) // we ignored these guys...
					write_string( "}" ) ;
			}
			else write_formatting_tag( tag ) ;
		}
	}
	return true ;
}



// Function name	: TradosDataExporter::write_span
// Description	    : 
// Return type		: bool 
// Argument         : const wstring &span_info
bool TradosDataExporter::write_span( const wstring &span_info )
{

	textstream_reader< wchar_t > reader ;

	reader.set_buffer( span_info.c_str() ) ;

	reader.find( L"style=", true ) ;

	if ( reader.peek() == wchar_t(L'"') )
	{
		reader.advance() ;
	}

	wstring attribute, value ;
	while ( reader.empty() == false )
	{
		reader.eat_whitespace() ;
		reader.getline( attribute, L":", true ) ;
		reader.eat_whitespace() ; // there may be whitespace after the colon
		reader.getline( value, L" ;\"", true ) ;


		// don't need an opening bracket the second time. So for instance, the
		// directive will look like this:
		// {\cf3 \highlight7 ... }
		write_key_value_pair( attribute, value ) ;
		reader.eat_whitespace() ;
	}
	return true ;
}


// Function name	: TradosDataExporter::write_key_value_pair
// Description	    : 
// Return type		: bool 
// Argument         :  wstring &key
// Argument         : wstring &value
bool TradosDataExporter::write_key_value_pair( wstring &key, wstring &value )
{
	BANNER( "TradosDataExporter::write_key_value_pair" ) ;

	string tag ;

	// bold
	if ( key == L"font-weight" ) // easy stuff first
	{
		if ( value == L"bold" ) 
			tag += ( "\\b " );
		else if ( value == L"italic" ) 
			tag += ( "\\i " );
	}
	// foreground color
	else if ( key == L"color" ) 
	{ 
		tag << "\\cf" << get_color( value ) << " " ; 
	}
	// background color
	else if ( key == L"background" ) 
	{ 
		tag << "\\highlight" << get_color( value ) << " " ; 
	}
	// default -- we don't know what the heck this is
	else tag += "\\cf0 " ;

	write_string( tag ) ;
	return true ;
}

// Function name	: get_color
// Description	    : 
// Return type		: string TradosDataExporter:: 
// Argument         : const wstring &value
string TradosDataExporter:: get_color( const wstring &value )
{
	BANNER( "get_color" ) ;

	if ( m_colors.find( value ) != m_colors.end() )
		return m_colors[value] ;
	return string("0") ;
}

// Function name	: TradosDataExporter::write_string
// Description	    : 
// Return type		: void 
// Argument         : const string &str
void TradosDataExporter::write_string( const string &str )
{

	try
	{
		m_file.write( str ) ;
	}
	catch ( CException &e ) 
	{
		e.add_to_message( (LPCTSTR)system_message( IDS_MSG_WRITE_FAILED, str.c_str() ) ) ;

		throw CException( e ) ;
	} 
}

string TradosDataExporter::create_unicode_escape(wchar_t c, char best_fit)
{
	string out ;
	out += "\\uc1\\u" ;
	out += ulong2string( (unsigned long)(unsigned short)c, 10  ) ;
	out += " " ;
	out += best_fit ;

	return out ;
}

bool TradosDataExporter::does_char_roundtrip( const wchar_t *c )
{
	const size_t BUFLEN = 10 ;
	char output_str[BUFLEN] ;
	BOOL default_used = FALSE ;

	int bytes_needed = ::WideCharToMultiByte( m_codepages.top(), 0, c, 1, output_str, BUFLEN, NULL, &default_used ) ;

	if ( bytes_needed == 0 )
	{
		return false ;
	}
	if ( default_used )
	{
		return false ;
	}

	wchar_t input_str[3]  = {0}; 
	::MultiByteToWideChar( m_codepages.top(), 0, output_str, bytes_needed, input_str, 2 ) ;

	return ( input_str[0] == *c ) ;
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* multiterm_data_exporter                                              */
/************************************************************************/

multiterm_data_exporter_55::multiterm_data_exporter_55(CProgressListener *listener) :
	m_listener(listener),
	m_source_cp(CP_ACP),
	m_trans_cp(CP_ACP)
{
}

multiterm_data_exporter_55::~multiterm_data_exporter_55() 
{ 
	m_file->close(); 
}
void multiterm_data_exporter_55::set_source( const string source ) 
{ 
	m_source_lang = source ; 
	charset_info info ;
	m_source_cp = info.cp_from_lang_str( m_source_lang ) ;
}
void multiterm_data_exporter_55::set_target( const string target ) 
{ 
	m_target_lang = target ; 
	charset_info info ;
	m_trans_cp = info.cp_from_lang_str( m_target_lang ) ;
}
void multiterm_data_exporter_55::open_destination( const CString destination )
{
	OutputDeviceFile *file_device = new OutputDeviceFile ;
	file_device->open(destination) ;
	m_file = device_ptr(file_device) ;
}
void multiterm_data_exporter_55::write_record( const record_pointer &rec )
{
	record_pointer record = rec ;
	
	string output ;
	output.reserve( 1000 ) ;
	
	// source
	wstring source_w = record->get_source_plain() ;
	string source = string2string( source_w, m_source_cp ) ;
	output += write_line( m_source_lang, source ) ;
	
	// target
	wstring trans_w = record->get_trans_plain() ;
	string trans = string2string( trans_w, m_trans_cp ) ;
	output += write_line( m_target_lang, trans ) ;
	
	wstring context_w = record->get_context_plain() ;
	string context = string2string( context_w, m_source_cp ) ;
	if (context.empty())
	{
		context = "-" ;
	}
	output += write_line( "Notes", context ) ;
	
	output += "***\r\n" ;
	
	m_file->write( output ) ;
}

string multiterm_data_exporter_55::write_line( const string &lang, const string &val )
{
	return (format("<%1%>%2%\r\n")
				% lang
				% prep_string(val)).str() ;
}

string multiterm_data_exporter_55::prep_string( const string &raw_string )
{
	string str = raw_string ;
#pragma warning( disable:4239 ) // A reference that is not to 'const' cannot be bound to a non-lvalue
	boost::replace_all(str, "\r\n", " " ) ;
//	str::replace_all( str, string("\r\n"), string(" ") ) ;
	str::replace_all( str, string("\r"), string(" ") ) ;
	str::replace_all( str, string("\n"), string(" ") ) ;
	str::replace_all( str, string("\t"), string(" ") ) ;
	str::replace_all( str, string("&lt;"), string("<") ) ;
#pragma warning( default:4239 ) // A reference that is not to 'const' cannot be bound to a non-lvalue
	
	return str ;
}

void multiterm_data_exporter_55::export_gloss( memory_engine::memory_pointer &mem )
{
	m_listener->OnProgressInit( mem->get_location(), 0, mem->size() ) ;

	long i = 0 ;
	foreach(record_pointer record, mem->get_records())
	{
		write_record( record ) ;
		++i ;
		m_listener->OnProgressWriteUpdate( i ) ;
	}

	m_listener->OnProgressDoneWrite( i ) ;
}


void multiterm_data_exporter_6::open_destination( const CString destination )
{
	OutputDeviceFile *file_device = new OutputDeviceFile ;
	file_device->open(destination) ;
	file_device->m_file.write_16le_bom() ;	
	m_file = device_ptr(file_device) ;
}

void multiterm_data_exporter_6::write_record( const memory_engine::record_pointer &rec )
{
	wstring source = rec->get_source_plain() ;
	wstring trans = rec->get_trans_plain() ;
	wstring context = rec->get_context_plain() ;

	if (context.empty())
	{
		context = L"-" ;
	}
	write_line( source, trans, context ) ;
}

void multiterm_data_exporter_6::write_line( wstring &col1, wstring &col2, wstring &col3 )
{
	wstring line = (wformat(L"%1%\t%2%\t%3%\n")
						% prep_string(col1)
						% prep_string(col2)
						% prep_string(col3)).str();
	m_file->write( line ) ;
}

wstring multiterm_data_exporter_6::prep_string( wstring &line )
{
#pragma warning( disable:4239 ) // A reference that is not to 'const' cannot be bound to a non-lvalue
	str::replace_all( line, wstring(L"\r\n"), wstring(L" ") ) ;
	str::replace_all( line, wstring(L"\r"), wstring(L" ") ) ;
	str::replace_all( line, wstring(L"\n"), wstring(L" ") ) ;
	str::replace_all( line, wstring(L"\t"), wstring(L" ") ) ;
#pragma warning( default:4239 ) // A reference that is not to 'const' cannot be bound to a non-lvalue

	return line ;
}