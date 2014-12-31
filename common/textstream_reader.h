/*!
	@file textstream_reader.h
	@brief textsteam_reader header file
	@date 2004/12/17
	Time: 23:20:25
	@author Ryan Ginstrom
 */

#pragma once

#include "stringex.h"

/**  CLASS textstream_reader.
 This class is for parsing streams of characters.
 It is templated to allow both wide and narrow-character streams.
 In order to ensure safety the multibyte versions have to
 be used for the narrow-character routines, which will compromise
 speed to some extent. However, this class should still be quite fast
 compared to higher-level implementations.
 */
template< class CHAR_T >
class textstream_reader
{
	typedef	CHAR_T							char_type ;
	typedef CHAR_T*							pointer_type ;
	typedef const CHAR_T*					const_pointer_type ;
	typedef textstream_reader< CHAR_T >		reader_type ;
	typedef std::basic_string< CHAR_T >		string_type ;

	const_pointer_type	m_buffer ;			///< The buffer
	const_pointer_type	m_pos ;				///< The current pos in the buffer

public:

	typedef const CHAR_T*	bookmark_type ; ///< For remembering points in buffer

	/** constructor. 
	@param reader parameter. 
	*/
	textstream_reader( const reader_type &reader ) : 
		m_buffer( reader.m_buffer ), 
		m_pos( reader.m_pos )
	{	
	}


	/** constructor. 
	@param buffer A text buffer. The caller must not free this buffer until the reader is done with it. 
	*/
	explicit textstream_reader( const_pointer_type buffer = NULL ) : 
		m_buffer( buffer ), 
		m_pos( buffer )
	{
	}



	/** Get the length of the buffer. 
	@return size_t description. 
	*/
	size_t length()
	{
		if ( m_buffer == NULL )
			return 0 ;

		return str::generic_charlen( m_buffer ) ;
	}



	/** Get the length from the current pos to the end of the buffer. 
	@return size_t description. 
	*/
	size_t length_at_pos()
	{
		if ( this->empty() )
			return 0 ;
		return str::generic_charlen( m_pos ) ;
	}



	/** Set the buffer. 
	The caller must not free this buffer until the reader is done with it. 
	@param buffer parameter. 
	*/
	void set_buffer( const_pointer_type buffer ) 
	{ 
		m_pos = m_buffer = buffer ; 
	}

	// assignment operators


	/** Assignment operator. 
	@return reader_type& reference to self. 
	@param buffer parameter. 
	*/
	reader_type &operator =( const_pointer_type buffer )
	{
		set_buffer( buffer ) ;
		return *this ;
	}


	/** Assignment operator. 
	@return reader_type& reference to self. 
	@param reader parameter. 
	*/
	reader_type &operator =( const reader_type &reader )
	{
		m_buffer = reader.m_buffer ;
		m_pos = reader.m_pos ;
		return *this ;
	}



	/** Whether the buffer is empty. 
	@return bool description. 
	*/
	bool empty() const
	{
		if ( m_pos == NULL ) 
		{
			return true ;
		}
		
		if ( *m_pos == char_type(0) ) 
		{
			return true ;
		}

		return false ;
	}

	/** Determines if the present character is a whitespace character. 
	@return bool description. 
	*/
	bool is_space() const
	{
		return str::generic_isspace( peek() ) ;
	}



	/** Determines if the present character is a punctuation character. 
	@return bool description. 
	*/
	bool is_punct() const
	{
		return str::generic_ispunct( peek() ) ;
	}

	/** Determines if the present character is a digit. 
	@return bool description. 
	*/
	bool is_digit() const
	{
		return str::generic_isdigit( peek() ) ;
	}

	bool is_hexint_digit() const
	{
		if ( str::generic_isdigit( peek() ) ) 
		{
			return true ;
		}

		switch( peek() ) 
		{
		case CHAR_T('a'):
		case CHAR_T('b'):
		case CHAR_T('c'):
		case CHAR_T('d'):
		case CHAR_T('e'):
		case CHAR_T('f'):
		case CHAR_T('A'):
		case CHAR_T('B'):
		case CHAR_T('C'):
		case CHAR_T('D'):
		case CHAR_T('E'):
		case CHAR_T('F'):
			return true ;
		default:
			return false ;
		}
	}

