/*!
	@file html_formatter.cpp
	@brief implementation of the html_formatter class.
	@date 2004/12/18
	Time: 1:25:26
	@author Ryan Ginstrom
 */

#include "stdafx.h"
#include "html_formatter.h"
#include "DebugUtilities.h"
#include "textstream_reader.h"

static LPCWSTR TAGNAME_BOLD			= L"B" ;
static LPCWSTR TAGNAME_ITALIC		= L"I" ;
static LPCWSTR TAGNAME_UNDERLINE	= L"U" ;
static LPCWSTR TAGNAME_SIZE			= L"FONT SIZE=\"" ;
static LPCWSTR TAGNAME_FCOLOR		= L"FONT COLOR=\"" ;
static LPCWSTR TAGNAME_FFACE		= L"FONT FACE=\"" ;
static LPCWSTR TAGNAME_VALIGN_SUP	= L"SUP" ;
static LPCWSTR TAGNAME_VALIGN_SUB	= L"SUB" ;

static LPCWSTR tag_names[] =
{
	TAGNAME_BOLD,
	TAGNAME_ITALIC, 
	TAGNAME_UNDERLINE,
	TAGNAME_SIZE, 
	TAGNAME_FCOLOR, 
	TAGNAME_FFACE,
	TAGNAME_VALIGN_SUP,
	TAGNAME_VALIGN_SUB
} ;

