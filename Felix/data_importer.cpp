// trados_data_importer.cpp: implementation of the trados_data_importer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "data_importer.h"
#include "File.h"
#include <stack>

#include "Exceptions.h"
#include "ArmadilloStuff.h"

#include "charset_info.h"	// convert lang ids to code pages
#include "input_device_file.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

using namespace except ;
using namespace mem_engine ;


// Function name	: trados_data_importer::open_data_source
bool  trados_data_importer::open_data_source(const CString &source_name)
{
	InputDeviceFile input ;
	input.ensure_file_exists(source_name);
	
	m_file_size = file::file::size( source_name )  ;

	m_file_name = CT2A( source_name );
	m_buffer.set_buffer( (const char*)m_view.create_view_readonly( source_name ) ) ;

	return true ;
}

 

// Function name	: trados_data_importer::size
// Description	    : 
// Return type		: size_t  
size_t trados_data_importer::size()
{
	m_buffer.rewind() ;

	size_t s = 0 ;
	for ( s = 0 ; m_buffer.find( tru_start_tag.c_str(), true ) ; ++s )
	{
		if ( m_buffer.empty() )
			break ;
	}

	m_buffer.rewind() ;
	return s ;
}

string trados_data_importer::get_tru()
{
	// Now get from this:
	//		\r\n<SomeTag>...
	// to this:
	//		<SomeTag>...
	m_buffer.eat_whitespace() ;

	ATLASSERT( m_buffer.current_is( '<' ) ) ;

	// get this record's text and put it in another buffer
	// So if we mess up, we only mess up the current record

	// start pos
	const reader_type::bookmark_type bookmark_start = m_buffer.get_current_pos() ;
	// end pos
	m_buffer.find( tru_end_tag.c_str() ) ; // eat up to the end-of-record tag (</TrU>)
	const reader_type::bookmark_type bookmark_end = m_buffer.get_current_pos() ;

	// put it in a string for permanence -- over use of memory?
	const string node( bookmark_start, bookmark_end ) ;

	ATLASSERT( node.empty() == false ) ;

	return node ;
}
 

// Function name	: trados_data_importer::get_next_record
// Description	    : 
// Return type		: bool  
bool trados_data_importer::get_next_record( )
{
	try
	{
		// clear out that bad boy...
		clear_record() ;
		// get the next one

		// wind up past next tag
		// we start with this:
		//		... \r\n<TrU>\r\n<SomeTag>...
		// we end up with this:
		//		\r\n<SomeTag>...
		if ( ! m_buffer.find( tru_start_tag.c_str(), true )  ) // true means eat the tag
		{
			return false ; // the tag wasn't found = no more entries
		}

		ATLASSERT( m_buffer.empty() == false ) ;

		const string node = get_tru() ;

		// the buffer for this record
		m_record_buffer.set_buffer( node.c_str() ) ;

		ATLASSERT( m_record_buffer.empty() == false ) ;

		// read in the whole record
		while ( ! m_record_buffer.empty() )
		{
			// get the text up to the end of line.
			const string line = m_record_buffer.getline_delims("\r\n" ) ;
			ATLASSERT( line.empty() == false ) ;
			// process it
			if ( ! process_line( line ) )
			{
				ATLASSERT( "Failed to process line" && false ) ;
				return false ;
			}
			// eat any whitespace
			m_record_buffer.eat_whitespace() ;
		}


	}
	catch ( CException &e )
	{
		CException local( e ) ;

		local.set_bottom_message( R2TS( IDS_PROMPT_DISCARD_ENTRY ) ) ;
		if ( IDNO == local.notify_user( R2TS( IDS_MSG_PARSE_FAILED ), MB_YESNO ) )
		{
			m_listener->OnProgressDoneLoad(0) ;
			throw except::CException( IDS_MSG_PARSE_FAILED ) ;
		}
	}

	// signal that we have a record
	return true ;
}

  

// Function name	: trados_data_importer::get_all_records
// Description	    : 
// Return type		: bool  
// Argument         : std::set< record_pointer > &records
bool  trados_data_importer::get_all_records( trans_set &records )
{
	BANNER( "trados_data_importer::get_all_records" ) ;

	// anti-crack stuff!!!
//	if ( ! armadillo::devious_crack_check_1() )
//	{
//		PostQuitMessage((UINT)-1) ;
//		return false ;
//	}
	// <--

	if ( ! load_font_table() )
	{
		return false ;
	}

	CProgressBarCtrl progress_bar ;
	if ( m_listener != NULL )
	{
		CString file_name = m_file_name.c_str() ;
		m_listener->OnProgressInit( file_name, 0, size() ) ;
	}

	while ( true )
	{
		try
		{
			// clear out that bad boy...
			m_current_record = record_pointer(new record_local()) ;
			// get the next one
			if ( ! get_next_record( ) )
			{
				if ( -1 == _setmbcp( m_old_codepage ) )
					_setmbcp( CP_ACP ) ;
				return true ;
			}
			// add it!
			records.insert( m_current_record ) ;

			if ( m_background.check_iterations() )
			{
				progress_bar.SetPos( records.size() ) ;
			}
		}
		catch ( CException &e )
		{
			CException local( e ) ;
			
			local.set_bottom_message( R2TS( IDS_PROMPT_DISCARD_ENTRY ) ) ;
			if ( IDNO == local.notify_user( R2TS( IDS_MSG_PARSE_FAILED ), MB_YESNO ) )
				throw except::CException( R2TS( IDS_MSG_PARSE_FAILED ) ) ;
		}
	}
	return true ;

}

  

