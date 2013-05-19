/*!
string extension utilities
*/

#pragma once

#include <mbstring.h>	// _ismbcspace() et al
// std string support

// other necessary stl stuff

#include <math.h>	// ERANGE


// other libraries
#include "mbctype.h" // for multibyte stuff

static const size_t MAX_NUM_STRLEN = 35 ;

/** string type definitions */
typedef std::basic_string< TCHAR > tstring ;
using std::string ;
using std::wstring ;


//-----------------
// stream text
//-----------------

// character stream

/** stream operator for strings.
Allows things like this: my_string << "hello, " << my_location ; 
@return string. 
@param lhs string. 
@param rhs const char *. 
*/
inline string &operator << ( string &lhs, const char *rhs )
{
	if( rhs == NULL ) 
		return lhs ;

	lhs += rhs ;
	return lhs ;
}


/** stream operator for wstring.
Allows things like this: my_string << L"hello, " << my_location ; 
@return wstring. 
@param lhs wstring. 
@param rhs const wchar_t *. 
*/
inline wstring &operator << ( wstring &lhs, const wchar_t *rhs )
{
	if ( rhs == NULL )
		return lhs ;

	lhs += rhs ;
	return lhs ;
}

/** stream operator for strings.
Allows things like this: my_string << "hello, " << my_location ; 
@return string. 
@param lhs string. 
@param rhs const string. 
*/
inline string &operator << ( string &lhs, const string &rhs )
{
	lhs += rhs ;
	return lhs ;
}

/** stream operator for wstring.
Allows things like this: my_string << L"hello, " << my_location ; 
@return wstring. 
@param lhs wstring. 
@param rhs const wstring. 
*/
inline wstring &operator << ( wstring &lhs, const wstring rhs )
{
	lhs += rhs ;
	return lhs ;
}



/**
	@namespace source_text
	@brief generic string  comparison/transformation functions.
 */
namespace str
{


	/** generic version of isdigit.
	Overloaded to work with both Unicode and ASCII cases. 
	This is the Unicode version.
	@return bool is it a digit? 
	@param c the char to test. 
	*/
	inline bool generic_isdigit( wchar_t c )
	{
		return iswdigit( c ) != 0 ;
	}


	/** generic version of isdigit.
	Overloaded to work with both Unicode and ASCII cases. 
	This is the ASCII version.
	@return bool is it a digit? 
	@param c the char to test. 
	*/
	inline bool generic_isdigit( char c )
	{
		return isdigit( static_cast< unsigned char >( c ) ) != 0 ;
	}

	// isspace


	/** generic version of isspace.
	Overloaded to work with both Unicode and ASCII cases. 
	This is the Unicode version.
	@return bool is it a space? 
	@param c the char to test. 
	*/
	inline bool generic_isspace( wchar_t c ) 
	{ 
		bool retval = iswspace( c ) != 0 ; 
		
		return retval ;
	}

	/** generic version of isspace.
	Overloaded to work with both Unicode and ASCII cases. 
	This is the ASCII version.
	@return bool is it a space? 
	@param c the char to test. 
	*/
	inline bool generic_isspace( char c ) 
	{ 
		bool retval = _ismbcspace( c ) != 0 ; 

		return retval ;
	}



	/** generic version of ispunct.
	Overloaded to work with both Unicode and ASCII cases. 
	This is the Unicode version.
	@return bool is it a punctuation character? 
	@param c the char to test. 
	*/
	inline bool generic_ispunct( wchar_t c ) 
	{ 
		bool retval = iswpunct( c ) != 0 ; 

		return retval ;
	}

	/** generic version of ispunct.
	Overloaded to work with both Unicode and ASCII cases. 
	This is the ASCII version.
	@return bool is it a punctuation character? 
	@param c the char to test. 
	*/
	inline bool generic_ispunct( char c ) 
	{ 
		bool retval = _ismbcpunct( c ) != 0 ; 

		return retval ;
	}

	// strchr


