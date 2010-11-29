#pragma once

#include "textstream_reader.h"
#include <string>
#include <list>
#include <vector>

template< class CHAR_T >
class CStringSplitter
{
	typedef textstream_reader< CHAR_T >			ReaderType ;
	typedef std::basic_string< CHAR_T >			StringType ;
	typedef std::vector< StringType >			ContainerType ;

	ContainerType	m_Elements ;
	StringType		m_SrcString ;

public:
	explicit CStringSplitter( const StringType &SrcString ) : m_SrcString( SrcString ) {}
	size_t size() const 
	{
		return m_Elements.size() ;
	}
	const StringType at( size_t index ) const 
	{
		return m_Elements.at( index ) ;
	}
	size_t Split( const StringType &Divider )
	{
		typedef textstream_reader< CHAR_T >::bookmark_type	BookmarkType ;
		// base case
		if ( Divider.empty() )
		{
			if ( ! m_SrcString.empty() )
			{
				m_Elements.push_back(m_SrcString) ;
			}
			return size() ;
		}

		ReaderType reader( m_SrcString.c_str() ) ;

		BookmarkType Start = reader.get_current_pos() ;
		while ( reader.find( Divider.c_str(), false ) )
		{
			BookmarkType End = reader.get_current_pos() ;
			reader.find( Divider.c_str(), true ) ; // skip past
			if ( Start != End )
			{
				m_Elements.push_back( StringType( Start, End ) ) ;
			}
			Start = reader.get_current_pos() ;
		}

		// final string
		if ( ! reader.empty() )
		{
			StringType str ;
			reader.getline( str, CHAR_T('\0') ) ;
			m_Elements.push_back( str ) ;
		}

		return size() ;
	}
	size_t Tokenize( const StringType &Delims )
	{
		// base case
		if ( Delims.empty() )
		{
			if ( ! m_SrcString.empty() )
			{
				m_Elements.push_back(m_SrcString) ;
			}
			return size() ;
		}

		ReaderType reader( m_SrcString.c_str() ) ;

		StringType token ;
		while ( ! reader.empty() && reader.getline( token, Delims.c_str() ) )
		{
			if ( ! token.empty() )
			{
				m_Elements.push_back( token ) ;
			}
			while ( Delims.find( reader.peek() ) != StringType::npos )
			{
				reader.advance() ;
			}
		}

		ATLASSERT( reader.empty() ) ;

		return size() ;
	}
};