// Function name	: trados_data_importer::read_the_tag
// Description	    : This is to read the initial Trados tag that 
//						tells us what this field is
// Return type		: string 
wstring trados_data_importer::read_the_tag( )
{
	string html_tag ;
	m_line_buffer.get( html_tag ) ;
	ATLASSERT( html_tag == "<" ) ;

	// get the tag
	// also allow for a spaces here -- that means this is a seg tag
	const string tag = m_line_buffer.getline('>', true ) ;
	ATLASSERT( m_line_buffer.empty() == false ) ;

	const string SEG = "SEG " ;
	if ( boost::iequals( str::left( tag, SEG.size() ), SEG ) ) // SEG tag!
	{
		textstream_reader< char > tag_buffer( tag.c_str() ) ;
		tag_buffer.find( " ", true ) ;
		tag_buffer.eat_whitespace() ;
		ATLASSERT( tag_buffer.current_is( 'L' ) ) ;
		tag_buffer.advance() ;
		ATLASSERT( tag_buffer.current_is( '=' ) ) ;
		tag_buffer.advance() ;

		const string lang_tag = tag_buffer.getline('>', true ) ;

		// clear out current codepages
		while ( ! m_codepage_stack.empty() )
		{
			m_codepage_stack.pop() ;
		}

		const charset_info info ;
		m_old_codepage = info.cp_from_lang_str( lang_tag ) ;
		
		m_codepage_stack.push( get_old_codepage() ) ;

		if ( lang_tag == m_source_language )
			return L"source" ;
		if ( lang_tag == m_target_language )
			return L"target" ;

		// some other misc language -- return it as-is
		return string2wstring( tag + " L=" + lang_tag ) ;

	}
	return string2wstring( tag ) ;
}

  
  

// Function name	: trados_data_importer::get_language_codes
// Description	    : 
// Return type		: size_t 
// Argument         : language_code_set &languages
size_t trados_data_importer::get_language_codes( language_code_set &languages )
{
	m_buffer.rewind() ;

	const string firstline = m_buffer.getline() ;
	if (firstline.find("<TWBExportFile version=") != string::npos)
	{
		m_is_new_type = true ;
	}

	const int MAX_SEGS = 100 ;
	const int MAX_CODE_LEN = 10 ;
	int num_segs = 0 ;
	while ( m_buffer.find( "<Seg L=", true ) ==  true && num_segs < MAX_SEGS)
	{
		const string_type language_code = m_buffer.getline('>', true ) ;
		if ( language_code.empty() == false && language_code.length() < MAX_CODE_LEN )
		{
			languages.insert( string2tstring( language_code ) ) ;
		}
		num_segs++ ;
	}

	m_buffer.rewind() ;
	return languages.size() ;
}

  

// Function name	: trados_data_importer::load_font_table
// Description	    : 
// Return type		: bool  
bool  trados_data_importer::load_font_table()
{
	m_fonts.parse_fonttbl( m_buffer ) ;

	m_file_pos = m_buffer.get_offset() ;

	return true ;
}


// Function name	: trados_data_importer::next_tag_is_end_tag
// Description	    : 
// Return type		: bool  
bool trados_data_importer::next_tag_is_end_tag()
{
	m_line_buffer.eat_whitespace() ;
	TRUE_ENFORCE( m_line_buffer.current_is('<'), CString( TEXT("Bad trados format -- expected start tag.") ) + CA2CT( dump_context().c_str() ) ) ;

	textstream_reader<char>::bookmark_type bookmark = m_line_buffer.get_current_pos() ;

	m_line_buffer.advance() ;

	const bool is_end_tag = ( m_line_buffer.current_is('/') ) ;

	m_line_buffer.set_pos( bookmark ) ;

	return is_end_tag ;
}




// Function name	: trados_data_importer::get_font_tag
// Description	    : 
// Return type		: string 
// Argument         : string_type &code
wstring trados_data_importer::get_font_tag(const string_type &code)
{
	try
	{
		rtf::font_table_entry entry = m_fonts.get_font_entry_from_code( _T("\\") + string2tstring( code ) ) ;
		
		const wstring tag = L"<font face=\"" + string2wstring( entry.m_name ) + L"\">" ;

		const charset_info info ;
		const unsigned int codepage = info.cp_from_charset( entry.m_font.get_charset() ) ;
		
		const UINT old_mbcp = _getmbcp() ;

		if ( -1 == _setmbcp( codepage ) )
		{
			if ( -1 == _setmbcp( m_old_codepage ) )
			{
				 _setmbcp( old_mbcp ) ;
			}
		}

		m_codepage_stack.push( codepage ) ;

		return tag ;
	}
	catch( CException &e )
	{
		e ;
		return wstring() ;
	}
}