	/** Determines if the present character is a the specified character. 
	@return bool The answer. 
	@param comp_char The character to check. 
	*/
	bool current_is( char_type comp_char ) const
	{
		if ( m_pos == NULL ) 
		{
			return false ;
		}
		
		return  *m_pos == comp_char ;
	}



	/** Determines if the string at the current pos matches the specified string. 
	@return bool description. 
	@param comp_str A string to match. Must not be NULL.
	*/
	bool current_is( const_pointer_type comp_str ) const
	{
		if ( comp_str == NULL )
		{
			throw std::invalid_argument("Null pointer") ;
		}
		
		if ( m_pos == comp_str )
		{
			return true ;
		}

		if ( this->empty() )
		{
			return false ;
		}

		return ( 0 == str::generic_strncmp( m_pos, comp_str, str::generic_strlen( comp_str ) ) ) ;

	}


	/** Take a peek at the current character, without eating it. 
	@return char_type description. 
	@param  parameter. 
	*/
	char_type peek(  ) const
	{ 
		if ( m_pos == NULL ) 
		{
			return char_type(0) ;
		}
		else
		{
			return *m_pos ;
		}
	}


	/** Advance the specified number of steps. 
	@return bool Success. 
	@param steps The number of steps to advance. 
	*/
	bool advance( size_t steps=1 )
	{
		if ( this->empty() ) 
		{
			return false ;
		}

		const_pointer_type temp ;

		for ( size_t i=0 ; i<steps ; ++i )
		{
			temp = m_pos ;
			m_pos = str::generic_char_next( temp ) ;
			if ( ! m_pos )
				return false ;
			if ( ! *m_pos )
				return false ;
		}
		
		if ( ! m_pos ) return false ;

		return *m_pos != char_type(0) ;
	}


	/** Get the next character. 
	@return char_type The next character. 
	*/
	char_type nextc() 
	{ 
		char_type c ;

		if ( this->empty() ) 
		{
			c = char_type(0) ;
		}
		else
		{
			m_pos = str::generic_char_next( m_pos ) ;
			c = *m_pos ;
		}

		return c ; 
	}


	/** Get the current character, and advance. 
	@return char_type The current character. 
	*/
	char_type get() 
	{ 
		char_type c ; 
		get( c ) ; 

		return c ; 
	}


	/** Get the current character, and advance. 
	@return bool Success. 
	@param c The current character. 
	*/
	bool get( char_type &c )
	{
		if ( this->empty() )
		{
			c = char_type(0) ;
			return false ;
		}

		c = *m_pos ;

		m_pos = str::generic_char_next( m_pos ) ;
		return true ;
	}


	/** Get the current character, and advance. 
	@return bool Success. 
	@param str The current character is appended to this string. 
	*/
	bool get( string_type &str )
	{
		if ( this->empty() ) 
		{
			return false ;
		}

		const_pointer_type next_pos = str::generic_char_next( m_pos ) ;

		if ( next_pos == NULL )
		{
			return false ;
		}

		string_type current_char( m_pos, next_pos ) ;

		str.append( current_char ) ;
		m_pos = next_pos ;

		return true ;
	}

	/** Get the specified number of characters. broken into a separate function, instead of default param, for speed of base case
	@return bool description. 
	@param str parameter. 
	@param num_steps parameter. 
	*/
	bool get( string_type &str, size_t num_steps )
	{
		for( size_t i=0 ; i<num_steps ; ++i )
		{
			get( str ) ;
		}
		return true ;
	}


	/** Back up the specified number of steps. 
	@return char_type description. 
	@param steps parameter. 
	*/
	char_type unget( size_t steps = 1 ) 
	{ 
		char_type c = 0 ;
		if ( m_pos == NULL || m_pos == m_buffer )
		{
			c = char_type(0) ;
		}
		else
		{
			for ( size_t i=0 ; i<steps ; ++i )
			{
				if ( m_pos != m_buffer )
					m_pos = str::generic_char_prev( m_buffer, m_pos ) ; 
			}
			if (m_pos)
			{
				c = *m_pos;
			}
		}

		return c; 
	}


	/** Get the buffer. 
	@return const_pointer_type description. 
	*/
	const_pointer_type get_buffer() 
	{ 
		return m_buffer ; 
	}


