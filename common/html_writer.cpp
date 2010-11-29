#include "stdafx.h"
#include "html_writer.h"
#include "tag_stripper.h"


// =====================
// base functions
// =====================

void html_writer::write_html( const wstring html_text )
{
	// &#32; entities will keep us from reading tags
	wstring local(html_text) ;
	boost::replace_all(local, L"&#32;", L" ") ;

	m_reader.set_buffer( local.c_str() ) ;

	push_defaults() ;

	while ( m_reader.empty() == false )
	{
		parse_text() ;
		parse_tag() ;
	}

	restore_original_states() ;
}

// private:

void html_writer::parse_text() 
{
	if ( m_reader.empty() )
	{
		return ;
	}

	const wstring text = m_reader.getline(L'<', false) ;

	if ( text.empty() == false )
	{
		write_text( strip_tags( text ) ) ;
	}
}

void html_writer::parse_tag()
{
	if ( m_reader.empty() )
	{
		return ;
	}

	ATLASSERT( m_reader.current_is( L'<' ) ) ;
	if ( m_reader.peek() != L'<' )
	{
		return ;
	}

	m_reader.advance() ;
	ATLASSERT( ! m_reader.current_is( L'<' ) ) ;

	if (iswspace(m_reader.peek()))
	{
		write_text(L"<") ;
		return ;
	}

	wstring tag = m_reader.getline(L'>') ;
	// these entities will keep us from reading tags
	boost::replace_all(tag, L"&#9;", L" ") ;
	boost::replace_all(tag, L"&#10;", L" ") ;
	boost::replace_all(tag, L"&#11;", L" ") ;
	boost::replace_all(tag, L"&#12;", L" ") ;

	textstream_reader< wchar_t > tag_reader ;
	tag_reader.set_buffer( tag.c_str() ) ;

	wstring tag_name, attributes ;

	tag_reader.getline( tag_name, L" \t\n\r>", false ) ;

	if ( tag_reader.is_space() )
	{
		tag_reader.eat_whitespace() ;
		tag_reader.getline( attributes, L"/>", true ) ;
	}

	boost::to_lower( tag_name ) ;

	// apply the tag formatting, further parsing the attributes if needed

	if ( tag_name.empty() == false && tag_name[0] == L'/' )
	{
		pop_tag( tag_name.substr(1) ) ;
	}
	else if ( tag_name == L"b" || tag_name == L"strong" )
	{
		push_bold( true ) ;
	}
	else if ( tag_name == L"i" || tag_name == L"em" )
	{
		push_italic( true ) ;
	}
	else if ( tag_name == L"u" )
	{
		push_underline( true ) ;
	}
	else if ( tag_name == L"font" )
	{
		parse_font( attributes ) ;
	}
	else if ( tag_name == L"span" )
	{
		parse_span( attributes ) ;
	}
	else if ( tag_name == L"br" || tag_name == L"br/" )
	{
		apply_linebreak( ) ;
	}
	else if ( tag_name == L"p" )
	{
		parse_paragraph( attributes ) ;
	}
	else if ( tag_name == L"p/" )
	{
		apply_paragraph() ;
	}
	else if ( tag_name == L"center" )
	{
		push_justification( JUST_CENTER ) ;
	}
	else if ( tag_name == L"sup" )
	{
		push_superscript( true ) ;
	}
	else if ( tag_name == L"sub" )
	{
		push_subscript( true ) ;
	}
	else
	{
		ATLTRACE( "*** Unknown tag name: %s\n", CW2A( tag_name.c_str() ) ) ;
		ATLASSERT(FALSE && "Unknown tag name") ;
	}
}

void html_writer::pop_tag( const wstring &tag )
{
	wstring tag_to_pop( tag ) ;
	boost::to_lower( tag_to_pop ) ;

	if ( tag_to_pop == L"b" || tag_to_pop == L"strong" )
	{
		pop_bold( ) ;
	}
	else if ( tag_to_pop == L"i" || tag_to_pop == L"em" )
	{
		pop_italic( ) ;
	}
	else if ( tag_to_pop == L"u" )
	{
		pop_underline( ) ;
	}
	else if ( tag_to_pop == L"font" )
	{
		int values = m_font_pop.top() ;
		m_font_pop.pop() ;
		pop_values( values ) ;
	}
	else if ( tag_to_pop == L"span" )
	{
		int values = m_span_pop.top() ;
		m_span_pop.pop() ;
		pop_values( values ) ;
	}
	else if ( tag_to_pop == L"p" )
	{
		int values = m_p_pop.top() ;
		m_p_pop.pop() ;
		pop_values( values ) ;
	}
	else if ( tag_to_pop == L"center" )
	{
		pop_justification( ) ;
	}
	else if ( tag_to_pop == L"sup" )
	{
		pop_superscript() ;
	}
	else if ( tag_to_pop == L"sub" )
	{
		pop_subscript( ) ;
	}
	else 
	{
		ATLASSERT( FALSE && "Unknown tag popped!" ) ;
	}
}

void html_writer::parse_font( const wstring &attributes ) 
{
	int pop_values = POP_NONE ;

	textstream_reader< wchar_t > reader ;
	reader.set_buffer( attributes.c_str() ) ;
	wstring key, val ;
	while ( parse_attribute( reader, key, val ) )
	{
		if ( boost::iequals( key, L"size" ) )
		{
			pop_values |= POP_SIZE ;
			push_font_size( val ) ;
		}
		else if ( boost::iequals( key, L"face" ) )
		{
			pop_values |= POP_FACE ;
			push_font_face( val ) ;
		}
		else if ( boost::iequals( key, L"color" ) )
		{
			pop_values |= POP_FORE_COLOR ;
			push_fore_color( CColorRef( val ) ) ;
		}
	}

	m_font_pop.push( pop_values ) ;
}

void html_writer::parse_span( const wstring &attributes ) 
{
	int pop_values = POP_NONE ;

	textstream_reader< wchar_t > reader ;
	reader.set_buffer( attributes.c_str() ) ;
	wstring key, val ;
	while ( parse_attribute( reader, key, val ) )
	{
	}
	
	m_span_pop.push( pop_values ) ;
}

void html_writer::parse_paragraph( const wstring &attributes ) 
{
	int pop_values = POP_NONE ;
	
	textstream_reader< wchar_t > reader ;
	reader.set_buffer( attributes.c_str() ) ;
	wstring key, val ;
	while ( parse_attribute( reader, key, val ) )
	{
	}

	m_p_pop.push( pop_values ) ;
	apply_paragraph( ) ;
}

bool html_writer::parse_attribute( textstream_reader<wchar_t> &reader, wstring &key, wstring &val )
{
	reader.eat_whitespace() ;

	if ( reader.empty() )
	{
		return false ;
	}

	reader.getline( key, L'=', true ) ;

	if ( reader.current_is( L'"' ) )
	{
		reader.advance() ;
		reader.getline( val, L'"', true ) ;
	}
	else
	{
		reader.getline( val, L" \t\n\r", false ) ;
	}

	return true ;
}