	// generic_strncmp
	inline int generic_strncmp( const wchar_t *string1, const wchar_t *string2, size_t count )
	{
		_ASSERTE( string1 != NULL ) ;
		_ASSERTE( string2 != NULL ) ;
		
		if (string1 == NULL || string2 == NULL)
		{
			throw std::invalid_argument("NULL pointer to generic_strncmp") ;
		}
		return wcsncmp( string1, string2, count ) ;
	}
 	inline int generic_strncmp( const char *string1, const char *string2, size_t count )
	{
		_ASSERTE( string1 != NULL ) ;
		_ASSERTE( string2 != NULL ) ;
		
		if (string1 == NULL || string2 == NULL)
		{
			throw std::invalid_argument("NULL pointer to generic_strncmp") ;
		}

		return _mbsncmp( (const unsigned char*)string1, (const unsigned char*)string2, count ) ;
	}


	// generic_strchr
	inline const wchar_t *generic_strchr( const wchar_t *pos, wchar_t delim )
	{
		_ASSERTE( pos != NULL ) ;

		if (!pos)
		{
			throw std::invalid_argument("NULL pointer to generic_strchr") ;
		}
		const wchar_t * retval = wcschr( pos, delim ) ; 

		return retval ;
	}
	inline const char *generic_strchr( const char *pos, char delim )
	{
		_ASSERTE( pos != NULL ) ;
		
		if (!pos)
		{
			throw std::invalid_argument("NULL pointer to generic_strchr") ;
		}
		const char* retval = (const char*) _mbschr( (const BYTE*)pos, delim )  ; 

		return retval ;
	}

	// generic_strpbrk
	inline const wchar_t *generic_strpbrk( const wchar_t *pos, const wchar_t * delim )
	{
		_ASSERTE( pos != NULL ) ;
		_ASSERTE( delim != NULL ) ;

		if (!pos || !delim)
		{
			throw std::invalid_argument("NULL pointer to generic_strpbrk") ;
		}
		return wcspbrk( pos, delim ) ;
	}
	inline const char *generic_strpbrk( const char *pos, const char * delim )
	{
		_ASSERTE( pos != NULL ) ;
		_ASSERTE( delim != NULL ) ;

		if (!pos || !delim)
		{
			throw std::invalid_argument("NULL pointer to generic_strpbrk") ;
		}
		return (const char*) _mbspbrk( (const BYTE*)pos, (const BYTE*)delim ) ;
	}

	// generic_strstr
	inline const wchar_t *generic_strstr( const wchar_t *pos, const wchar_t * delim )
	{
		_ASSERTE( pos != NULL ) ;
		_ASSERTE( delim != NULL ) ;

		if (!pos || !delim)
		{
			throw std::invalid_argument("NULL pointer to generic_strstr") ;
		}
		return wcsstr( pos, delim ) ;
	}
	inline const char *generic_strstr( const char *pos, const char * delim )
	{
		_ASSERTE( pos != NULL ) ;
		_ASSERTE( delim != NULL ) ;

		if (!pos || !delim)
		{
			throw std::invalid_argument("NULL pointer to generic_strstr") ;
		}
		return (const char*)strstr( pos, delim ) ;
	}

	// generic_char_next
	inline const char *generic_char_next( const char *pos ) 
	{ 
		_ASSERTE( pos != NULL ) ;

		if (!pos)
		{
			throw std::invalid_argument("NULL pointer to generic_char_next") ;
		}
		return CharNextA( pos ) ; 
	}
	inline const wchar_t *generic_char_next( const wchar_t *pos ) 
	{ 
		_ASSERTE( pos != NULL ) ;

		if (!pos)
		{
			throw std::invalid_argument("NULL pointer to generic_char_next") ;
		}
		return CharNextW( pos ) ; 
	}


	// generic_char_prev
	inline const wchar_t * generic_char_prev( const wchar_t *buf, const wchar_t *pos ) 
	{ 
		_ASSERTE( buf != NULL ) ;
		_ASSERTE( pos != NULL ) ;

		if (!pos || !buf)
		{
			throw std::invalid_argument("NULL pointer to generic_char_prev") ;
		}

		if ( pos == buf )
			return buf ;

		--pos ;
		return pos ;
	}
	inline const char * generic_char_prev( const char *buf, const char *pos ) 
	{ 
		_ASSERTE( buf != NULL ) ;
		_ASSERTE( pos != NULL ) ;

		if (!pos || !buf)
		{
			throw std::invalid_argument("NULL pointer to generic_char_prev") ;
		}
		return CharPrevA( buf, pos ) ; 
	}

