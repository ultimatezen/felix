/*!
	@file data_importer.h
	@brief Interface file for data importer classes
	@date 2005/06/25
	Time: 14:40:54
	@author Ryan Ginstrom
 */

#pragma once

#include "File.h"
#include "background_processor.h"	// AKA poor-man's multithreading
#include "rtf.h"					// for font table
#include "record.h"					// translation_record
#include "data_converter.h"			// converting trados to internal format
#include "charset_info.h"			// get code page from charset/lang id
#include "record_local.h"

#include "TranslationMemory.h"		// trans_set

#include "tag_stripper.h"

#include "ProgressListener.h"


/**
	@class data_importer
	@brief Common stuff for importing Trados txt and TMX files.
 */
template< class CHAR_T >
class data_importer
{
public:
	typedef textstream_reader< CHAR_T >			reader_type ;
	typedef std::basic_string< CHAR_T >			string_type ;
	typedef std::stack< wstring >				tag_stack ;
	typedef std::stack< tag_stack >				tag_tracker ;
	typedef std::map< string, wstring >			color_map ;
	typedef std::set< tstring >					language_code_set ;
	typedef std::vector< string_type >			language_list ;
protected:
	reader_type				m_line_buffer ;
	rtf::font_table		m_fonts ;

	color_map				m_colors ;

	bool m_underline_on;
	bool m_italic_on;
	bool m_bold_on;
	int m_num_to_ignore;

	wstring					m_html ;

private:
public:
	/**
	*	PURE VIRTUAL FUNCTIONS
	*/

	virtual bool handle_backslash() = 0 ;
	virtual bool close_off_tags(tag_tracker &tags) = 0 ;
	virtual wstring handle_f_tag(tag_stack &tags) = 0 ;
	virtual wstring handle_foreground_color_tag(tag_stack &tags) = 0 ;

	/**
	 *	Common implementation
	 */
	
	data_importer() :
		m_num_to_ignore(1),
		m_bold_on(false), 
		m_italic_on(false), 
		m_underline_on(false)
	{
		m_colors["0"] = L"#000000" ;
		m_colors["1"] = L"#000000" ;
		m_colors["2"] = L"#0000ff" ;
		m_colors["3"] = L"#00ffff" ;
		m_colors["4"] = L"#008000" ;
		m_colors["5"] = L"#ff00ff" ;
		m_colors["6"] = L"#ff0000" ;
		m_colors["7"] = L"#ffff00" ;
		m_colors["8"] = L"#ffffff" ;
		m_colors["9"] = L"#00008b" ;
		m_colors["10"] = L"#008b8b" ;
		m_colors["11"] = L"#006400" ;
		m_colors["12"] = L"#8b008b" ;
		m_colors["13"] = L"#8b0000" ;
		m_colors["14"] = L"#8b8b00" ;
		m_colors["15"] = L"#a9a9a9" ;
		m_colors["16"] = L"#d3d3d3" ;

	}
	virtual ~data_importer()
	{}

	bool handle_open_bracket(tag_tracker &tags)
	{
		ATLASSERT( m_line_buffer.current_is( CHAR_T('{') ) ) ;

		tag_stack format_tags ; // we might have more than one tag,
		// which all need to be popped off when we
		// find the }

		m_line_buffer.advance() ;

		// read in the tags for this bracket
		m_html += read_format_tag( format_tags ) ;
		// push them onto our tag stack
		tags.push( format_tags ) ;

		// eat the space
		m_line_buffer.eat_whitespace() ;

		return true ;
	}

	bool handle_needs_entity_code( const CHAR_T c )
	{
		ATLASSERT( c == CHAR_T('&') || c == CHAR_T('<') ) ; // these are the only ones we escape right now


		m_html += L"&#" ;
		m_html += int2wstring( static_cast<int>( c ), 10 ) ;
		m_html += L";" ;
		m_line_buffer.advance() ;

		return true ;

	}


