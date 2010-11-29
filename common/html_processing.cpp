/********************************************************************
	Date:	    2004/11/11
	Time:   	12:11:50
	filename: 	C:\Programming\MyLibrary\html_processing.cpp
	file path:	C:\Programming\MyLibrary
	file base:	html_processing
	file ext:	cpp
	author:		Ryan Ginstrom
	
	purpose:	implementation of the html_formatter class.
*********************************************************************/

// Code walkthrough [2004-11-11]

#include "stdafx.h"
#include "html_processing.h"

// these are the font sizes with the 7 HTML font size settings (1-7)
const static int font_sizes[] = 
{ 8, 10, 12, 14, 18, 24, 36 } ;

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

void html_processing::pop_values( int values )
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
void html_processing::push_font_size( int setting /* = 4 */ ) 
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

int html_processing::font_size_in_points()
{
	int html_font_size = get_font_size() ;

	return html_size_to_point_size( html_font_size ) ;
}

int html_processing::html_size_to_point_size( int html_size )
{
	ATLASSERT( 0 < html_size && html_size <= 7 ) ;
	
	if( 0 < html_size && html_size <= 7 )
		return font_sizes[html_size] ;
	
	return 12 ; // default!
}

int html_processing::point_size_to_html_size( int point_size ) 
{
	int min_diff = abs( ( point_size - font_sizes[0] ) ) ;
	
	int min_index = 0 ;
	int num_fonts = sizeof( font_sizes ) / sizeof( font_sizes[0] ) ;
	for ( int i = 1 ; i < num_fonts ; ++i )
	{
		int this_diff = abs( ( point_size - font_sizes[i] ) ) ;
		if ( this_diff < min_diff )
		{
			min_index = i ;
			min_diff = this_diff ;
		}
	}
	
	return min_index + 1 ;
}

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
