#pragma once

#include <string>
#include <map>
#include "textstream_reader.h"

template< class CHAR_T >
class CTag
{
	typedef std::basic_string< CHAR_T > str_type ;
	typedef std::map< str_type, str_type > attr_type ;

	str_type m_tag_name ;
	attr_type m_attributes ;
	bool		m_is_end_tag ;

public:
	CTag() : m_is_end_tag(false) {}

	void parse_tag( const str_type &tag_text )
	{
		textstream_reader< CHAR_T > reader ;
		reader.set_buffer( tag_text.c_str() ) ;

		reader.eat_if( CHAR_T('<') ) ; 

		if ( reader.current_is( CHAR_T( '/' ) ) )
		{
			m_is_end_tag = true ;
			reader.advance( 1 ) ;
		}
		else
		{
			m_is_end_tag = false ;
		}

		str_type delims ;
		delims += CHAR_T(' ') ;
		delims += CHAR_T('\n') ;
		delims += CHAR_T('\r') ;
		delims += CHAR_T('\t') ;
		delims += CHAR_T('>') ;

		str_type quotes ;
		quotes += CHAR_T('"') ;
		quotes += CHAR_T('\'') ;

		ATLVERIFY(reader.getline( m_tag_name, delims.c_str(), true )) ;
		ATLASSERT ( m_tag_name.empty() == false ) ; 

		reader.eat_whitespace() ;

		while ( ! reader.empty() && ! reader.current_is( CHAR_T('>') ) ) 
		{
			const str_type key = reader.getline(CHAR_T('=')) ;
			reader.jump_to_first_of(quotes.c_str(), true) ;
			str_type val ;
			reader.getline(val, quotes.c_str(), true) ;
			if ( ! key.empty() && ! val.empty() ) 
			{
				m_attributes[key] = val ;
			}
			reader.eat_whitespace() ;
		}
	}
	bool is_end_tag() const
	{
		return m_is_end_tag ;
	}
	bool tag_is( const str_type &tag_name ) const
	{
		return boost::iequals( m_tag_name, tag_name ) ;
	}
	const str_type &get_tag() const
	{
		return m_tag_name ;
	}
	void get_attributes( attr_type &attributes ) const
	{
		attr_type tmp( m_attributes ) ;
		tmp.swap( attributes ) ;
	}
	const str_type get_attribute( const str_type &key ) const
	{
		attr_type::const_iterator pos = m_attributes.find(key) ;

		if ( pos == m_attributes.end() ) 
		{
			return str_type() ;
		}
		return pos->second ;
	}
};