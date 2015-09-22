/********************************************************************
	Date:	    2004/11/11
	Time:   	12:11:50
	author:		Ryan Ginstrom
	
	purpose:	implementation of the html_formatter class.
*********************************************************************/

// Code walkthrough [2004-11-11]

#include "stdafx.h"
#include "html_processing.h"

// these are the font sizes with the 7 HTML font size settings (1-7)
const static unsigned int font_sizes[] = 
{ 8u, 10u, 12u, 14u, 18u, 24u, 36u } ;



unsigned int html_size_to_point_size( unsigned int html_size )
{
	html_size = clamp_html_size(html_size);

	return font_sizes[html_size-1] ;
}

unsigned int clamp_html_size( unsigned int html_size )
{
	if (html_size == 0)
	{
		return 1 ;
	}
	if (html_size > get_num_fonts())
	{
		return get_num_fonts() ;
	}	
	return html_size;
}

unsigned int get_num_fonts()
{
	return sizeof( font_sizes ) / sizeof( font_sizes[0] );
}

unsigned int point_size_to_html_size( unsigned int point_size ) 
{
	unsigned int min_diff = get_font_point_diff(point_size, 0) ;

	unsigned int min_index = 0 ;
	unsigned int num_fonts = get_num_fonts() ;
	for ( size_t i = 1 ; i < num_fonts ; ++i )
	{
		unsigned int this_diff = get_font_point_diff(point_size, i) ;
		if ( this_diff < min_diff )
		{
			min_index = i ;
			min_diff = this_diff ;
		}
	}

	return min_index + 1 ;
}

unsigned int get_font_point_diff( unsigned int point_size, size_t i )
{
	return abs( ( static_cast<int>(point_size) - static_cast<int>(font_sizes[i]) ) );
}

// =====================
// state initialization
// =====================

void html_processing::init_justification( JUSTIFICATION setting /* = JUST_LEFT */ )
{
	while ( m_justification.empty() == false )
	{
		m_justification.pop() ;
	}
	
	m_justification.push( setting ) ;
}

void html_processing::init_bold( bool setting /* = false */ )
{
	m_is_bold_specified = false ;

	while ( m_bold.empty() == false )
	{
		m_bold.pop() ;
	}
	
	m_bold.push( setting ) ;
}

void html_processing::init_italic( bool setting /* = false */ ) 
{
	m_is_italic_specified = false ;

	while ( m_italic.empty() == false )
	{
		m_italic.pop() ;
	}
	
	m_italic.push( setting ) ;
}

void html_processing::init_underline( bool setting /* = false */ ) 
{
	m_is_underline_specified = false ;

	while ( m_underline.empty() == false )
	{
		m_underline.pop() ;
	}
	
	m_underline.push( setting ) ;
}

void html_processing::init_fore_color( CColorRef setting /* = CColorRef() */ ) 
{
	m_is_fore_color_specified = false ;

	while ( m_fore_color.empty() == false )
	{
		m_fore_color.pop() ;
	}
	
	m_fore_color.push( setting.as_colorref() ) ;
}

void html_processing::init_back_color( CColorRef setting /* = CColorRef() */ ) 
{
	while ( m_back_color.empty() == false )
	{
		m_back_color.pop() ;
	}
	
	m_back_color.push( setting.as_colorref() ) ;
}

void html_processing::init_font_face( const wstring &setting /* = L"Times New Roman" */ ) 
{
	while ( m_font_face.empty() == false )
	{
		m_font_face.pop() ;
	}
	
	m_font_face.push( setting ) ;
}

void html_processing::init_font_size( int setting /* = 4 */ ) 
{
	m_is_font_size_specified = false ;

	while ( m_font_size.empty() == false )
	{
		m_font_size.pop() ;
	}
	
	m_font_size.push( setting ) ;
}

void html_processing::init_v_alignment( const VERT_ALIGNMENT valign /* = VA_BASELINE */ )
{
	while ( m_subscript.empty() == false )
	{
		m_subscript.pop() ;
	}
	m_subscript.push( valign == VA_SUBSCRIPT ) ;
	
	while ( m_superscript.empty() == false )
	{
		m_superscript.pop() ;
	}
	m_superscript.push( valign == VA_SUPERSCRIPT ) ;
}

// =====================
// pop states
// =====================

void html_processing::restore_original_states() 
{
	while ( m_bold.size() > 1 )
	{
		pop_bold() ;
	}

	while ( m_italic.size() > 1 )
	{
		pop_italic() ;
	}

	while ( m_underline.size() > 1 )
	{
		pop_underline() ;
	}

	while ( m_fore_color.size() > 1 )
	{
		pop_fore_color() ;
	}

	while ( m_back_color.size() > 1 )
	{
		pop_back_color() ;
	}

	while ( m_font_face.size() > 1 )
	{
		pop_font_face() ;
	}

	while ( m_font_size.size() > 1 )
	{
		pop_font_size() ;
	}

	while ( m_justification.size() > 1 )
	{
		pop_justification() ;
	}
	
	while ( m_subscript.size() > 1 )
	{
		pop_subscript() ;
	}

	while ( m_superscript.size() > 1 )
	{
		pop_superscript() ;
	}
}