wstring tmx_data_importer::get_font_tag(const wstring code)
{
		const rtf::font_table_entry entry = m_fonts.get_font_entry_from_code( _T("\\") + string2tstring( code ) ) ;

		return L"<font face=\"" + string2wstring( entry.m_name ) + L"\">" ;
}

// Function name	: trados_data_importer::dump_context
string trados_data_importer::dump_context( )
{
	BANNER( "trados_data_importer::dump_context" ) ;

	reader_type temp_reader( m_record_buffer ) ;
	temp_reader.rewind() ;

	const string context = "\r\rContext:\r\r" ;
	return context + temp_reader.get_current_pos() ;
}

// Function name	: trados_data_importer::handle_f_tag
// Description	    : 
// Return type		: wstring 
// Argument         : tag_stack &tags
wstring trados_data_importer::handle_f_tag(tag_stack &tags)
{
	const string tag = m_line_buffer.getline_delims(" \\", false ) ;

	// "field" tag -- used by TRADOS to mark symbols.
	if ( tag == "field" )
	{
		tags.push( wstring() ) ; // ignore this bad boy
		return wstring() ;
	}

	if ( tag == "fldinst" ) // we could have a hyperlink...
	{
		const string see_what = m_line_buffer.getline('}', true ) ;
		reader_type wot( see_what.c_str() ) ;
		reader_type::bookmark_type bookmark = wot.get_current_pos() ;
		if ( wot.find( "HYPERLINK", true ) )
		{
			wot.eat_whitespace() ;
			const string hlink = wot.getline(' ', true ) ;
			ATLASSERT( wot.empty() ) ;
			tags.push(L"a") ;
			ATLASSERT ( hlink[0] == '"' ) ; 
			const wstring anchor_tag = L"<a href=" + string2wstring( hlink ) + L">" ;

			// we are going to skip over a lot of junk here ... don't try this at home kiddies!
			m_line_buffer.find( "fldrslt ", true ) ;
			tags.push( wstring() ) ;

			return anchor_tag ;
		}
		wot.set_pos( bookmark ) ;
		if ( wot.find( "SYMBOL", true ) )
		{
			tags.push( wstring() ) ;
			return wstring(L"<font color=gray>#</font>") ;
		}
		tags.push( wstring() ) ;
		return wstring() ;
	}

	// do we have a font tag?
	if ( tag.substr(1).find_first_not_of( "0123456789" ) == string_type::npos )
	{
		try
		{
			const wstring font_tag = get_font_tag( tag ) ;
			if ( font_tag.empty() )
			{
				tags.push( wstring() ) ;
			}
			else
				tags.push( L"font" ) ;
	
			return font_tag ;
		}
		catch ( CException &e ) // something has gone terribly wrong...
		{ 
			static bool been_here = false ;
			if ( ! been_here )
			{
				been_here = true ;
				CException local( e ) ;
				local.set_bottom_message( _T("Do you wish to continue the import (this message will not be shown for subsequent instances)?") ) ;
				if ( IDNO == local.notify_user(  
						CString( _T("Unable to retrieve font. Please make sure file is not corrupt.")  ), 
						MB_YESNO ) )
				{
					e.add_to_message( _T("Import aborted by user.") ) ;
					throw except::CException( e ) ;
				}
			}
		}
	}
	
	// unknown tag, just ignore...
	tags.push( wstring() ) ;
	
	return wstring() ;
}


wstring tmx_data_importer::handle_f_tag(tag_stack &tags)
{
	const wstring tag = m_line_buffer.getline_delims(L" \\", false ) ;

	// "field" tag -- used by TRADOS to mark symbols.
	if ( tag == L"field" )
	{
		tags.push( wstring() ) ; // ignore this bad boy
		return wstring() ;
	}

	if ( tag == L"fldinst" ) // we could have a hyperlink...
	{
		const wstring see_what = m_line_buffer.getline(L'}', true ) ;
		reader_type wot( see_what.c_str() ) ;
		reader_type::bookmark_type bookmark = wot.get_current_pos() ;
		if ( wot.find( L"HYPERLINK", true ) )
		{
			wot.eat_whitespace() ;
			const wstring hlink = wot.getline(L' ', true ) ;
			ATLASSERT( wot.empty() ) ;
			tags.push(L"a") ;

			ATLASSERT ( hlink[0] == L'"' ) ; 
			
			const wstring anchor_tag = L"<a href=" + hlink + L">" ;

			// we are going to skip over a lot of junk here ... don't try this at home kiddies!
			m_line_buffer.find( L"fldrslt ", true ) ;
			tags.push( wstring() ) ;

			return anchor_tag ;
		}
		wot.set_pos( bookmark ) ;
		if ( wot.find( L"SYMBOL", true ) )
		{
			tags.push( wstring() ) ;
			return wstring(L"<font color=gray>#</font>") ;
		}
		tags.push( wstring() ) ;
		return wstring() ;
	}

	// do we have a font tag?
	if ( tag.substr(1).find_first_not_of( L"0123456789" ) == string_type::npos )
	{
		try
		{
			wstring font_tag = get_font_tag( tag ) ;
			if ( font_tag.empty() )
			{
				tags.push( wstring() ) ;
			}
			else
			{
				tags.push( L"font" ) ;
			}

			return font_tag ;
		}
		catch ( CException &e ) // something has gone terribly wrong...
		{ 
			static bool been_here = false ;
			if ( ! been_here )
			{
				been_here = true ;
				CException local( e ) ;
				local.set_bottom_message( _T("Do you wish to continue the import (this message will not be shown for subsequent instances)?") ) ;
				if ( IDNO == local.notify_user(  
					CString( _T("Unable to retrieve font. Please make sure file is not corrupt.")  ), 
					MB_YESNO ) )
				{
					e.add_to_message( _T("Import aborted by user.") ) ;
					throw except::CException( e ) ;
				}
			}
		}
	}

	// unknown tag, just ignore...
	tags.push( wstring() ) ;

	return wstring() ;
}


