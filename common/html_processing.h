#pragma once

#include "StringEx.h"
#include <stack>

#include "StringConversions.h"
#include "ColorRef.h"

const static int DEFAULT_HTML_FONT_SIZE = 4 ;

unsigned int clamp_html_size( unsigned int html_size ) ;

unsigned int get_num_fonts();

unsigned int html_size_to_point_size( unsigned int html_size ) ;
unsigned int point_size_to_html_size( unsigned int point_size ) ;
unsigned int get_font_point_diff( unsigned int point_size, size_t i );

class html_processing
{
public:
	enum JUSTIFICATION 
	{ 
		JUST_NONE, 
		JUST_LEFT, 
		JUST_CENTER, 
		JUST_RIGHT 
	} ;
	
	enum Trival 
	{ 
		TRIVAL_FALSE, 
		TRIVAL_TRUE, 
		TRIVAL_UNDETERMINED 
	} ;

	enum VERT_ALIGNMENT
	{
		VA_BASELINE,
		VA_SUBSCRIPT,
		VA_SUPERSCRIPT
	};

private:
	typedef std::stack< wstring >			font_face_stack ;
	typedef std::stack< bool >				bold_stack ;
	typedef std::stack< bool >				italic_stack ;
	typedef std::stack< bool >				underline_stack ;
	typedef std::stack< unsigned int >		font_size_stack ;
	typedef std::stack< COLORREF >			color_stack ;
	typedef std::stack< JUSTIFICATION >		just_stack ;
	typedef std::stack< bool >				subscript_stack ;
	typedef std::stack< bool >				superscript_stack ;
	typedef std::stack< unsigned int >		value_pop_stack ;
	
	bold_stack			m_bold ;
	italic_stack		m_italic ;
	underline_stack		m_underline ;
	color_stack			m_fore_color ;
	color_stack			m_back_color ;
	font_face_stack		m_font_face ;
	font_size_stack		m_font_size ;
	just_stack			m_justification ;
	subscript_stack		m_subscript ;
	superscript_stack	m_superscript ;
	

protected:
	value_pop_stack		m_font_pop ;
	value_pop_stack		m_span_pop ;
	value_pop_stack		m_p_pop ;

	bool m_is_fore_color_specified ;
	bool m_is_font_size_specified ;
	bool m_is_bold_specified ;
	bool m_is_italic_specified ;
	bool m_is_underline_specified ;


public:
	bool is_underline_specified();
	bool is_italic_specified();
	bool is_bold_specified();
	bool is_font_size_specified();
	bool is_fore_color_specified();

	// =====================
	// status access
	// =====================
	
	bool get_bold() 
	{ 
		ATLASSERT( m_bold.empty() == false ) ;
		return m_bold.top()  ; 
	}
	bool get_italic() 
	{ 
		ATLASSERT( m_italic.empty() == false ) ;
		return m_italic.top()   ; 
	}
	bool get_underline() 
	{ 
		ATLASSERT( m_underline.empty() == false ) ;
		return m_underline.top()   ; 
	}

	COLORREF get_fore_color() 
	{ 
		ATLASSERT( m_fore_color.empty() == false ) ;
		return m_fore_color.top() ; 
	}
	COLORREF get_back_color() 
	{ 
		ATLASSERT( m_back_color.empty() == false ) ;
		return m_back_color.top() ; 
	}

	wstring get_font_face() 
	{ 
		ATLASSERT( m_font_face.empty() == false ) ;
		return m_font_face.top() ; 
	}

	JUSTIFICATION get_justification() 
	{ 
		ATLASSERT( m_justification.empty() == false ) ;
		return m_justification.top() ; 
	}
	
	bool get_subscript()
	{
		ATLASSERT( m_subscript.empty() == false ) ;
		return ( m_subscript.top() ) ;
	}
	bool get_superscript()
	{
		ATLASSERT( m_superscript.empty() == false ) ;
		return ( m_superscript.top() ) ;
	}

	VERT_ALIGNMENT get_v_align() 
	{ 
		if ( m_subscript.empty() == false )
		{
			if ( m_subscript.top() )
			{
				return VA_SUBSCRIPT ;
			}
		}

		if ( m_superscript.empty() == false )
		{
			if ( m_superscript.top() )
			{
				return VA_SUPERSCRIPT ;
			}
		}

		return VA_BASELINE ; 
	}


	void restore_original_states()  ;
	unsigned int get_font_size() ;

	unsigned int font_size_in_points();

	html_processing() :
		m_is_fore_color_specified ( false ),
		m_is_font_size_specified ( false ),
		m_is_bold_specified ( false ),
		m_is_italic_specified ( false ),
		m_is_underline_specified ( false )
	{}

public:


	enum  
	{ 
		POP_NONE		= 0x00000000,
		POP_FACE		= 0x00000001, 
		POP_SIZE		= 0x00000002, 
		POP_FORE_COLOR 	= 0x00000004, 
		POP_BACK_COLOR 	= 0x00000008, 
		POP_BOLD 		= 0x00000010, 
		POP_ITALIC		= 0x00000020,
		POP_JUSTIFY		= 0x00000040
	} ;
	
	// =====================
	// state initialization
	// =====================
	
	void init_state();

	void init_bold( bool setting = false ) ;
	void init_italic( bool setting = false ) ;
	void init_underline( bool setting = false ) ;

	void init_fore_color( CColorRef setting = CColorRef() )  ;
	void init_back_color( CColorRef setting = CColorRef() )  ;

	void init_font_size( int setting = 4 )  ;

	void init_justification( JUSTIFICATION setting = JUST_NONE ) ;
	
	// initialize to empty string by default (means use current font)
	void init_font_face( const wstring &setting = wstring() )  ;
	
	void init_v_alignment( const VERT_ALIGNMENT valign = VA_BASELINE ) ;
	// =====================
	// push states
	// =====================
	
	void push_defaults()  ;
	void push_justification( JUSTIFICATION setting = JUST_NONE ) ;
	void push_bold( bool setting = false ) ;
	void push_italic( bool setting = false ) ;
	void push_underline( bool setting = false )  ;
	void push_fore_color( CColorRef setting = CColorRef() )  ;
	void push_back_color( CColorRef setting = CColorRef() )  ;
	// initialize to empty string by default (means use current font)
	void push_font_face( const wstring &setting = wstring() )  ;
	void push_font_size( unsigned int setting = DEFAULT_HTML_FONT_SIZE )  ;
	void push_font_size( const wstring &setting )  ;

	void push_subscript( bool setting = false ) ;
	void push_superscript( bool setting = false ) ;
	
	// =====================
	// pop states
	// =====================
	
	void pop_justification( ) ;
	void pop_bold( ) ;
	void pop_italic( ) ;
	void pop_underline( ) ;
	void pop_fore_color( ) ;
	void pop_back_color( ) ;
	void pop_font_face( ) ;
	void pop_font_size( )  ;
	void pop_subscript() ;
	void pop_superscript() ;
	void pop_values( unsigned int values ) ;
};