enum 
{ 
	TAG_TYPE_BOLD,
	TAG_TYPE_ITALIC,
	TAG_TYPE_UNDERLINE,
	TAG_TYPE_SIZE,
	TAG_TYPE_FORE_COLOR,
	TAG_TYPE_FONT_FACE,
	TAG_TYPE_V_ALIGN_SUP,
	TAG_TYPE_V_ALIGN_SUB,
	TAG_TYPE_INVALID
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
html_formatter::html_formatter()
{
	init() ;	
}

html_formatter::~html_formatter()
{

}


const wstring html_formatter::get_text()
{
	flush_buffer() ;
	close_off_tags() ;
	return m_text ;
}

void html_formatter::unwind_stack_until( const tag_type tag_to_check )
{
	ATLASSERT( m_current_buffer.empty() ) ;
	
	if ( ! tag_needs_closing( tag_to_check ) ) 
	{
		return ;
	}
	
	flush_buffer() ;

	tag_stack tags_to_put_back ;

	while( m_tags_to_close.empty() == false )
	{
		tag_type tag = m_tags_to_close.back() ;
		m_tags_to_close.pop_back() ;

		close_off_tag( tag ) ;

		if ( tag == tag_to_check )
		{
			switch( tag )
			{
			case TAG_TYPE_BOLD:
				pop_bold() ;
				break ;
				
			case TAG_TYPE_ITALIC:
				pop_italic() ;
				break ;
				
			case TAG_TYPE_UNDERLINE:
				pop_underline() ;
				break ;
				
			case TAG_TYPE_SIZE:
				pop_font_size() ;
				break ;
				
			case TAG_TYPE_FORE_COLOR:
				pop_fore_color() ;
				break ;
				
			case TAG_TYPE_FONT_FACE:
				pop_font_face() ;
				break ;

			case TAG_TYPE_V_ALIGN_SUB:
				pop_subscript() ;
				break ;

			case TAG_TYPE_V_ALIGN_SUP:
				pop_superscript() ;
				break ;

			default:
				ATLASSERT( FALSE && "Unknown tag encountered!" ) ;
			}

			// finalize the closures
			break ;
		}
		else
		{
			tags_to_put_back.push( tag ) ;
		}
		
	}

	while ( tags_to_put_back.empty() == false )
	{
		tag_type tag = tags_to_put_back.top() ;
		
		add_tag( tag ) ;
		
		tags_to_put_back.pop() ;
	}


}

void html_formatter::close_off_tags()
{
	
	ATLASSERT( m_current_buffer.empty() ) ;
	
	while( m_tags_to_close.empty() == false )
	{
		tag_type tag = m_tags_to_close.back() ;
		close_off_tag( tag ) ;
		m_tags_to_close.pop_back() ;
	}
	flush_buffer() ;
}

void html_formatter::close_off_tag( const tag_type tag_to_close )
{
	textstream_reader< wchar_t > reader ;

	wstring tag_text = tag_names[tag_to_close] ;

	reader.set_buffer( tag_text.c_str() ) ;
	const wstring tag = reader.getline(L' ') ;
	m_text << L"</" << tag << L">" ;
}

void html_formatter::flush_buffer()
{
	if ( m_current_buffer.empty() )
	{
		return ;
	}

	flush_tag_buffer() ;
	m_text << m_current_buffer ;
	m_current_buffer.erase() ;
}

// ---------------------------
// set various formatting
// ---------------------------


void html_formatter::set_bold( const bool setting )
{
	m_is_bold_specified = true ;

	if ( setting == get_bold() )
	{
		return ;
	}
	
	flush_buffer() ;

	if ( setting )
	{
		push_bold( setting ) ;
		add_tag( TAG_TYPE_BOLD ) ;
	}
	else
	{
		remove_from_tag_buffer( TAG_TYPE_BOLD ) ;
		unwind_stack_until( TAG_TYPE_BOLD ) ;
		push_bold( setting ) ;
	}
}

void html_formatter::set_italic( const bool setting )
{
	m_is_italic_specified = true ;

	if ( setting == get_italic() )
	{
		return ;
	}

	flush_buffer() ;
	
	if ( setting )
	{
		push_italic( setting ) ;
		add_tag( TAG_TYPE_ITALIC ) ;
	}
	else
	{
		remove_from_tag_buffer( TAG_TYPE_ITALIC ) ;
		unwind_stack_until( TAG_TYPE_ITALIC ) ;
		push_italic( setting ) ;
	}

}

void html_formatter::set_underline( const bool setting )
{
	m_is_underline_specified = true ;

	if ( setting == get_underline() )
	{
		return ;
	}
	
	flush_buffer() ;
	
	if ( setting )
	{
		push_underline( setting ) ;
		add_tag( TAG_TYPE_UNDERLINE ) ;
	}
	else
	{
		remove_from_tag_buffer( TAG_TYPE_UNDERLINE ) ;
		unwind_stack_until( TAG_TYPE_UNDERLINE ) ;
		push_underline( setting ) ;
	}

}

void html_formatter::set_fore_color( const CColorRef color )
{
	m_is_fore_color_specified = true ;

	COLORREF current_fore_color = get_fore_color() ;
	if ( color == current_fore_color )
	{
		return ;
	}
	
	flush_buffer() ;
	
	if ( tag_needs_closing( TAG_TYPE_FORE_COLOR ) )
	{
		unwind_stack_until( TAG_TYPE_FORE_COLOR ) ;
	}

	push_fore_color( color ) ;
	add_tag( TAG_TYPE_FORE_COLOR ) ;

}

// specify empty string to clear font-face information
void html_formatter::set_font_name( const wstring &font_name )
{
	if ( font_name == get_font_face() )
	{
		return ;
	}

	flush_buffer() ;
	
	if ( tag_needs_closing( TAG_TYPE_FONT_FACE ) )
	{
		unwind_stack_until( TAG_TYPE_FONT_FACE ) ;
	}
	
	if ( font_name.empty() )
	{
		remove_from_tag_buffer( TAG_TYPE_FONT_FACE ) ;
		return ;
	}

	push_font_face( font_name ) ;

	add_tag( TAG_TYPE_FONT_FACE ) ;

}

void html_formatter::set_size( const int font_size )
{
	m_is_font_size_specified = true ;

	int html_font_size = point_size_to_html_size( font_size ) ;
	
	if ( get_font_size() == html_font_size ) 
	{
		return ;
	}
	
	flush_buffer() ;
	
	unwind_stack_until( TAG_TYPE_SIZE ) ;

	if ( font_size == -1 )
	{
		remove_from_tag_buffer( TAG_TYPE_SIZE ) ;
		return ;
	}

	push_font_size( html_font_size ) ;

	add_tag( TAG_TYPE_SIZE ) ;
}

void html_formatter::set_subscript(bool setting)
{
	if ( setting == get_subscript() )
	{
		return ;
	}
	
	flush_buffer() ;
	
	if ( setting )
	{
		set_superscript( false ) ;
		push_subscript( setting ) ;
		add_tag( TAG_TYPE_V_ALIGN_SUB ) ;
	}
	else
	{
		remove_from_tag_buffer( TAG_TYPE_V_ALIGN_SUB ) ;
		unwind_stack_until( TAG_TYPE_V_ALIGN_SUB ) ;
		push_subscript( setting ) ;
	}
	
}

void html_formatter::set_superscript(bool setting)
{
	if ( setting == get_superscript() )
	{
		return ;
	}

	flush_buffer() ;
	
	if ( setting )
	{
		set_subscript( false ) ;
		push_superscript( setting ) ;
		add_tag( TAG_TYPE_V_ALIGN_SUP ) ;
	}
	else
	{
		remove_from_tag_buffer( TAG_TYPE_V_ALIGN_SUP ) ;
		unwind_stack_until( TAG_TYPE_V_ALIGN_SUP ) ;
		push_superscript( setting ) ;
	}
}

void html_formatter::add_paragraph_break()
{
	flush_buffer() ;
	m_text << L"\r\n<P>" ;
}

void html_formatter::add_linebreak()
{
	flush_buffer() ;
	m_text << L"<BR>\r\n" ;
}

void html_formatter::add_text ( const wstring &text )
{
	wstring text_to_add( text ) ;
	boost::replace_all( text_to_add, L"&", L"&amp;" ) ;
	boost::replace_all( text_to_add, L"<", L"&lt;" ) ;
	boost::replace_all( text_to_add, L">", L"&gt;" ) ;
	m_current_buffer << text_to_add ;
}

void html_formatter::set_justification( const JUSTIFICATION just_setting )
{
	push_justification( just_setting ) ;
}

const wstring html_formatter::get_paragraph_text() 
{
	wstring heading_type ;

	if ( m_heading_level == 0 )
	{
		heading_type = L"P" ;
	}
	else
	{
		heading_type = L"H" + int2wstring( m_heading_level ) ;
	}

	wstring par_format ;

	switch( get_justification() )
	{
	case JUST_CENTER:
		par_format = L"<%s ALIGN=\"MIDDLE\">" ;
		break ;

	case JUST_RIGHT:
		par_format = L"<%s ALIGN=\"RIGHT\">" ;
		break ;
		
	case JUST_LEFT:
		par_format = L"<%s ALIGN=\"LEFT\">" ;
		break ;
		
	case JUST_NONE:
		par_format = L"<%s>" ;
		break ;

	default:
		ATLASSERT( FALSE && "Unknown justification type!") ;
		par_format = L"<%s>" ;
		break ;
		
	}

	wformat start_tag = wformat(par_format) % heading_type ;
	wformat end_tag = wformat(L"</%s>") % heading_type ;
	return start_tag.str() + get_text() + end_tag.str() ;
}

void html_formatter::init()
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