// Function name	: trados_data_importer::process_line
// Description	    : 
// Return type		: bool 
// Argument         : const string &line
bool trados_data_importer::process_line(const string &line)
{
	// set our buffer for this line
	m_line_buffer.set_buffer( line.c_str() ) ;
	// get the tag
	const wstring wrapper_tag = read_the_tag( ) ;

	// do we need to process this guy?
	if ( wrapper_tag == L"CrD" ) // date created
	{
		const string date_string = m_line_buffer.get_current_pos() ;
		m_current_record->set_created( TradosDate2FelixDate( string2wstring( date_string ) ) ) ;
		return true ;
	}
	else if ( wrapper_tag == L"ChD" ) // last modified
	{
		const string date_string = m_line_buffer.get_current_pos() ;
		m_current_record->set_modified( TradosDate2FelixDate( string2wstring( date_string ) ) ) ;
		return true ;
	}
	else if ( wrapper_tag == L"UsC" ) // usage count
	{
		const string usage_count = m_line_buffer.get_current_pos() ;
		m_current_record->set_item( wrapper_tag, string2wstring( usage_count ) ) ;
		return true ;
	}
	else if ( wrapper_tag == L"CrU" ) // creating user
	{
		const string user = m_line_buffer.get_current_pos() ;
		m_current_record->set_creator(string2wstring(user)) ;
		return true ;
	}
	else if ( wrapper_tag == L"ChU" ) // changing user
	{
		const string user = m_line_buffer.get_current_pos() ;
		m_current_record->set_modified_by(string2wstring( user )) ;
		return true ;
	}
	else if ( wrapper_tag == L"UsD" ) // using date
	{
		const string date = m_line_buffer.get_current_pos() ;
		m_current_record->set_item( wrapper_tag, string2wstring( date ) ) ;
		return true ;
	}

	// ======================================================
	// OK, so we will actually have to process this baby...
	// ======================================================
	
	if (m_is_new_type)
	{
		m_html.erase() ;
		while (! m_line_buffer.empty())
		{
			const string chunk = m_line_buffer.getline('{') ;
			m_line_buffer.jump_to_first_of('}', true) ;
			m_html += string2wstring(chunk, CP_UTF8) ;
		}
	}
	else
	{
		process_formatted_line_old();
	}


	// now we decide where to put the html text
	if ( wrapper_tag == L"source" )
	{
		m_current_record->set_source( m_html );
	}
	else if ( wrapper_tag == L"target" )
	{
		m_current_record->set_trans( m_html ) ;
	}
	else // some misc info -- just duplicate
		// We have to do it here and not above, 
		// because if it was an extra SEG tag, we need the
		// codepage info from the L atrribute in order to convert properly into unicode
	{
		m_current_record->set_item( wrapper_tag, m_html ) ;
		return true ;
	}

	return true ;
}


// Function name	: trados_data_importer::handle_close_bracket
// Description	    : 
// Return type		: bool 
// Argument         : tag_tracker &tags
bool trados_data_importer::handle_close_bracket(tag_tracker &tags)
{
	ATLASSERT( m_line_buffer.current_is( char('}') ) ) ;
	// this may be just a plain-old }, in which case we won't have
	// any formatting to close off, and will only have the end-of-tag 
	// tag left in our stack.
	if ( tags.empty() == false )
	{
		// get all the formatting tags that we pushed on when we
		// found the {
		close_tags_top(tags);		
		
		// eat the }
		m_line_buffer.advance() ;
		return true ;
	}
	// get the }
	string bracket ;
	m_line_buffer.get( bracket ) ;
	m_html += string2wstring( bracket ) ;
	return true ;
}

bool trados_data_importer::close_off_tags(tag_tracker &tags)
{
	while ( tags.empty() == false )
	{
		close_tags_top(tags);		
	}

	return true ;
}
// Function name	: trados_data_importer::handle_close_bracket
// Description	    : 
// Return type		: bool 
// Argument         : tag_tracker &tags
bool tmx_data_importer::handle_close_bracket(tag_tracker &tags)
{
	ATLASSERT( m_line_buffer.current_is( wchar_t(L'}') ) ) ;
	// this may be just a plain-old }, in which case we won't have
	// any formatting to close off, and will only have the end-of-tag 
	// tag left in our stack.
	if ( tags.empty() == false )
	{
		// get all the formatting tags that we pushed on when we
		// found the {
		tag_stack format_tags = tags.top() ;
		tags.pop() ;
		while ( false == format_tags.empty() ) 
		{
			// it could be empty if we ignored this formatting tag
			if ( format_tags.top().empty() == false )
			{
				m_html += L"</" + format_tags.top() + wchar_t(L'>') ;
			}
			format_tags.pop() ;
		}
		// eat the }
		m_line_buffer.advance() ;
		return true ;
	}
	// get the }
	wstring bracket ;
	m_line_buffer.get( bracket ) ;
	m_html +=  bracket ;
	return true ;
}