	// generic_strlen
	inline size_t generic_strlen( const wchar_t *s ) 
	{ 
		_ASSERTE( s != NULL ) ;

		if (!s)
		{
			throw std::invalid_argument("NULL pointer to generic_strlen") ;
		}
		return wcslen( s ); 
	}
	inline size_t generic_strlen( const char *s ) 
	{ 
		_ASSERTE( s != NULL ) ;

		if (!s)
		{
			throw std::invalid_argument("NULL pointer to generic_strlen") ;
		}
		return strlen( s ) ;
	}


	//  generic_strdist
	template< typename CHAR_T >
	inline size_t generic_strdist( const CHAR_T *start, const CHAR_T *stop )
	{
		BOOST_STATIC_ASSERT(sizeof(CHAR_T) == sizeof(char) || sizeof(CHAR_T) == sizeof(wchar_t)) ;
		_ASSERTE( start != NULL ) ;

		if ( start == NULL )
		{
			return 0 ;
		}

		size_t dist(0) ;
		if ( stop == NULL )
		{
			while( *start )
			{
				++start ;
				++dist ;
			}
		}
		else
		{
			while( start != stop )
			{
				++start ;
				++dist ;
			}
		}
		return dist ;
	}

	//  generic_charlen
	inline size_t generic_charlen( const char *s ) 
	{ 
		_ASSERTE( s != NULL ) ;

		if ( s == NULL )
		{
			throw std::invalid_argument("NULL pointer to generic_charlen") ;
		}
		// _mbslen tests string length for the codepage currently in use
		// use _mbstrlen to get the length of the string for the current locale
		return _mbslen( (const BYTE*)s ); 
	}
	inline size_t generic_charlen( const wchar_t *s ) 
	{ 
		_ASSERTE( s != NULL ) ;

		if ( s == NULL )
		{
			throw std::invalid_argument("NULL pointer to generic_charlen") ;
		}
		return wcslen( s ); 
	}

	//  is_uint_rep
	inline bool is_uint_rep( const string &source_text )
	{
		return ( source_text.find_first_not_of( "0123456789" ) == string::npos ) ;
	}
	inline bool is_uint_rep( const wstring source_text )
	{
		return ( source_text.find_first_not_of( L"0123456789" ) == wstring::npos ) ;
	}

	//  is_int_rep
	inline bool is_int_rep( const string &source_text )
	{
		return ( source_text.find_first_not_of( "-0123456789" ) == string::npos ) ;
	}
	inline bool is_int_rep( const wstring source_text )
	{
		return ( source_text.find_first_not_of( L"-0123456789" ) == wstring::npos ) ;
	}

	//  is_hexint_rep
	inline bool is_hexint_rep( const string &source )
	{
		return boost::all(source, boost::is_xdigit()) ;
	}
	inline bool is_hexint_rep( const wstring source )
	{
		return boost::all(source, boost::is_xdigit()) ;
	}


	//  left
   template< typename CHAR_T >
	inline std::basic_string< CHAR_T > left( const std::basic_string< CHAR_T > &source, size_t len )
	{
		BOOST_STATIC_ASSERT(sizeof(CHAR_T) == sizeof(char) || sizeof(CHAR_T) == sizeof(wchar_t)) ;
		return source.substr( 0, len ) ;
	}

	//  right
	 template< typename CHAR_T >
	inline std::basic_string< CHAR_T > right( const std::basic_string< CHAR_T > &source, size_t len )
	{
		BOOST_STATIC_ASSERT(sizeof(CHAR_T) == sizeof(char) || sizeof(CHAR_T) == sizeof(wchar_t)) ;
		size_t start = source.size() ;
		if ( len >= start ) 
		{
			return std::basic_string< CHAR_T >() ;
		}

		return source.substr(start - len) ;
	}

static const size_t HIRAGANA_START = 0x3041 ;
static const size_t HIRAGANA_END = 0x309F ;

static const size_t FULL_KATAKANA_UNICODE_START = 0x30A1 ;
static const size_t KATAKANA_START = 0x30A0 ;
static const size_t KATAKANA_END = 0x30FF ;

static const wchar_t HIRAGANA_OFFSET = ((wchar_t)(FULL_KATAKANA_UNICODE_START -HIRAGANA_START)) ;
	
