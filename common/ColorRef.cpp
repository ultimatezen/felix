/*!
	@file ColorRef.cpp
	@brief Wrapper for COLORREF data type
	@date 2005/06/07
	Time: 2:13:07
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#include "stdafx.h"
#include "ColorRef.h"
#include "StringConversions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorRef::rgb_cracker::rgb_cracker( COLORREF val ) :
	rgb_val(val)
	{}
CColorRef::rgb_cracker::rgb_cracker( ) :
	rgb_val( RGB(0,0,0) )
	{}


const wstring CColorRef::as_wstring( ) const
{
// #define GetRValue(rgb)      ((BYTE)(rgb))
// #define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
// #define GetBValue(rgb)      ((BYTE)((rgb)>>16))

	// turn it into an HTML tag
	const wstring color = L"#" +
	// R
			byte_2_wstring( m_color.rgb_bytes[BYTE_POS_RED] ) +
	// G
			byte_2_wstring( m_color.rgb_bytes[BYTE_POS_GREEN] ) +
	// B
			byte_2_wstring( m_color.rgb_bytes[BYTE_POS_BLUE] ) ;
	
	ATLASSERT( color[0] == L'#' ) ;
	ATLASSERT( color.length() == 7 ) ;
	
	return color ;
	
}

void CColorRef::invert()
{
	set_red_value( 0xFF - get_red_value() ) ;
	set_green_value( 0xFF - get_green_value() ) ;
	set_blue_value( 0xFF - get_blue_value() ) ;
}


const string CColorRef::as_string( ) const
{
	return string2string(as_wstring()) ;
}

const wstring CColorRef::byte_2_wstring( const BYTE val ) const
{
	// turn it into an HTML tag
	wstring color ;
	// R
	const wstring chunk = int2wstring( val, 16 ) ;
	if ( chunk.length() == 1 ) 
	{
		color += L'0' ;
	}
	color += chunk ;

	return color ;
}


CColorRef::CColorRef()
{

}

CColorRef::CColorRef(COLORREF color) : 
	m_color( color )
{

}

CColorRef::CColorRef( BYTE red_byte, BYTE green_byte, BYTE blue_byte ) : 
	m_color( RGB( red_byte, green_byte, blue_byte ) )
{

}

CColorRef::CColorRef( const wstring &color )
{
	from_string( color ) ;
}


CColorRef& CColorRef::operator =(const COLORREF rhs)
{
	m_color.rgb_val = rhs ;
	return *this ;
}


BYTE CColorRef::get_red_value() const
{
	return m_color.rgb_bytes[BYTE_POS_RED] ;
}
BYTE CColorRef::get_green_value() const
{
	return m_color.rgb_bytes[BYTE_POS_GREEN] ;
}
BYTE CColorRef::get_blue_value() const
{
	return m_color.rgb_bytes[BYTE_POS_BLUE] ;
}

void CColorRef::set_red_value(BYTE val)
{
	m_color.rgb_bytes[BYTE_POS_RED] = val ;
}
void CColorRef::set_green_value(BYTE val)
{
	m_color.rgb_bytes[BYTE_POS_GREEN] = val ;
}
void CColorRef::set_blue_value(BYTE val)
{
	m_color.rgb_bytes[BYTE_POS_BLUE] = val ;
}


bool CColorRef::is_black() const 
{
	if( get_red_value() > 0u ) // 32-bit RGB value
	{
		return false ;  
	}
	if( get_green_value() > 0u ) // 32-bit RGB value
	{
		return false ;  
	}
	if( get_blue_value() > 0u ) // 32-bit RGB value
	{
		return false ;  
	}

	return true ;
}
bool CColorRef::is_white() const 
{
	if( get_red_value() < 0xff ) // 32-bit RGB value
	{
		return false ;  
	}
	if( get_green_value() < 0xff ) // 32-bit RGB value
	{
		return false ;  
	}
	if( get_blue_value() < 0xff ) // 32-bit RGB value
	{
		return false ;  
	}

	return true ;
}

// private

void CColorRef::from_string(const wstring &color)
{
	if ( color.empty() == true )
	{
		m_color.rgb_val = RGB( 0, 0, 0 ) ;
		return ;
	}

	size_t start_pos( 0u ) ;
	if ( color[0] == L'#' ) 
	{
		ATLASSERT( color.length() == 7 ) ;
		start_pos = 1u ;
	}

	// red
	wstring red_chunk = color.substr( start_pos, 2 ) ;
	long red = string2long( red_chunk, 16 ) ;

	start_pos += 2u ;

	// green
	wstring green_chunk = color.substr( start_pos, 2 ) ;
	long green = string2long( green_chunk, 16 ) ;

	start_pos += 2u ;

	// blue
	wstring blue_chunk = color.substr( start_pos, 2 ) ;
	long blue = string2long( blue_chunk, 16 ) ;

	// create COLORREF
	m_color = RGB( red, green, blue ) ;
}

COLORREF CColorRef::as_colorref() const
{
	return static_cast< COLORREF >( m_color.rgb_val ) ;
}

const bool CColorRef::operator==( const COLORREF rhs ) const
{
	return ( as_colorref() == rhs ) ;
}