size_t trados_data_importer::file_size()
{
	return m_file_size ;
}

size_t trados_data_importer::get_pos()
{
	const size_t pos = m_file_pos ;
	m_file_pos = m_buffer.get_offset( pos ) ;
	return m_file_pos ;
}

void trados_data_importer::rewind()
{
	m_buffer.rewind() ;
	m_file_pos = 0 ;
}

bool trados_data_importer::get_language_code(language_code_set &languages)
{
	if ( ! m_buffer.find( "<Seg L=", true )  )
		return false ;

	const string_type language_code = m_buffer.getline('>', true ) ;
	if ( language_code.empty() == false && language_code.length() < 10 )
		languages.insert( string2tstring( language_code ) ) ;

	return true ;
}

string trados_data_importer::read_backslashed_sequence()
{
	string backslashed_sequence ;
	m_line_buffer.getline( backslashed_sequence, " \\", false ) ;
	return backslashed_sequence ;
}

// tmx_data_importer

wstring tmx_data_importer::seg2html( const wstring seg_text )
{
	const wstring stripped_text = strip_tags(seg_text) ;

	m_line_buffer.set_buffer(stripped_text.c_str()) ;

	// default values
	m_num_to_ignore = 1 ;		// number of bytes to ignore after Unicode character is read
	m_bold_on = (false) ;
	m_italic_on = (false) ;
	m_underline_on = (false) ;

	// clear out the current html text
	m_html.erase() ;

	// keep a record of tags
	tag_tracker tags ;

	while ( m_line_buffer.empty() == false ) // read in the whole line
	{
		const wstring chunk = m_line_buffer.getline_delims(L"{}&<\\", false ) ;

		m_html += chunk ;

		switch ( m_line_buffer.peek() )
		{
			// start of rtf-style formatting
		case char( L'{'):
			handle_open_bracket( tags );
			break ;
			// end of rtf-style formatting
		case char( L'}' ):
			handle_close_bracket( tags );
			break ;
			// needs to be in html format
		case char( L'&' ):
		case char( L'<' ):
			handle_needs_entity_code( m_line_buffer.peek() );
			break ;
			// escaped char
		case char('\\'):
			handle_backslash( ) ;
			break ;
		}
	}

	// We should have just 1 tag left, namely the closing tag (e.g. Seg)
	// However, we will allow tags to not be closed off, and just quietly do so ourselves.
	close_off_tags( tags ) ;

	if ( m_bold_on )
	{
		m_html += L"</B>" ;
	}
	if ( m_italic_on )
	{
		m_html += L"</I>" ;
	}
	if ( m_underline_on )
	{
		m_html += L"</U>" ;
	}

	return m_html ;

}