	wstring read_format_tag(tag_stack &tags)
	{
		ATLASSERT( m_line_buffer.current_is( '\\' ) );
		m_line_buffer.advance() ;

		wstring html_tag ;

		switch ( m_line_buffer.peek() )
		{
		case char('c'):
			html_tag += handle_c_tag( tags ) ;
			break ;
		case char('h'):	
			html_tag += handle_background_color_tag( tags ) ;
			break ;
		case char('f'):
			html_tag += handle_f_tag( tags ) ;
			break ;
		case 's': // super or sub?
			html_tag += handle_s_tag( tags ) ;
			break ;
		case '*': // skip it, but watch out for the fldinst jobby
			m_line_buffer.advance() ;
			break ;
		default:
			{
				string_type tag ;

				CHAR_T delims[] = { CHAR_T(' '), CHAR_T('\\'), CHAR_T('}') } ;

				m_line_buffer.getline( tag, delims, false ) ;

				if ( string2string(tag) == "ul" ) // underline
				{
					html_tag += L"<u>" ;
					tags.push( L"u" ) ;
				}
				else
				{
					html_tag << L"<" << string2wstring( tag ) << L">" ;
					tags.push( string2wstring( tag ) ) ;
				}

				break ;
			}
		}

		if ( m_line_buffer.current_is( CHAR_T('\\') ) )
		{
			html_tag += read_format_tag( tags ) ;
		}
		return html_tag ;
	}

	wstring handle_background_color_tag(tag_stack &tags)
	{
		while ( ! m_line_buffer.is_digit() )
		{
			TRUE_ENFORCE( m_line_buffer.empty() == false, R2TS( IDS_MSG_UNEXPECTED_EOF) ) ;

			if ( m_line_buffer.is_space() ) // some other kind of tag...
			{
				// Add an empty string.
				// Then when we get to a } and 
				// go to pop it off the stack, if it's empty we will know there is
				// nothing to closs off because we ingored this tag
				tags.push( wstring() ) ;
				return wstring() ;
			}
			m_line_buffer.advance() ;
		}
		string_type color_num ;
		while ( m_line_buffer.is_digit() )
		{
			TRUE_ENFORCE( m_line_buffer.empty() == false, R2TS( IDS_MSG_UNEXPECTED_EOF) ) ;
			m_line_buffer.get( color_num ) ;
		}
		if ( m_colors.find( string2string( color_num ) ) != m_colors.end() )
		{
			wstring html_tag = L"<span style=\"background:" ;
			html_tag += m_colors[string2string( color_num )] ;
			html_tag += L"\">" ;
			tags.push(L"span") ;
			return html_tag ;
		}

		tags.push( wstring() ) ;
		return wstring() ;
	}


	wstring handle_c_tag(tag_stack &tags)
	{
		CHAR_T delims[] = { CHAR_T(' '), CHAR_T('\\'), CHAR_T('}') } ;

		m_line_buffer.advance() ;
		switch ( m_line_buffer.peek() )
		{
		case CHAR_T('f'):
			return handle_foreground_color_tag( tags ) ;

		case CHAR_T('s'): // skip
			{
				m_line_buffer.jump_to_first_of( delims, false ) ;

				TRUE_ENFORCE( m_line_buffer.empty() == false, R2TS( IDS_MSG_UNEXPECTED_EOF) ) ;
				break ;
			}
		default: // skip any other stuff...
			{
				m_line_buffer.jump_to_first_of( delims, false ) ;

				TRUE_ENFORCE( ! m_line_buffer.empty(), R2TS( IDS_MSG_UNEXPECTED_EOF ) ) ;
				break ;
			}
		}
		tags.push( wstring( ) ) ;	
		return wstring( ) ;
	}


	wstring handle_s_tag(tag_stack &tags)
	{
		BANNER( "trados_data_importer::handle_s_tag" ) ;

		string_type tag ;

		CHAR_T delims[] = { CHAR_T(' '), CHAR_T('\\'), CHAR_T('}') } ;

		m_line_buffer.getline( tag, delims, false ) ; // eat the char after

		if ( m_line_buffer.is_space() ) 
		{
			m_line_buffer.advance() ;
		}

		TRUE_ENFORCE( ! m_line_buffer.empty(), R2TS( IDS_MSG_UNEXPECTED_EOF) ) ;

		if ( string2string( tag ) == "super" ) 
		{
			tags.push( L"sup" ) ;
			return L"<sup>" ;
		}
		if ( string2string( tag ) == "sub" )
		{
			tags.push( L"sub" ) ;
			return L"<sub>" ;
		}

		// we don't understand it, so we ignore it!
		tags.push( wstring() ) ;
		return wstring() ;
	}



};