	inline bool is_hiragana(wchar_t c)
	{
		return HIRAGANA_START <= c && c <= HIRAGANA_END ;
	}
	inline bool is_katakana(wchar_t c)
	{
		return KATAKANA_START <= c && c <= KATAKANA_END ;
	}

	/** converts Hiragana to Katakana
	*/
	inline bool normalize_hiragana_to_katakana( wstring &source_str )
	{
		size_t len = source_str.size() ;
		wchar_t c ;
		
		for ( size_t i = 0u ; i < len ; ++i )
		{
			c = source_str[i] ;
			
			// upper case letters
			if (is_hiragana(c))
			{
				source_str[i] = (wchar_t)(c + HIRAGANA_OFFSET) ;
			}
		}
		return source_str.empty() == false ;
	}

static const size_t FULLWIDTH_UPPER_START = 0xFF21 ;
static const size_t FULLWIDTH_UPPER_END = 0xFF39 ;
static const size_t FULLWIDTH_LOWER_START = 0xFF41 ;
static const size_t FULLWIDTH_LOWER_END = 0xFF59 ;
static const size_t FULLWIDTH_NUMBER_START = 0xFF10 ;
static const size_t FULLWIDTH_NUMBER_END = 0xFF19 ;
static const wchar_t UPPERCASE_LETTER_OFFSET = ((wchar_t)(FULLWIDTH_UPPER_START - 0x0041)) ;
static const wchar_t LOWERCASE_LETTER_OFFSET = ((wchar_t)(FULLWIDTH_LOWER_START - 0x0061)) ;
static const wchar_t NUMBER_OFFSET = ((wchar_t)(FULLWIDTH_NUMBER_START - 0x0030)) ;

static const wchar_t IDEOGRAPHIC_SPACE = (wchar_t)0x3000 ;