	/** Eat if the current character is the specified character. 
	@return bool description. 
	@param c parameter. 
	*/
	bool eat_if( char_type c )
	{
		if( current_is( c ) )
		{
			advance() ;
			return true ;
		}
		return false ;
	}


	/** Eat until the next non-whitespace character. 
	@return bool Success. False if m_pos is NULL 
	*/
	bool eat_whitespace()
	{
		if ( m_pos == NULL )
		{
			return false ;
		}

		while ( str::generic_isspace( *m_pos ) ) 
		{
			++m_pos ;  // mb spaces are only one byte, so this is safe
		}
		return true ;
	}

	/** Jump to the first of the specified character.
	If the function fails, no jump is made and returns false. 
	Set eat to true to jump past char being jumped to
	@return bool description. 
	@param delim parameter. 
	@param eat parameter. 
	*/
	bool jump_to_first_of( char_type delim, bool eat=false )
	{
		if ( this->empty() )
			return false ;
		const_pointer_type new_pos = str::generic_strchr( m_pos, delim ) ;
		if ( new_pos == NULL )
			return false ;
		m_pos = new_pos ;
		if ( eat ) m_pos = str::generic_char_next( m_pos ) ;
		return true ;
	}

	/** Jump to the first of the specified string.
	If the function fails, no jump is made and returns false. 
	Set eat to true to jump past string being jumped to
	@return bool description. 
	@param delims parameter. 
	@param eat parameter. 
	*/
	bool jump_to_first_of( const_pointer_type delims, bool eat=false )
	{
		if ( this->empty() )
			return false ;
		const_pointer_type new_pos = str::generic_strpbrk( m_pos, delims ) ;
		if ( new_pos == NULL )
			return false ;
		m_pos = new_pos ;
		if ( eat ) 
			m_pos = str::generic_char_next( m_pos ) ;
		return true ;
	}


	/** Gets the text up to the end-of-line marker (specified by user). 
	If the delim is not found, gets the rest of the buffer.
	The pos ends up 1 beyond the characters in the line.
	@return bool description. 
	@param str parameter. 
	@param '\n' parameter. 
	@param eat parameter. 
	*/
	bool getline( string_type &str, char_type delim = char_type('\n'), bool eat=true )
	{
		if ( this->empty() ) 
		{
			str.erase() ;
			return false ;
		}
		const_pointer_type end_pos = str::generic_strchr( m_pos, delim ) ;

		return str_from_endpos( str, end_pos, eat ) ;

	}
	string_type getline(char_type delim = char_type('\n'), bool eat=true )
	{
		string_type str ;
		this->getline(str, delim, eat) ;
		return str ;
	}

	std::vector<string_type> &split(std::vector<string_type> &items, 
									char_type delim = char_type(' '))
	{
		while (! this->empty())
		{
			string_type item ;
			this->getline(item, delim, true) ;
			items.push_back(item) ;
			while (this->peek() == delim)
			{
				this->advance() ;
			}
		}
		return items ;
	}
	std::vector<string_type> &split(std::vector<string_type> &items, 
		const_pointer_type delims)
	{
		while (! this->empty())
		{
			string_type item ;
			this->getline(item, delims, true) ;
			items.push_back(item) ;
			while(str::contains(delims, this->peek()) && ! this->empty())
			{
				this->advance() ;
			}
		}
		return items ;
	}
	/** Gets the text up to the end-of-line marker (specified by user). 
	If the delim is not found, gets the rest of the buffer.
	The pos ends up 1 beyond the characters in the line.
	Any text already in the string will be erased.
	@return bool description. 
	@param str parameter. 
	@param delims parameter. 
	@param eat parameter. 
	*/
	bool getline( string_type &str, const_pointer_type delims, bool eat=true )
	{
		ATLASSERT( delims != NULL ) ;

		if ( delims==NULL || this->empty()) 
		{
			str.erase() ;
			return false ;
		}

		const_pointer_type end_pos = str::generic_strpbrk( m_pos, delims ) ;

		return str_from_endpos( str, end_pos, eat ) ;
	}
	string_type getline_as_string(const_pointer_type delims, bool eat=true )
	{
		string_type chunk ;
		this->getline(chunk, delims, eat) ;
		return chunk ;
	}