bool trados_data_importer::handle_backslash( )
{
	ATLASSERT( m_line_buffer.current_is( char('\\') ) ) ;

	m_line_buffer.advance() ; // skip this char

	// One possible escape is an escaped tab, which will be "tab"
	// This temporary buffer will relieve us of the hassle of rewinding
	// to where we started if the escaped sequence turns out to be other than
	// a tab
	if ( m_line_buffer.current_is( '\'' ) ) // It's an escaped char!!
	{
		m_line_buffer.advance() ;
		string num ;
		while (  m_line_buffer.is_hexint_digit() && num.size() < 2 ) 
		{
			m_line_buffer.get( num ) ;
		}
		if (! boost::all(num, boost::is_xdigit())) 
		{
			return true ;
		}
		m_line_buffer.eat_if( ' ' ) ;

		string escaped_text ;
		escaped_text += get_escaped_char(num) ;

		while ( m_line_buffer.current_is( '\\' ) )
		{
			m_line_buffer.advance() ;
			if ( ! m_line_buffer.current_is( '\'' ) )
			{
				m_line_buffer.unget() ;
				m_html += string2wstring( escaped_text, m_codepage_stack.top() ) ;
				return true ;
			}
			// this is another escaped character ( we found ' )
			m_line_buffer.advance() ; // eat it
			num.erase() ;
			m_line_buffer.get( num, 2 ) ; // always 2 digits
			escaped_text += get_escaped_char(num) ;
		}

		m_html += string2wstring( escaped_text, m_codepage_stack.top() ) ;
		return true ;
	}
	else 
	{
		const string backslashed_sequence = read_backslashed_sequence() ;
		// newline
		if ( backslashed_sequence == line_str )
		{
			m_html += L"\r\n" ;
			m_line_buffer.eat_if( ' ' ) ;
		}
		// tab
		else if ( backslashed_sequence == tab_str )
		{
			m_html += wchar_t(L'\t') ; // accept no tab substitutes!
			m_line_buffer.eat_if( ' ' ) ;
		}
		// Unicode character
		else if (	backslashed_sequence.size() >= 2 && 
			backslashed_sequence[0] == 'u' && 
			isdigit( static_cast<unsigned char>(backslashed_sequence[1]) ) )
		{
			m_line_buffer.eat_if( ' ' ) ;
			if ( m_num_to_ignore )
			{
				m_line_buffer.advance( m_num_to_ignore ) ;
			}

			ATLASSERT( backslashed_sequence.substr(1).find_first_not_of( "1234567890" ) == string::npos ) ;

			m_html << L"&#" << string2wstring( backslashed_sequence.substr(1) ) << L";" ;
		}
		// ignore directive
		else if ( ! strncmp( backslashed_sequence.c_str(), "uc", 2 ) && 
				isdigit( static_cast<unsigned char>(backslashed_sequence[2]) ) )
		{
			m_line_buffer.eat_if( ' ' ) ;

			ATLASSERT( backslashed_sequence.substr(2).find_first_not_of( "1234567890" ) == string::npos ) ;

			m_num_to_ignore = (int)string2long( backslashed_sequence.substr(2) ) ;
		}
		// bold
		else if ( backslashed_sequence == "b0" )
		{
			m_html += L"</B>" ; // 
			m_line_buffer.eat_if( ' ' ) ;
			m_bold_on = false ;
		}
		else if ( backslashed_sequence == "b" )
		{
			m_html += L"<B>" ; // 
			m_line_buffer.eat_if( ' ' ) ;
			m_bold_on = true ;
		}
		// italic
		else if ( backslashed_sequence == "i0")
		{
			m_html += L"</I>" ; // 
			m_line_buffer.eat_if( ' ' ) ;
			m_italic_on = false ;
		}
		else if ( backslashed_sequence == "i" )
		{
			m_html += L"<I>" ; // 
			m_line_buffer.eat_if( ' ' ) ;
			m_italic_on = true ;
		}
		// underline
		else if ( backslashed_sequence == "ul0" )
		{
			m_html += L"</U>" ; // 
			m_line_buffer.eat_if( ' ' ) ;
			m_underline_on = false ;
		}
		else if ( backslashed_sequence == "ul" )
		{
			m_html += L"<U>" ; // 
			m_line_buffer.eat_if( ' ' ) ;
			m_underline_on = true ;
		}
		// escaped character		
		else
		{
			string escaped_char ;
			m_line_buffer.get( escaped_char ) ; // get the char
			m_line_buffer.eat_if( ' ' ) ;
			m_html += string2wstring( escaped_char ) ;
		}
	}

	return true ;
}


bool tmx_data_importer::handle_backslash( )
{
	ATLASSERT( m_line_buffer.current_is( char('\\') ) ) ;

	m_line_buffer.advance() ; // skip this char

	// One possible escape is an escaped tab, which will be "tab"
	// This temporary buffer will relieve us of the hassle of rewinding
	// to where we started if the escaped sequence turns out to be other than
	// a tab
	if ( m_line_buffer.current_is( L'\'' ) ) // It's an escaped char!!
	{
		m_line_buffer.advance() ;
		string_type num ;
		while (  m_line_buffer.is_hexint_digit() && num.size() < 2 ) 
		{
			m_line_buffer.get( num ) ;
		}
		if ( ! str::is_hexint_rep( num ) ) 
		{
			return true ;
		}

		m_line_buffer.eat_if( wchar_t(L' ') ) ;

		long escaped_char = string2long( num, 16 ) ;
		if ( escaped_char >= 256 && str::is_uint_rep( num ) ) 
		{
			escaped_char = string2long( num, 10 ) ;
		}

		ATLASSERT( escaped_char < 256 ) ; // it must be in range...

		wstring escaped_text ;

		escaped_text += static_cast< wchar_t >( escaped_char ) ;

		while ( m_line_buffer.current_is( L'\\' ) )
		{
			m_line_buffer.advance() ;
			if ( ! m_line_buffer.current_is( L'\'' ) )
			{
				m_line_buffer.unget() ;
				m_html += escaped_text ;
				return true ;
			}
			// this is another escaped character ( we found ' )
			m_line_buffer.advance() ; // eat it
			num.erase() ;
			m_line_buffer.get( num, 2 ) ; // always 2 digits
			escaped_char = (char)string2long( num, 16 ) ; // convert to char
			ATLASSERT( -128 <= escaped_char && escaped_char <= 127 ) ; // it must be in range...
			escaped_text += static_cast< wchar_t >( escaped_char ) ;
		}

		m_html += escaped_text ;
		return true ;
	}
	else 
	{
		m_line_buffer.eat_if( wchar_t(L' ') ) ;

		const wstring backslashed_sequence = m_line_buffer.getline_delims(L" \\", false ) ;

		// newline
		if ( backslashed_sequence == L"line" )
		{
			m_html += L"\r\n" ;
		}
		// tab
		else if ( backslashed_sequence == L"tab" )
		{
			m_html += wchar_t(L'\t') ; // accept no tab substitutes!
		}
		// Unicode character
		else if (	backslashed_sequence.size() >= 2 && 
			backslashed_sequence[0] == L'u' && 
			iswdigit( backslashed_sequence[1] ) )
		{
			if ( m_num_to_ignore )
			{
				m_line_buffer.advance( m_num_to_ignore ) ;
			}

			ATLASSERT( backslashed_sequence.substr(1).find_first_not_of( L"1234567890" ) == string::npos ) ;

			m_html << L"&#" << backslashed_sequence.substr(1) << L";" ;
		}
		// ignore directive
		else if ( ! wcsncmp( backslashed_sequence.c_str(), L"uc", 2 ) && iswdigit( backslashed_sequence[2] ) )
		{
			ATLASSERT( backslashed_sequence.substr(2).find_first_not_of( L"1234567890" ) == string::npos ) ;

			m_num_to_ignore = (int)string2long( backslashed_sequence.substr(2) ) ;
		}
		// bold
		else if ( backslashed_sequence == L"b0" )
		{
			m_html += L"</B>" ; // 
			m_bold_on = false ;
		}
		else if ( backslashed_sequence == L"b" )
		{
			m_html += L"<B>" ; // 
			m_bold_on = true ;
		}
		// italic
		else if ( backslashed_sequence == L"i0")
		{
			m_html += L"</I>" ; // 
			m_italic_on = false ;
		}
		else if ( backslashed_sequence == L"i" )
		{
			m_html += L"<I>" ; // 
			m_italic_on = true ;
		}
		// underline
		else if ( backslashed_sequence == L"ul0" )
		{
			m_html += L"</U>" ; // 
			m_underline_on = false ;
		}
		else if ( backslashed_sequence == L"ul" )
		{
			m_html += L"<U>" ; // 
			m_underline_on = true ;
		}
		// escaped character		
		else
		{
			wstring escaped_char ;
			m_line_buffer.get( escaped_char ) ; // get the char
			m_html += escaped_char ;
		}
	}

	return true ;
}

