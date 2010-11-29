// font_properties.h: interface for the font_properties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONT_PROPERTIES_H__9DCAAA74_BB6E_44B7_AA84_9846B13F01B4__INCLUDED_)
#define AFX_FONT_PROPERTIES_H__9DCAAA74_BB6E_44B7_AA84_9846B13F01B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorRef.h"

class font_properties  
{
	bool	m_bold_established ;
	bool	m_italic_established ;
	bool	m_underline_established ;
	bool	m_font_color_established ;
	bool	m_font_face_established ;
	bool	m_font_size_established ;
	bool	m_subscript_established ;
	bool	m_superscript_established ;

	bool			m_bold ;
	bool			m_italic ;
	bool			m_underline ;
	CColorRef	m_font_color ;
	wstring		m_font_face ;
	int			m_font_size ;
	bool			m_subscript ;
	bool			m_superscript ;

public:
	void set_superscript( bool setting ) ;
	void set_subscript( bool setting ) ;
	void set_bold( bool setting ) ;
	void set_italic( bool setting ) ;
	void set_underline( bool setting ) ;
	void set_font_color( CColorRef &setting ) ;
	void set_font_face( const wstring &setting ) ;
	void set_font_size( int size ) ;

	const bool get_superscript( ) const { return m_superscript ; }
	const bool get_subscript( ) const { return m_subscript ; }
	const bool get_bold( ) const ;
	const bool get_italic( ) const ;
	const bool get_underline( ) const ;
	const COLORREF get_font_color( ) const ;
	const wstring get_font_face( ) const ;
	const int get_font_size( ) const ;

	bool is_superscript_established( ) { return m_superscript_established ; }
	bool is_subscript_established( ) { return m_subscript_established ; }
	bool is_bold_established( ) ;
	bool is_italic_established( ) ;
	bool is_underline_established( ) ;
	bool is_font_color_established( ) ;
	bool is_font_face_established( ) ;
	bool is_font_size_established( ) ;

	font_properties& operator =( const font_properties &cpy ) ;
	font_properties( const font_properties &cpy ) ;

	font_properties();
	virtual ~font_properties();


};

#endif // !defined(AFX_FONT_PROPERTIES_H__9DCAAA74_BB6E_44B7_AA84_9846B13F01B4__INCLUDED_)