/**
 *	The trados_data_importer class. Imports trados ".TXT" format memories into a Felix memory.
 */
class trados_data_importer : public data_importer< char >
{

public:
	const string_type tru_start_tag	;
	const string_type tru_end_tag ;
	const string_type tab_str ;
	const string_type line_str ;

	reader_type				m_record_buffer ;

	reader_type				m_buffer ;
	string					m_file_name ;
	file::view				m_view ;
	language_code_set		m_languages ;
	background_processor	m_background ;

	string					m_source_language ;
	string					m_target_language ;
	mem_engine::record_pointer	m_current_record ;

	UINT					m_old_codepage ;
	UINT					m_default_codepage ;
	UINT					m_current_codepage ;

	std::stack< unsigned int >	m_codepage_stack ;
	size_t						m_file_size ;
	size_t						m_file_pos ;
	CProgressListener			*m_listener ;

public:
	bool	m_is_new_type ;
	bool	m_in_bracket ;

	trados_data_importer( CProgressListener *listener) :
	m_listener( listener ), 
		m_file_size( 0 ), 
		m_file_pos( 0 ) ,
		m_is_new_type(false),
		m_in_bracket(false),
		tru_start_tag("<TrU>"),
		tru_end_tag("</TrU>"),
		tab_str("tab"),
		line_str("line"),
		m_old_codepage(_getmbcp()),
		m_default_codepage(0),
		m_current_codepage(0)
	{
	}

	~trados_data_importer()
	{
		// set the old code page
		_setmbcp( get_old_codepage() ) ;
	}

	bool load( const CString &location, mem_engine::memory_pointer mem );

	string get_tru() ;

	void clear_record()
	{
		m_current_record = mem_engine::record_pointer(new mem_engine::record_local()) ;
	}
	mem_engine::record_pointer get_current_record()
	{
		return m_current_record ;
	}
	UINT get_old_codepage() const
	{
		return m_old_codepage ;
	}
	bool close_off_tags(tag_tracker &tags) ;

	wstring handle_foreground_color_tag(tag_stack &tags);

	string read_backslashed_sequence();
	bool get_language_code( language_code_set &languages );
	void rewind();
	size_t get_pos();
	size_t file_size();
	bool process_line( const string &line );

	void process_formatted_line_old();
	void set_source_language( const tstring &source_language ) { m_source_language = string2string( source_language ) ; }
	void set_target_language( const tstring &target_language ) { m_target_language = string2string( target_language ) ; }
	string dump_context( );
	//void set_codepage( string &lang_tag );
	wstring get_font_tag( const string_type &code );
	bool next_tag_is_end_tag();
	bool load_font_table();
	size_t get_language_codes( language_code_set &languages ) ;
	//wstring handle_background_color_tag( tag_stack &tags );
	//wstring handle_foreground_color_tag( tag_stack &tags );
	//wstring read_format_tag( tag_stack &tags );
	//wstring handle_s_tag( tag_stack &tags );
	//wstring handle_c_tag( tag_stack &tags );

	wstring read_the_tag(  );
	bool get_next_record( )  ;
	size_t size();
	bool open_data_source( const CString &source_name );
	bool get_all_records( mem_engine::trans_set &records )   ;
	// implemenations of pure virtual functions in parent
	wstring handle_f_tag( tag_stack &tags ) ;


	bool handle_backslash() ;

	char get_escaped_char( string num ) const;
private:
	bool handle_close_bracket( tag_tracker &tags );


	void handle_formatting_stack_top(data_importer< char >::tag_stack& format_tags);


	void close_tags_top(data_importer< char >::tag_tracker& tags);
} ;

/**
	@class tmx_data_importer 
	@brief Import TMX data.
 */
class tmx_data_importer 
	: public data_importer< wchar_t >
{
public:
	tmx_data_importer()
	{
		c_reader dummy_reader ;
		m_fonts.parse_fonttbl(dummy_reader) ;
	}
	void parse_fonttbl( const wstring fonttbl );
	wstring seg2html( const wstring seg_text ) ;


	bool close_off_tags(tag_tracker &tags);
	wstring handle_foreground_color_tag(tag_stack &tags);


	wstring handle_f_tag(tag_stack &tags ) ;
	bool handle_backslash() ;


protected:
private:
	bool handle_close_bracket( tag_tracker &tags );
	wstring get_font_tag(const wstring code) ;
};