	/** is_double_byte
	*/
	inline bool is_double_byte( wchar_t c )
	{
		return ( c > (wchar_t)0x00FF ) ;
	}
	inline bool is_asian(wchar_t c)
	{
		return c > IDEOGRAPHIC_SPACE ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Full-width romaji
	//////////////////////////////////////////////////////////////////////////

	// is_fullwidth_
	inline bool is_fullwidth_upper(const wchar_t c)
	{
		return ( c >= FULLWIDTH_UPPER_START && c <= FULLWIDTH_UPPER_END ) ;
	}
	inline bool is_fullwidth_lower(const wchar_t c)
	{
		return ( c >= FULLWIDTH_LOWER_START && c <= FULLWIDTH_LOWER_END ) ;
	}
	inline bool is_fullwidth_number(const wchar_t c)
	{
		return ( c >= FULLWIDTH_NUMBER_START && c <= FULLWIDTH_NUMBER_END ) ;
	}

	// normalize_fullwidth_
	inline wchar_t normalize_fullwidth_upper(const wchar_t c)
	{
		return (wchar_t)(c - UPPERCASE_LETTER_OFFSET);
	}
	inline wchar_t normalize_fullwidth_lower(const wchar_t c)
	{
		return (wchar_t)(c - LOWERCASE_LETTER_OFFSET );
	}
	inline wchar_t normalize_fullwidth_number(const wchar_t c)
	{
		return (wchar_t)(c - NUMBER_OFFSET );
	}
	/** normalize_width
	*/
	inline wstring & normalize_width( wstring &source_str )
	{
		size_t len = source_str.size() ;
		wchar_t c ;

		for ( size_t i = 0u ; i < len ; ++i )
		{
			c = source_str[i] ;
			
			// upper case letters
			if ( is_fullwidth_upper(c) )
			{
				source_str[i] = normalize_fullwidth_upper(c) ;
			}
			// lower case letters
			else if ( is_fullwidth_lower(c) )
			{
				source_str[i] = normalize_fullwidth_lower(c) ;
			}
			// numbers
			else if ( is_fullwidth_number(c) )
			{
				source_str[i] = normalize_fullwidth_number(c);
			}
			else switch( c )
			{
			case	0xFF0C: // FULLWIDTH COMMA
				source_str[i] = L',' ;
				break ;
			case	0xFF0E: // FULLWIDTH FULL STOP
				source_str[i] = L'.' ;
				break ;
			case	0xFF1A: // FULLWIDTH COLON
				source_str[i] = L':' ;
				break ;
			case	0xFF1B: // FULLWIDTH SEMICOLON
				source_str[i] = L';' ;
				break ;
			case	0xFF1F: // FULLWIDTH QUESTION MARK
				source_str[i] = L'?' ;
				break ;
			case	0xFF01: // FULLWIDTH EXCLAMATION MARK
				source_str[i] = L'!' ;
				break ;
			case	0xFF40: // FULLWIDTH GRAVE ACCENT
				source_str[i] = (wchar_t)0x0060 ;
				break ;
			case	0xFF3E: // FULLWIDTH CIRCUMFLEX ACCENT
				source_str[i] = (wchar_t)0x005E ;
				break ;
			case	0xFFE3: // FULLWIDTH MACRON
				source_str[i] = L'^' ;
				break ;
			case	0xFF3F: // FULLWIDTH LOW LINE
				source_str[i] = (wchar_t)0x005F ;
				break ;
			case	0xFF0F:	// FULLWIDTH SOLIDUS
				source_str[i] = L'/' ;
				break ;
			case	0xFF3C:	// FULLWIDTH REVERSE SOLIDUS
				source_str[i] = L'\\' ;
				break ;
			case	0xFF5E: // FULLWIDTH TILDE
				source_str[i] = L'~' ;
				break ;
			case	0xFF5C: // FULLWIDTH VERTICAL LINE
				source_str[i] = L'|' ;
				break ;
			case	0xFF08: // FULLWIDTH LEFT PARENTHESIS
				source_str[i] = L'(' ;
				break ;
			case	0xFF09: // FULLWIDTH RIGHT PARENTHESIS
				source_str[i] = L')' ;
				break ;
			case	0xFF3B: // FULLWIDTH LEFT SQUARE BRACKET
				source_str[i] = L'[' ;
				break ;
			case	0xFF3D: // FULLWIDTH RIGHT SQUARE BRACKET
				source_str[i] = L']' ;
				break ;
			case	0xFF5B: // FULLWIDTH LEFT CURLY BRACKET
				source_str[i] = L'{' ;
				break ;
			case	0xFF5D: // FULLWIDTH RIGHT CURLY BRACKET
				source_str[i] = L'}' ;
				break ;
			case	0xFF0B: // FULLWIDTH PLUS SIGN
				source_str[i] = L'+' ;
				break ;
			case	0xFF0D: // FULLWIDTH HYPHEN-MINUS
				source_str[i] = L'-' ;
				break ;
			case	0xFF1D: // FULLWIDTH EQUALS SIGN
				source_str[i] = L'=' ;
				break ;
			case	0xFF1C: // FULLWIDTH LESS-THAN SIGN
				source_str[i] = L'<' ;
				break ;
			case	0xFF1E: // FULLWIDTH GREATER-THAN SIGN
				source_str[i] = L'>' ;
				break ;
			case	0xFF04: // FULLWIDTH DOLLAR SIGN
				source_str[i] = L'$' ;
				break ;
			case	0xFF05: // FULLWIDTH PERCENT SIGN
				source_str[i] = L'%' ;
				break ;
			case	0xFF03: // FULLWIDTH NUMBER SIGN
				source_str[i] = L'#' ;
				break ;
			case	0xFF06: // FULLWIDTH AMPERSAND
				source_str[i] = L'&' ;
				break ;
			case	0xFF0A: // FULLWIDTH ASTERISK
				source_str[i] = L'*' ;
				break ;
			case	0xFF20: // FULLWIDTH COMMERCIAL AT
				source_str[i] = L'@' ;
				break ;
			case	0xFF61: // HALFWIDTH IDEOGRAPHIC FULL STOP
				source_str[i] = (wchar_t)0x3002 ;
				break ;
			case	0xFF62: // HALFWIDTH LEFT CORNER BRACKET
				source_str[i] = (wchar_t)0x300C ;
				break ;
			case	0xFF63: // HALFWIDTH RIGHT CORNER BRACKET
				source_str[i] = (wchar_t)0x300D ;
				break ;
			case	0xFF64: // HALFWIDTH IDEOGRAPHIC COMMA
				source_str[i] = (wchar_t)0x3001 ;
				break ;
			case	0xFF65: // HALFWIDTH KATAKANA MIDDLE DOT
				source_str[i] =  (wchar_t)0x30FB ;
				break ;
			case	0xFF66: // HALFWIDTH KATAKANA LETTER WO
				source_str[i] = (wchar_t)0x30F2 ;
				break ;
			case	0xFF67: // HALFWIDTH KATAKANA LETTER SMALL A
				source_str[i] = (wchar_t)0x30A1 ;
				break ;
			case	0xFF68: // HALFWIDTH KATAKANA LETTER SMALL I
				source_str[i] = (wchar_t)0x30A3 ;
				break ;
			case	0xFF69: // HALFWIDTH KATAKANA LETTER SMALL U
				source_str[i] = (wchar_t)0x30A5 ;
				break ;
			case	0xFF6A: // HALFWIDTH KATAKANA LETTER SMALL E
				source_str[i] = (wchar_t)0x30A7 ;
				break ;
			case	0xFF6B: // HALFWIDTH KATAKANA LETTER SMALL O
				source_str[i] = (wchar_t)0x30A9 ;
				break ;
			case	0xFF6C: // HALFWIDTH KATAKANA LETTER SMALL YA
				source_str[i] = (wchar_t)0x30E3 ;
				break ;
			case	0xFF6D: // HALFWIDTH KATAKANA LETTER SMALL YU
				source_str[i] = (wchar_t)0x30E5 ;
				break ;
			case	0xFF6E: // HALFWIDTH KATAKANA LETTER SMALL YO
				source_str[i] = (wchar_t)0x30E7 ;
				break ;
			case	0xFF6F: // HALFWIDTH KATAKANA LETTER SMALL TU
				source_str[i] = (wchar_t)0x30C3 ;
				break ;
			case	0xFF70: // HALFWIDTH KATAKANA-HIRAGANA PROLONGED SOUND MARK
				source_str[i] = (wchar_t)0x30FC ;
				break ;
			case	0xFF71: // HALFWIDTH KATAKANA LETTER A
				source_str[i] = (wchar_t)0x30A2 ;
				break ;
			case	0xFF72: // HALFWIDTH KATAKANA LETTER I
				source_str[i] = (wchar_t)0x30A4 ;
				break ;
			case	0xFF73: // HALFWIDTH KATAKANA LETTER U
				source_str[i] = (wchar_t)0x30A6 ;
				break ;
			case	0xFF74: // HALFWIDTH KATAKANA LETTER E
				source_str[i] = (wchar_t)0x30A8 ;
				break ;
			case	0xFF75: // HALFWIDTH KATAKANA LETTER O
				source_str[i] = (wchar_t)0x30AA ;
				break ;
			case	0xFF76: // HALFWIDTH KATAKANA LETTER KA
				c = source_str[i+1] ;
				if ( c == (wchar_t)0xFF9E )		// GA
				{
					source_str[i] = (wchar_t)0x30AC ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30AB ;
				break ;
			case	0xFF77: // HALFWIDTH KATAKANA LETTER KI
				c = source_str[i+1] ;
				if ( c == (wchar_t)0xFF9E )		// GI
				{
					source_str[i] = (wchar_t)0x30AE ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30AD ;
				break ;
			case	0xFF78: // HALFWIDTH KATAKANA LETTER KU
				c = source_str[i+1] ;
				if ( c == (wchar_t)0xFF9E )		// GU
				{
					source_str[i] = (wchar_t)0x30B0 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30AF ;
				break ;
			case	0xFF79: // HALFWIDTH KATAKANA LETTER KE
				c = source_str[i+1] ;
				if ( c == (wchar_t)0xFF9E )		// GE
				{
					source_str[i] = (wchar_t)0x30B2 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30B1 ;
				break ;
			case	0xFF7A: // HALFWIDTH KATAKANA LETTER KO
				c = source_str[i+1] ;
				if ( c == (wchar_t)0xFF9E )		// GO
				{
					source_str[i] = (wchar_t)0x30B4 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30B3 ;
				break ;
			case	0xFF7B: // HALFWIDTH KATAKANA LETTER SA
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// ZA
				{
					source_str[i] = (wchar_t)0x30B6 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30B5 ;
				break ;
			case	0xFF7C: // HALFWIDTH KATAKANA LETTER SI
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// ZI
				{
					source_str[i] = (wchar_t)0x30B8 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30B7 ;
				break ;
			case	0xFF7D: // HALFWIDTH KATAKANA LETTER SU
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// ZU
				{
					source_str[i] = (wchar_t)0x30BA ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30B9 ;
				break ;
			case	0xFF7E: // HALFWIDTH KATAKANA LETTER SE
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// ZE
				{
					source_str[i] = (wchar_t)0x30BC ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30BB ;
				break ;
			case	0xFF7F: // HALFWIDTH KATAKANA LETTER SO
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// ZO
				{
					source_str[i] = (wchar_t)0x30BE ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30BD ;
				break ;
			case	0xFF80: // HALFWIDTH KATAKANA LETTER TA
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// DA
				{
					source_str[i] = (wchar_t)0x30C0 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30BF ;
				break ;
			case	0xFF81: // HALFWIDTH KATAKANA LETTER TI
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// DI
				{
					source_str[i] = (wchar_t)0x30C2 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30C1 ;
				break ;
			case	0xFF82: // HALFWIDTH KATAKANA LETTER TU
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// DU
				{
					source_str[i] = (wchar_t)0x30C5 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30C4 ;
				break ;
			case	0xFF83: // HALFWIDTH KATAKANA LETTER TE
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// DE
				{
					source_str[i] = (wchar_t)0x30C7 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30C6 ;
				break ;
			case	0xFF84: // HALFWIDTH KATAKANA LETTER TO
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// DO
				{
					source_str[i] = (wchar_t)0x30C9 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30C8 ;
				break ;
			case	0xFF85: // HALFWIDTH KATAKANA LETTER NA
				source_str[i] = (wchar_t)0x30CA ;
				break ;
			case	0xFF86: // HALFWIDTH KATAKANA LETTER NI
				source_str[i] = (wchar_t)0x30CB ;
				break ;
			case	0xFF87: // HALFWIDTH KATAKANA LETTER NU
				source_str[i] = (wchar_t)0x30CC ;
				break ;
			case	0xFF88: // HALFWIDTH KATAKANA LETTER NE
				source_str[i] = (wchar_t)0x30CD ;
				break ;
			case	0xFF89: // HALFWIDTH KATAKANA LETTER NO
				source_str[i] = (wchar_t)0x30CE ;
				break ;
			case	0xFF8A: // HALFWIDTH KATAKANA LETTER HA
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// HA
				{
					source_str[i] = (wchar_t)0x30D0 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else if ( c == 0xFF9F )			// PA
				{
					source_str[i] = (wchar_t)0x30D1 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30CF ;
				break ;
			case	0xFF8B: // HALFWIDTH KATAKANA LETTER HI
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// HI
				{
					source_str[i] = (wchar_t)0x30D3 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else if ( c == 0xFF9F )			// PI
				{
					source_str[i] = (wchar_t)0x30D4 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30D2 ;
				break ;
			case	0xFF8C: // HALFWIDTH KATAKANA LETTER HU
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// HU
				{
					source_str[i] = (wchar_t)0x30D6 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else if ( c == 0xFF9F )			// PU
				{
					source_str[i] = (wchar_t)0x30D7 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30D5 ;
				break ;
			case	0xFF8D: // HALFWIDTH KATAKANA LETTER HE
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// HE
				{
					source_str[i] = (wchar_t)0x30D9 ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else if ( c == 0xFF9F )			// PE
				{
					source_str[i] = (wchar_t)0x30DA ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30D8 ;
				break ;
			case	0xFF8E: // HALFWIDTH KATAKANA LETTER HO
				c = source_str[i+1] ;
				if ( c == 0xFF9E )				// HO
				{
					source_str[i] = (wchar_t)0x30DC ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else if ( c == 0xFF9F )			// PO
				{
					source_str[i] = (wchar_t)0x30DD ;
					source_str.erase( i+1, 1) ;
					_ASSERTE( len > source_str.size() ) ;
					len = source_str.size() ;
				}
				else 
					source_str[i] = (wchar_t)0x30DB ;
				break ;
			case	0xFF8F: // HALFWIDTH KATAKANA LETTER MA
				source_str[i] = (wchar_t)0x30DE ;
				break ;
			case	0xFF90: // HALFWIDTH KATAKANA LETTER MI
				source_str[i] = (wchar_t)0x30DF ;
				break ;
			case	0xFF91: // HALFWIDTH KATAKANA LETTER MU
				source_str[i] = (wchar_t)0x30E0 ;
				break ;
			case	0xFF92: // HALFWIDTH KATAKANA LETTER ME
				source_str[i] = (wchar_t)0x30E1 ;
				break ;
			case	0xFF93: // HALFWIDTH KATAKANA LETTER MO
				source_str[i] = (wchar_t)0x30E2 ;
				break ;
			case	0xFF94: // HALFWIDTH KATAKANA LETTER YA
				source_str[i] = (wchar_t)0x30E4 ;
				break ;
			case	0xFF95: // HALFWIDTH KATAKANA LETTER YU
				source_str[i] = (wchar_t)0x30E6 ;
				break ;
			case	0xFF96: // HALFWIDTH KATAKANA LETTER YO
				source_str[i] = (wchar_t)0x30E8 ;
				break ;
			case	0xFF97: // HALFWIDTH KATAKANA LETTER RA
				source_str[i] = (wchar_t)0x30E9 ;
				break ;
			case	0xFF98: // HALFWIDTH KATAKANA LETTER RI
				source_str[i] = (wchar_t)0x30EA ;
				break ;
			case	0xFF99: // HALFWIDTH KATAKANA LETTER RU
				source_str[i] = (wchar_t)0x30EB ;
				break ;
			case	0xFF9A: // HALFWIDTH KATAKANA LETTER RE
				source_str[i] = (wchar_t)0x30EC ;
				break ;
			case	0xFF9B: // HALFWIDTH KATAKANA LETTER RO
				source_str[i] = (wchar_t)0x30ED ;
				break ;
			case	0xFF9C: // HALFWIDTH KATAKANA LETTER WA
				source_str[i] = (wchar_t)0x30EF ;
				break ;
			case	0xFF9D: // HALFWIDTH KATAKANA LETTER N
				source_str[i] = (wchar_t)0x30F3 ;
				break ;
			}
		}

		return source_str ;
	}

	inline bool is_number(const wstring source)
	{
		wstring tmp(boost::trim_copy(source)) ;
		if (tmp.empty())
		{
			return false ;
		}
		// strip trailing percent sign
		size_t len = tmp.size() ;
		if (tmp[len-1] == L'%')
		{
			tmp = tmp.substr(0, len-1) ;
		}
		// these are the characters that a number is allowed to have
		return ( tmp.find_first_not_of( L"-0123456789.," ) == wstring::npos ) ;

	}

	template< typename CHAR_T >
	bool contains(const CHAR_T *chars, CHAR_T c)
	{
		BOOST_STATIC_ASSERT(sizeof(CHAR_T) == sizeof(char) || sizeof(CHAR_T) == sizeof(wchar_t)) ;
		int len = generic_strlen(chars) ;
		for (int i = 0; i < len; ++i)
		{
			if (c == chars[i])
			{
				return true ;
			}
		}
		return false ;
	}

} ;
inline bool has_asian(const wstring s)
{
	FOREACH(wchar_t c, s)
	{
		if (str::is_asian(c))
		{
			return true ;
		}
	}
	return false ;
}
inline bool has_spaces(const wstring s)
{
	FOREACH(wchar_t c, s)
	{
		if (iswspace(c))
		{
			return true ;
		}
	}
	return false ;
}