	string_type getline_delims(const_pointer_type delims, bool eat=true )
	{
		string_type out ;
		this->getline(out, delims, eat) ;
		return out ;
	}

	string_type get_until( const_pointer_type endPos, bool eat=true )
	{
		bookmark_type bookmarkStart = get_current_pos() ;
		if ( ! find( endPos, false ) )
		{
			set_pos( bookmarkStart ) ;
			return string_type() ;
		}

		bookmark_type bookmarkEnd = get_current_pos() ;

		if ( eat )
		{
			find( endPos, true ) ;
		}

		return string_type( bookmarkStart, bookmarkEnd ) ;
	}

	/** Set the buffer back to the beginning. 
	*/
	void rewind() 
	{ 
		m_pos = m_buffer ; 
	}


	/** Find the specified string. 
	If the string is not found, the pos does not move. 
	@return bool Found. 
	@param substring The string to search for. 
	@param eat Whether to advance beyond the string if found. 
	*/
	bool find( const_pointer_type substring, bool eat=false )
	{
		ATLASSERT( substring != NULL ) ;

		if ( substring == NULL ) return false ;
		if ( this->empty() ) return false ;

		bookmark_type old_pos = m_pos ;
		m_pos = str::generic_strstr( m_pos, substring ) ;

		if ( m_pos == NULL ) 
		{
			m_pos = old_pos ;
			return false ;
		}

		if ( eat ) 
		{
			m_pos += str::generic_strlen( substring ) ;
		}

		return true ;
	}


	/** Jump the specified offset.
	For textstream_reader< char >, this is in bytes, rather than characters. 
	To jump forward the specified number of characters, use @ref advance( size_t ).
	There is no difference for wchar_t, except that unlike advance, 
	this function allows you to jump forwards or backwards.
	@param offset The offset to jump. Can be positive or negative. 
	*/
	void jump( int offset )
	{
		m_pos += offset ;
	}

	// for bookmarking positions in the buffer

	/** The current pos. 
	@return bookmark_type The current pos. 
	*/
	bookmark_type get_current_pos() 
	{ 
		return m_pos ; 
	}


	/** Set the current pos. Usually with a bookmark that was previously saved. 
	@param pos parameter. 
	*/
	void set_pos( bookmark_type pos ) 
	{ 
		m_pos = pos ; 
	}

	/** Get the offset of the specified bookmark_type from the beginning of the buffer. 
	A bit computation-intensive...
	@return size_t description. 
	@param bookmark parameter. 
	*/
	size_t get_offset( bookmark_type bookmark ) const
	{
		size_t offset = 0 ;

		for ( offset=0 ; m_buffer+offset != bookmark ; ++offset ) ;

		return offset ;
	}


	/** Get the offset of the current pos from the specified offset from the beginning of the buffer. 
	For example, if you already know that the pos is at least 10, you can specify 10 so that
	it does not count back from the beginning. 
	A bit computation-intensive...
	@return size_t description. 
	@param start_offset parameter. 
	*/
	size_t get_offset( size_t start_offset = 0 )
	{
		if ( m_buffer + start_offset > m_pos )
		{
			throw std::out_of_range("Invalid index") ;
		}

		size_t offset = 0 ;

		for ( offset=start_offset ; m_buffer+offset != m_pos ; ++offset ) ;

		return offset ;
	}
		

	private:
	/** Get the string from the buffer pos up to a specified end-pos. 
	For example, the position of a period or newline. 
	@return bool Success. 
	@param s Holds the result. 
	@param end_pos The end pos. 
	@param eat Whether to eat the characters. 
	*/
	bool str_from_endpos( string_type &s, const_pointer_type end_pos, bool eat )
	{
		// delim not found
		if ( end_pos == NULL || *end_pos == char_type(0) )
		{
			s = m_pos ;
			m_pos = NULL ;
			return true ;
		}
		
		s = string_type( m_pos, end_pos ) ;
		m_pos = end_pos ;
		if ( eat ) m_pos = str::generic_char_next( m_pos ) ; // skip the delimiter
		
		return true ;
	}

} ;

typedef textstream_reader< char >		c_reader ;
typedef textstream_reader< wchar_t >	wc_reader ;