bool tmx_data_importer::close_off_tags( tag_tracker &tags )
{
	while ( tags.empty() == false )
	{
		tags.pop() ;
	}

	return true ;
}

wstring tmx_data_importer::handle_foreground_color_tag( tag_stack &tags )
{
	wstring html_tag = L"<font" ;
	wstring color_num ;
	m_line_buffer.advance() ;
	while ( m_line_buffer.is_digit() )
	{
		TRUE_ENFORCE( m_line_buffer.empty() == false, R2TS( IDS_MSG_UNEXPECTED_EOF) );
		m_line_buffer.get( color_num ) ;
	}

	ATLASSERT ( m_colors.find( string2string(color_num) ) != m_colors.end() ) ;
	if ( m_colors.find( string2string(color_num) ) != m_colors.end() )
	{
		html_tag += L" color=\"" ;
		html_tag += m_colors[string2string(color_num)] ;
		html_tag += wchar_t(L'\"') ;
	}
	else html_tag += L" color=\"black\"" ;

	html_tag += wchar_t(L'>') ;
	tags.push( L"font" ) ;
	return html_tag ;
}

void tmx_data_importer::parse_fonttbl( const wstring fonttbl )
{
	wstring stripped = strip_tags(fonttbl) ;
	string narrow_fonttbl = string2string(stripped) ;
	c_reader reader(narrow_fonttbl.c_str() ) ;

	m_fonts.clear() ;
	m_fonts.parse_fonttbl(reader) ;
}

void trados_data_importer::handle_formatting_stack_top(data_importer< char >::tag_stack& format_tags)
{
	ATLASSERT ( ! format_tags.empty() ) ; 

	// it could be empty if we ignored this formatting tag
	if ( format_tags.top().empty() == true )
	{
		return ;
	}

	m_html += L"</" + format_tags.top() + wchar_t(L'>') ;
	if ( format_tags.top() == L"font" ) // restore codepage
	{
		unsigned int codepage( m_current_codepage ) ;

		if ( m_codepage_stack.empty() == false )
		{
			m_codepage_stack.pop() ;
			if ( m_codepage_stack.empty() == false )
			{
				codepage = m_codepage_stack.top() ;
			}
		}

		if ( -1 == _setmbcp( codepage ) )
		{
			_setmbcp( CP_ACP ) ;
		}
	}
}


void trados_data_importer::close_tags_top(data_importer< char >::tag_tracker& tags)
{
	tag_stack format_tags = tags.top() ;
	while ( false == format_tags.empty() ) 
	{
		handle_formatting_stack_top(format_tags) ;			
		format_tags.pop() ;
	}
	tags.pop() ;

}

wstring trados_data_importer::handle_foreground_color_tag( tag_stack &tags )
{
	wstring html_tag = L"<font" ;
	string color_num ;
	m_line_buffer.advance() ;
	while ( m_line_buffer.is_digit() )
	{
		TRUE_ENFORCE( m_line_buffer.empty() == false, R2TS( IDS_MSG_UNEXPECTED_EOF) );
		m_line_buffer.get( color_num ) ;
	}

	ATLASSERT ( m_colors.find( color_num ) != m_colors.end() ) ;
	if ( m_colors.find( color_num ) != m_colors.end() )
	{
		html_tag += L" color=\"" ;
		html_tag += m_colors[color_num] ;
		html_tag += wchar_t(L'\"') ;
	}
	else html_tag += L" color=\"black\"" ;

	html_tag += wchar_t(L'>') ;
	m_codepage_stack.push( static_cast<unsigned int>( _getmbcp() ) ) ;
	tags.push( L"font" ) ;
	return html_tag ;
}

