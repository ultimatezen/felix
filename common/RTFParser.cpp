// RTFParser.cpp: implementation of the RTFParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTFParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace rtf
{
	RTFParser::RTFParser()
	{
		
	}
	
	RTFParser::~RTFParser()
	{
		
	}
	
	bool RTFParser::parse_rtf_stream(LPCSTR stream_text)
	{
		m_reader.set_buffer( stream_text ) ;
		
		if ( ! parse_preamble() ) return false ;
		if ( ! parse_font_table() ) return false  ;
		if ( ! parse_color_table() ) return false  ;
		if ( ! parse_body() ) return false  ;

		return true ;
	}
	
	bool RTFParser::parse_font_table( )
	{
		if ( m_reader.find( "{\\fonttbl", true ) == false ) // some Trados files have no font table
		{
			m_reader.rewind() ;

			c_reader::bookmark_type bookmark = m_reader.get_current_pos() ;
			m_reader.set_buffer( 
			// this seems to be the default font table output by Trados.
			"{\\fonttbl "
			"{\\f1 \\fmodern\\fprq1 \\fcharset128 MS UI Gothic;}"
			"{\\f2 \\fswiss\\fprq2 \\fcharset0 Arial;}"
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

			bool retval = parse_font_table() ;
			m_reader.set_buffer( bookmark ) ;
			return retval ;
		}

		m_reader.eat_whitespace() ;

		while ( m_reader.current_is( '{' ) )
		{
			// eat the {
			m_reader.advance() ;

			font_table_entry entry ;	// the entry
			my_font font ;				// the font

			// we know that the first tag is font num
			string code ;
			m_reader.getline( code, char(' '), true ) ;
		
			entry.m_code = string2tstring( code, CP_ACP ) ;

			while ( ! m_reader.current_is( '}' ) )
			{
 				m_reader.jump_to_first_of( char('\\'), true ) ;
				string tag ;
				m_reader.getline( tag, " \\{", false ) ;

				// check for font family
				if ( tag == "fdecorative" || tag == "fdecor" )
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
				else if ( str::left(tag, 8 ) == "fcharset" || str::left(tag,7) == "charset" )
				{
					USES_CONVERSION ;

					if ( str::left(tag, 8 ) == "fcharset" )
					{
						font.set_charset( static_cast< unsigned char >(string2long( tag.substr(8) ) ) ) ;
					}
					else
					{
						font.set_charset( static_cast< unsigned char >(string2long( tag.substr(7) ) ) ) ;
					}
					m_reader.advance() ;
					m_reader.getline( tag, char(';'), true ) ;
					font.set_name( A2CT( unescape_string( tag ).c_str() ) ) ;
				}

				if( m_reader.current_is( '{' ) )
				{
					m_reader.jump_to_first_of( char('}'), true ) ; 
				}
			}
			font.lookup_font() ;
			entry = font ;
			m_fonts.add_font( entry ) ;
			font.init_font_info() ;

			// eat the }
			ATLASSERT( m_reader.current_is( '}' ) ) ;
			m_reader.advance() ; 
			// advance to next brace
			m_reader.eat_whitespace() ;

		}

		return true ;
	}
	
	bool RTFParser::parse_color_table( )
	{
		if ( m_reader.find( "{\\colortbl;", true ) == false ) // some Trados files have no font table
		{
			m_reader.rewind() ;

			c_reader::bookmark_type bookmark = m_reader.get_current_pos() ;
			m_reader.set_buffer( 
			// this seems to be the default font table output by Trados.
			"{\\colortbl ;" 													
			"\\red0\\green0\\blue0;"  			// BLACK		
			"\\red0\\green0\\blue255;"	  		// BLUE		
			"\\red0\\green255\\blue255;"  		// CYAN		
			"\\red0\\green255\\blue0;"	  		// GREEN		
			"\\red255\\green0\\blue255;"  		// MAGENTA		
			"\\red255\\green0\\blue0;"	  		// RED			
			"\\red255\\green255\\blue0;"  		// YELLOW		
			"\\red255\\green255\\blue255;"	  	// WHITE		
			"\\red0\\green0\\blue128;"	  		// DK_BLUE		
			"\\red0\\green128\\blue128;"  		// DK_CYAN	
			"\\red0\\green128\\blue0;"	  		// DK_GREEN	
			"\\red128\\green0\\blue128;"  		// DK_MAGENTA 
			"\\red128\\green0\\blue0;"  			// DK_RED		
			"\\red128\\green128\\blue0;"  		// DK_YELLOW  
			"\\red128\\green128\\blue128;"  		// DK_GRAY	
			"\\red192\\green192\\blue192;"  		// LT_GRAY		
			"}" 
			) ;

			bool retval = parse_color_table() ;
			m_reader.set_buffer( bookmark ) ;
			return retval ;
		}

		m_reader.eat_whitespace() ;

		while ( ! m_reader.current_is( '}' ) )
		{
			string color_string ;
			m_reader.getline( color_string, ";}", false ) ;

			COLORREF color = parse_color_table_entry( color_string ) ;

			m_colors.push_back( color ) ;

			m_reader.eat_if( ';' ) ;
			m_reader.eat_whitespace() ;
		}

		return true ;
	}
	
	bool RTFParser::parse_preamble()
	{
		m_reader.jump_to_first_of( '{', true ) ;

		while ( m_reader.peek() != '}' ) 
		{
			m_reader.eat_if( '\\' ) ;
			
			string element_text ;
			m_reader.getline( element_text, "\\}", false ) ;
			m_preamble.parse_element( element_text ) ;
		}

		ATLASSERT( m_reader.current_is( '}' ) ) ;
		m_reader.eat_if( '}' ) ;

		return true ;
	}

	bool RTFParser::parse_body()
	{

		return true ;
	}

	COLORREF RTFParser::parse_color_table_entry(const string &entry)
	{
		c_reader reader ;
		reader.set_buffer( entry.c_str() ) ;

		reader.eat_if( '\\' ) ;
		string red, green, blue ;
		reader.getline( red, '\\' ) ;
		reader.getline( green, '\\' ) ;
		reader.getline( blue, '\\' ) ;

		int red_val = string2long( red.substr( 3 ) ) ;
		int green_val = string2long( green.substr( 5 ) );
		int blue_val = string2long( blue.substr( 4 ) ) ;
		
		return RGB( red_val, green_val, blue_val ) ;
	}
}