void html_processing::pop_values( unsigned int values )
{
	if( values & POP_FACE ) 
		pop_font_face() ;
	if( values & POP_SIZE )
		pop_font_size() ;
	if( values & POP_FORE_COLOR ) 
		pop_fore_color() ;
	if( values & POP_BACK_COLOR ) 
		pop_back_color() ;
	if( values & POP_BOLD ) 
		pop_bold() ;
	if( values & POP_ITALIC )
		pop_italic() ;
	if( values & POP_JUSTIFY )
		pop_justification() ;
}

void html_processing::pop_justification( )
{
	if ( m_justification.empty() == false )
	{
		m_justification.pop( ) ;
	}
}
void html_processing::pop_bold( )
{
	if ( m_bold.empty() == false )
	{
		m_bold.pop( ) ;
	}
}
void html_processing::pop_italic( ) 
{
	if ( m_italic.empty() == false )
	{
		m_italic.pop( ) ;
	}
}
void html_processing::pop_underline( ) 
{
	if ( m_underline.empty() == false )
	{
		m_underline.pop( ) ;
	}
}
void html_processing::pop_fore_color( ) 
{
	if ( m_fore_color.empty() == false )
	{
		m_fore_color.pop( ) ;
	}
}
void html_processing::pop_back_color( ) 
{
	if ( m_back_color.empty() == false )
	{
		m_back_color.pop( ) ;
	}
}
void html_processing::pop_font_face( ) 
{
	if ( m_font_face.empty() == false )
	{
		m_font_face.pop( ) ;
	}
}
void html_processing::pop_font_size( ) 
{
	if ( m_font_size.empty() == false )
	{
		m_font_size.pop( ) ;
	}
}
void html_processing::pop_subscript()
{
	if ( m_subscript.empty() == false )
	{
		m_subscript.pop() ;
	}
}
void html_processing::pop_superscript()
{
	if ( m_superscript.empty() == false )
	{
		m_superscript.pop() ;
	}
}

// =====================
// push states
// =====================

void html_processing::push_defaults() 
{
	push_bold() ;
	push_italic() ;
	push_underline() ;
	push_fore_color() ;
	push_back_color() ;
	push_font_face() ;
	push_font_size() ;
	push_justification() ;
}

void html_processing::push_justification( html_processing::JUSTIFICATION setting /* = JUST_NONE */ )
{
	m_justification.push( setting ) ;
}
void html_processing::push_bold( bool setting /* = false */ )
{
	m_bold.push( setting ) ;
}
void html_processing::push_italic( bool setting /* = false */ ) 
{
	m_italic.push( setting ) ;
}
void html_processing::push_underline( bool setting /* = false */ ) 
{
	m_underline.push( setting ) ;
}
void html_processing::push_fore_color( CColorRef setting /* = CColorRef() */ ) 
{
	m_fore_color.push( setting.as_colorref() ) ;
}
void html_processing::push_back_color( CColorRef setting /* = CColorRef() */ ) 
{
	m_back_color.push( setting.as_colorref() ) ;
}
void html_processing::push_font_face( const wstring &setting /* = L"MS UI Gothic" */ ) 
{
	m_font_face.push( setting ) ;
}
void html_processing::push_font_size( unsigned int setting /* = 4 */ ) 
{
	m_font_size.push( setting ) ;
}
void html_processing::push_font_size( const wstring &setting ) 
{
	push_font_size( string2long( setting ) ) ;
}
void html_processing::push_subscript( bool setting /* = false */ )
{
	m_subscript.push( setting ) ;
}
void html_processing::push_superscript( bool setting /* = false */ )
{
	m_superscript.push( setting ) ;
}


// =====================
// public interface stuff
// =====================





bool html_processing::is_fore_color_specified()
{
	return m_is_fore_color_specified ;
}

bool html_processing::is_font_size_specified()
{
	return m_is_font_size_specified ;
}

bool html_processing::is_bold_specified()
{
	return m_is_bold_specified ;
}

bool html_processing::is_italic_specified()
{
	return m_is_italic_specified ;
}

bool html_processing::is_underline_specified()
{
	return m_is_underline_specified ;
}

void html_processing::init_state()
{
	init_bold() ;
	init_italic() ;
	init_underline() ;
	init_fore_color() ;
	init_back_color() ;
	init_font_face() ;
	init_font_size() ;
	init_justification() ;
	init_v_alignment() ;
}

unsigned int html_processing::get_font_size()
{
	ATLASSERT( m_font_size.empty() == false ) ;
	if (m_font_size.empty())
	{
		return font_sizes[DEFAULT_HTML_FONT_SIZE] ;
	}
	return m_font_size.top() ;
}

unsigned int html_processing::font_size_in_points()
{
	unsigned int html_font_size = get_font_size() ;

	return html_size_to_point_size( html_font_size ) ;
}
