// font_properties.cpp: implementation of the font_properties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "font_properties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

font_properties::font_properties() :
	m_bold_established( false ),
	m_italic_established( false ),
	m_underline_established( false ),
	m_font_color_established( false ),
	m_font_face_established( false ),
	m_font_size_established( false ),

	m_subscript_established(false),
	m_superscript_established(false),

	m_bold( false ),
	m_italic( false ),
	m_underline( false ),
	m_font_size(-1),

	m_subscript(false),
	m_superscript(false)
{

}
font_properties::font_properties( const font_properties &rhs ) :
	m_bold_established			( rhs.m_bold_established ),
	m_italic_established			( rhs.m_italic_established ),
	m_underline_established		( rhs.m_underline_established ),
	m_font_color_established	( rhs.m_font_color_established ),
	m_font_face_established		( rhs.m_font_face_established ),
	m_font_size_established		( rhs.m_font_size_established ),

	m_subscript_established		( rhs.m_subscript_established ),
	m_superscript_established	( rhs.m_superscript_established ),

	m_bold			( rhs.m_bold ), 
	m_italic			( rhs.m_italic ),
	m_underline		( rhs.m_underline ),
	m_font_color	( rhs.m_font_color ),
	m_font_face		( rhs.m_font_face ),
	m_font_size		( rhs.m_font_size ),

	m_subscript		( rhs.m_subscript ),
	m_superscript	( rhs.m_superscript )
{
}

font_properties::~font_properties()
{

}

font_properties& font_properties::operator =( const font_properties &rhs )
{
	m_bold_established			= rhs.m_bold_established ;
	m_italic_established			= rhs.m_italic_established ;
	m_underline_established		= rhs.m_underline_established ;
	m_font_color_established	= rhs.m_font_color_established ;
	m_font_face_established		= rhs.m_font_face_established ;
	m_font_size_established		= rhs.m_font_size_established ;

	m_subscript_established		= rhs.m_subscript_established ;
	m_superscript_established	= rhs.m_superscript_established ;

	m_bold			= rhs.m_bold ; 
	m_italic			= rhs.m_italic ;
	m_underline		= rhs.m_underline ;
	m_font_color	= rhs.m_font_color ;
	m_font_face		= rhs.m_font_face ;
	m_font_size		= rhs.m_font_size ;

	m_subscript		= rhs.m_subscript ;
	m_superscript	= rhs.m_superscript ;
	return *this ;
}


// set
void font_properties::set_superscript( bool setting )
{
	m_superscript_established = true ;
	
	m_superscript = setting ;
	
}
void font_properties::set_subscript( bool setting )
{
	m_subscript_established = true ;
	
	m_subscript = setting ;
	
}

void font_properties::set_bold( bool setting )
{
	m_bold_established = true ;

	m_bold = setting ;
		
}

void font_properties::set_italic( bool setting ) 
{
	m_italic_established = true ;

	m_italic = setting ;
	
}

void font_properties::set_underline( bool setting ) 
{
	m_underline_established = true ;
	
	m_underline = setting ;
	
}

void font_properties::set_font_color( CColorRef &setting ) 
{
	m_font_color_established = true ;

	m_font_color = setting ;
}

void font_properties::set_font_face( const wstring &setting ) 
{
	m_font_face_established = true ;

	m_font_face = setting ;

}

void font_properties::set_font_size( int size ) 
{
	m_font_size_established = true ;

	m_font_size = size ;
}

// get

const bool font_properties::get_bold( ) const 
{
	return m_bold ;
}

const bool font_properties::get_italic( ) const 
{
	return m_italic ;
}

const bool font_properties::get_underline( ) const 
{
	return m_underline ;
}

const COLORREF font_properties::get_font_color( ) const 
{
	return m_font_color.as_colorref() ;
}

const wstring font_properties::get_font_face( ) const 
{
	return m_font_face ;
}

const int font_properties::get_font_size( ) const 
{
	return m_font_size ;
}

// is_established

bool font_properties::is_bold_established( ) 
{
	return	m_bold_established ;
}

bool font_properties::is_italic_established( ) 
{
	return	m_italic_established ;
}

bool font_properties::is_underline_established( ) 
{
	return	m_underline_established ;
}

bool font_properties::is_font_color_established( ) 
{
	return	m_font_color_established ;
}

bool font_properties::is_font_face_established( ) 
{
	return	m_font_face_established ;
}

bool font_properties::is_font_size_established( ) 
{
	return	m_font_size_established ;
}

