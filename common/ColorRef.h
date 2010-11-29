/*!
	@brief interface for the CColorRef class.
	@author Ryan Ginstrom
 */

#pragma once

const COLORREF COLOR_WHITE = RGB(0xFF, 0xFF, 0xFF) ;
const COLORREF COLOR_BLACK = RGB(0x0, 0x0, 0x0) ;
const COLORREF COLOR_RED = RGB(0xFF, 0, 0);
const COLORREF COLOR_OFFWHITE = RGB(0xFF, 0xFB, 0xF0);
const COLORREF COLOR_DARK_GREEN	= RGB(0, 0x80, 0);
const COLORREF COLOR_DARK_BLUE = RGB(0, 0, 0x80);
const COLORREF COLOR_VIOLET	= RGB(0x80, 0, 0x80);
const COLORREF COLOR_TURQUOISE = RGB(0, 0x80, 0x80);
const COLORREF COLOR_LIGHT_BLUE	= RGB(0xA6, 0xCA, 0xF0);
const COLORREF COLOR_GOLD = RGB(0xFF, 0xD7, 0);

/**
	@class CColorRef  
	@brief Wrapper for COLORREF.
 */
class CColorRef  
{
	enum
	{
		BYTE_POS_RED		= 0x0,
		BYTE_POS_GREEN		= 0x1,
		BYTE_POS_BLUE		= 0x2,
		BYTE_POS_UNUSED		= 0x3,
	};
	union rgb_cracker
	{
		COLORREF	rgb_val ;
		BYTE		rgb_bytes[4] ;
		rgb_cracker() ;
		rgb_cracker( COLORREF val ) ;
	} ;

public:
	BYTE get_red_value() const ;
	BYTE get_green_value() const ;
	BYTE get_blue_value() const ;

	bool is_black() const ;
	bool is_white() const ;
	const string as_string() const ;
	const wstring as_wstring() const ;
	CColorRef& operator =(const COLORREF rhs) ;

	COLORREF as_colorref() const;

	const bool operator ==( const COLORREF rhs ) const;

	void invert() ;

	// construction/destruction
	explicit CColorRef( const wstring &color ) ;
	explicit CColorRef( COLORREF color );
	explicit CColorRef( BYTE red_byte, BYTE green_byte, BYTE blue_byte );
	CColorRef();

	const wstring byte_2_wstring( BYTE val ) const ;
	void from_string( const wstring &color ) ;
	rgb_cracker m_color ;

	void set_red_value(BYTE val) ;
	void set_green_value(BYTE val) ;
	void set_blue_value(BYTE val) ;

};