	while( m_tags_to_close.empty() == false )
	{
		m_tags_to_close.pop_back() ;
	}

	m_current_buffer.erase() ;
	m_text.erase() ;

	m_heading_level = 0 ;
}

void html_formatter::flush_tag_buffer()
{
	
	while ( m_tag_buffer.empty() == false )
	{
		tag_type tag = m_tag_buffer.front() ;

		m_tag_buffer.pop_front( ) ;

		wstring tag_text = tag_names[tag] ;
		switch( tag )
		{
			// nothing needs to be added for bold, italic, or underline
		case TAG_TYPE_BOLD:  break ;
		case TAG_TYPE_ITALIC:  break ;
		case TAG_TYPE_UNDERLINE:  break ;
		case TAG_TYPE_V_ALIGN_SUB:  break ;
		case TAG_TYPE_V_ALIGN_SUP:  break ;
			
			// for the others, we must specify the extra bits
		case TAG_TYPE_SIZE: 
			{
				int font_size = get_font_size() ;
				tag_text << int2wstring( font_size ) ;
				tag_text << L"\"" ;
			}
			break ;
			
		case TAG_TYPE_FORE_COLOR: 
			{
				CColorRef fore_color( get_fore_color() ) ;
				tag_text << fore_color.as_wstring() ;
				tag_text << L"\"" ;
			}
			break ;
			
		case TAG_TYPE_FONT_FACE :
			{
				wstring font_face = get_font_face() ;
				tag_text << font_face ;
				tag_text << L"\"" ;
			}
			break ;

		default:
			ATLASSERT( FALSE && "Unknown tag encountered!" ) ;
			return ;
		}
		
		m_tags_to_close.push_back( tag ) ;
		
		m_text << L"<" << tag_text << L">" ;

	}
}

void html_formatter::add_tag( const tag_type tag )
{
	
	auto pos = 
		std::find( m_tag_buffer.begin(), m_tag_buffer.end(), tag ) ;

	if( pos == m_tag_buffer.end() )
	{
		m_tag_buffer.push_back( tag ) ;
	}

}

void html_formatter::set_heading_level(size_t level)
{
	m_heading_level = static_cast< int >( level ) ;
}

void html_formatter::clear_fore_color()
{
	flush_buffer() ;
	if ( tag_needs_closing( TAG_TYPE_FORE_COLOR ) )
	{
		unwind_stack_until( TAG_TYPE_FORE_COLOR ) ;
	}
	remove_from_tag_buffer( TAG_TYPE_FORE_COLOR ) ;
	init_fore_color() ;
}

void html_formatter::remove_from_tag_buffer(const tag_type tag)
{
	for( auto pos = m_tag_buffer.begin() ; pos != m_tag_buffer.end() ; ++pos )
	{
		if ( *pos == tag )
		{
			m_tag_buffer.erase( pos ) ;
			return ;
		}
	}
}


bool html_formatter::tag_needs_closing(const tag_type tag)
{
	for( auto close_tags_pos = m_tags_to_close.begin() ; close_tags_pos != m_tags_to_close.end() ; ++close_tags_pos )
	{
		if ( *close_tags_pos == tag )
		{
			return true ;
		}
	}

	return false ;
}

void html_formatter::set_v_alignment(VERT_ALIGNMENT valign)
{
	switch( valign )
	{
	case( VA_BASELINE ):
		set_subscript( false ) ;
		set_superscript( false ) ;
		return ;
	case( VA_SUPERSCRIPT )	:
		set_subscript( false ) ;
		set_superscript( true ) ;
		return ;
	case( VA_SUBSCRIPT ):
		set_subscript( true ) ;
		set_superscript( false ) ;
		return ;
	}
}