bool trados_data_importer::load( const CString &location, mem_engine::memory_pointer mem )
{
	ATLASSERT ( m_listener != NULL ) ; 
	ATLASSERT ( file::file::exists(location) ) ; 

	if (!m_listener)
	{
		throw CProgramException(_T("m_listener is NULL"));
	}
	m_listener->OnProgressInit(location, 0, size() ) ;

	ATLASSERT ( m_buffer.get_buffer() == m_buffer.get_current_pos() ) ; 

	m_buffer.rewind() ;
	const string firstline = m_buffer.getline() ;
	if (firstline.find("<TWBExportFile version=") != string::npos)
	{
		m_is_new_type = true ;
	}
	m_buffer.rewind() ;

	if ( ! load_font_table() )
	{
		return false ;
	}

	while ( get_next_record( ) )
	{

		// add it!
		record_pointer rec = get_current_record() ;
		if ( rec->is_valid_record() ) 
		{
			mem->add_record( rec ) ;
		}

		m_listener->OnProgressLoadUpdate(mem->size()) ;

	}

	m_listener->OnProgressDoneLoad(mem->size()) ;
	mem->set_location( location ) ;

	return true ;
}

void trados_data_importer::process_formatted_line_old()
{
	// default values
	m_num_to_ignore = 1 ;		// number of bytes to ignore after Unicode character is read
	m_bold_on = (false) ;
	m_italic_on = (false) ;
	m_underline_on = (false) ;

	// clear out the current html text
	m_html.erase() ;

	// keep a record of tags
	tag_tracker tags ;

	unsigned int codepage( m_current_codepage ) ;

	const UINT old_mbcp = _getmbcp() ;

	while ( m_line_buffer.empty() == false ) // read in the whole rest of the line
	{
		if ( m_codepage_stack.empty() )
		{
			m_codepage_stack.push( m_old_codepage ) ;
		}

		codepage = m_codepage_stack.top() ;

		if ( -1 == _setmbcp( codepage ) )
		{
			if ( -1 == _setmbcp( m_old_codepage ) )
			{
				_setmbcp( old_mbcp ) ;
			}
		}
		const string chunk = m_line_buffer.getline_delims("{}&<\\", false ) ;


		if ( m_codepage_stack.empty() == false ) 
		{
			codepage = m_codepage_stack.top() ;
		}

		if ( chunk.empty() == false )
		{
			const size_t len_needed = chunk.size() + 1 ;
			boost::scoped_array<wchar_t> dest_buff(new wchar_t[len_needed]) ;
			size_t chars_copied = ::MultiByteToWideChar(codepage, 0, chunk.c_str(), chunk.size(), dest_buff.get(), len_needed) ;
			if ( chars_copied == 0 )
			{
				chars_copied = ::MultiByteToWideChar(m_old_codepage, 0, chunk.c_str(), chunk.size(), dest_buff.get(), len_needed) ;
			}
			ATLASSERT(chars_copied+1 <= len_needed) ;
			m_html.append(dest_buff.get(), chars_copied) ;
		}
		switch ( m_line_buffer.peek() )
		{
			// start of rtf-style formatting
		case char('{'):
			{
				handle_open_bracket( tags );
				m_in_bracket = true ;
			}
			break ;
			// end of rtf-style formatting
		case char('}'):
			{
				handle_close_bracket( tags );
				m_in_bracket = false ;
			}
			break ;
			// needs to be in html format
		case char('&'):
			handle_needs_entity_code( m_line_buffer.peek() );
			break ;
		case char('<'):
			handle_needs_entity_code( m_line_buffer.peek() );
			break ;
			// escaped char
		case char('\\'):
			handle_backslash( ) ;
			break ;
		}
	}
	_setmbcp( old_mbcp ) ;
	// We should have just 1 tag left, namely the closing tag (e.g. Seg)
	// However, we will allow tags to not be closed off, and just quietly do so ourselves.
	close_off_tags( tags ) ;

	if ( m_bold_on)
	{
		m_html += L"</B>" ;
	}
	if ( m_italic_on)
	{
		m_html += L"</I>" ;
	}
	if ( m_underline_on)
	{
		m_html += L"</U>" ;
	}

	if ( m_codepage_stack.empty() == false ) 
	{
		codepage = m_codepage_stack.top() ;
	}
}

char trados_data_importer::get_escaped_char( const string num ) const
{
	const int hexadecimal = 16 ;
	const long escaped_char = string2long( num, hexadecimal ) ;
	if ( escaped_char >= 256 && str::is_uint_rep( num ) ) 
	{
		const int decimal = 10 ;
		return static_cast<char>(string2long(num, decimal)) ;
	}
	ATLASSERT( escaped_char < 256 ) ; // it must be in range...
	return static_cast<char>(escaped_char) ;
}
