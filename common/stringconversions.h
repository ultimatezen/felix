#pragma once

#pragma message( "   Processing StringConversions.h...")

#include "StringEx.h"	// string extensions
#include "winnls.h" // unicode-multibyte conversion
#include "errno.h"
#include "textstream_reader.h"
#include "comutil.h" // _bstr_t and _variant_t

#pragma warning( disable : 4701 ) // ����������Ă��Ȃ��\���̂��郍�[�J���ϐ� 'result' ���g�p����܂�
#include <boost/lexical_cast.hpp>
#pragma warning( default : 4701 )

#include <boost/scoped_array.hpp>

static const size_t ONEK = 1024 ;
static const size_t ONEM = (ONEK*ONEK) ;
static const size_t ONEG = (ONEM*ONEK) ;

// forward declarations
wstring resource2wstring( const UINT uid, const HINSTANCE instance ) ;
string string2string( const wstring &str, UINT code_page = _getmbcp() ) ;
wstring string2wstring( const wstring &str, UINT code_page = _getmbcp() ) ;


/***************************
 *
 * string conversion routines
 *
 ***************************/

inline string string2string( const wstring &str, const UINT code_page )
{
	if ( str.empty() ) 
	{
		return string() ;
	}

	// Calculate the required length of the buffer
	const size_t len_needed = ::WideCharToMultiByte(code_page, 0, str.c_str(), (UINT)(str.length()) , NULL, 0, NULL, NULL) ;

	_ASSERTE(len_needed > 0) ;
	if ( len_needed == 0 ) 
	{
		return string() ;
	}

    // Create the buffer
	boost::scoped_array<char> buff(new char[len_needed+1]) ;

	// Do the conversion.
	const size_t num_copied = ::WideCharToMultiByte(code_page,		// UINT CodPage
													0,				// DWORD dwFlags, 
													str.c_str(),	// LPCWSTR lpWideCharStr,
													static_cast<UINT>(str.size()), // int cchWideChar, 
													buff.get(),		// LPSTR lpMultiByteStr, 
													static_cast<int>(len_needed+1),	// int cbMultiByte,
													NULL,			// LPCSTR lpDefaultChar,    
													NULL			// LPBOOL lpUsedDefaultChar
													) ;
	_ASSERTE(num_copied == len_needed) ;
	_ASSERTE(num_copied > 0) ;
	_ASSERTE(! str.empty()) ;

	if (0 == num_copied) return string() ;

	buff[num_copied] = 0 ;

	return string(buff.get(), num_copied) ;
}


// convert to wstring

inline wstring string2wstring( const wstring &str, const UINT)
{
	return wstring( str ) ;
}
inline wstring string2wstring( const string &str, const UINT code_page = _getmbcp() )
{
	if ( str.empty() ) 
	{
		return wstring() ;
	}
	
	// Calculate the required length of the buffer
	const size_t len_needed = ::MultiByteToWideChar(code_page, 0, str.c_str(), (UINT)(str.length()) , NULL, 0 );

	_ASSERTE(len_needed > 0) ;
	if (0 == len_needed) 
	{
		return wstring() ;
	}
	
	// Create the buffer
	boost::scoped_array<wchar_t> buff(new wchar_t[len_needed+1]) ;

	// Do the conversion.
	const size_t num_copied = ::MultiByteToWideChar(code_page, 
													0, 
													str.c_str(), 
													static_cast<UINT>(str.size()), 
													buff.get(),
													static_cast<int>(len_needed+1)) ;
	_ASSERTE(num_copied == len_needed) ;
	_ASSERTE(num_copied > 0) ;
	_ASSERTE(! str.empty()) ;

	if (0 == num_copied) 
	{
		return wstring() ;
	}

	buff[num_copied] = 0 ;
	
	return wstring(buff.get(), num_copied) ;
}

/* Converts the string into a wstring using the system's multibyte
   code page, then converts that into a string using the supplied
   code page. It's a way to convert e.g. SJIS to UTF-8 in one go.
   If the original string isn't in the system code page, then you
   should do this manually:
   const wstring tmp = string2wstring(s, CP_XX) ;
   return string2wstring(tmp, CP_UTF8) ;
 */
inline string string2string( const string &str, UINT code_page = CP_ACP  )
{
	if ( _getmbcp() == static_cast<int>(code_page) )
		return string(str) ;

	const wstring w = string2wstring( str, _getmbcp() ) ;
	return string2string( w, code_page ) ;
}

// convert from BSTR

inline wstring BSTR2wstring( const BSTR &bstr )
{
	if (! bstr)
	{
		return wstring() ;
	}
	const int len = ::SysStringLen(bstr) ;
	if ( len == 0 )
	{
		return wstring() ;
	}

	// the string may have embedded nulls...
	return wstring( (const OLECHAR*)bstr, len ) ;
}

inline string BSTR2string( const BSTR &bstr, UINT code_page = _getmbcp() )
{
	const wstring tmp = BSTR2wstring( bstr ) ;
	return string2string( tmp, code_page ) ;
}



inline _bstr_t string2BSTR( const wstring str )
{
	if (str.empty())
	{
		return _bstr_t() ;
	}
	_ASSERTE(str.size() > 0) ;
	return _bstr_t(::SysAllocStringLen(str.c_str(), static_cast<UINT>(str.size())), false) ;
}
inline _bstr_t string2BSTR( const string str, UINT cp = CP_ACP )
{
	const wstring tmp = string2wstring( str, cp ) ;
	return string2BSTR(tmp) ;
}

// conversion to tstring

inline tstring string2tstring( const string &str, UINT code_page = _getmbcp()  )
{
#ifdef _UNICODE
	return string2wstring( str, code_page ) ;
#else
	return string2string( str, code_page ) ;
#endif
}

inline tstring string2tstring( const wstring &str, UINT code_page = _getmbcp()  )
{
#ifdef _UNICODE
	return string2wstring( str, code_page ) ;
#else
	return string2string( str, code_page ) ;
#endif
}


inline tstring BSTR2tstring( const BSTR &bstr, UINT code_page = _getmbcp()  )
{
	const wstring rhs = BSTR2wstring( bstr ) ;
	return string2tstring( rhs, code_page ) ;
}

inline tstring utf82tstring( const string &str )
{
	return string2tstring( str, CP_UTF8 ) ;
}

//***************************
//* data type conversions
//***************************

inline double string2double( const wstring &str )
{
	try
	{
		return boost::lexical_cast<double>(str) ;
	}
	catch (boost::bad_lexical_cast& e)
	{
		std::string message ("Failed to convert string to double: ") ;
		message += e.what();
		throw std::exception(message.c_str()) ;
	}
}

inline double string2double( const string &str )
{
	const wstring wstr = string2wstring( str ) ;

	return string2double( wstr ) ;
}

inline long string2long( const string &str, int base=10 )
{
	// global value...
	errno = 0 ;
	const long val = strtol( str.c_str(), NULL, base ) ;

	_ASSERTE( base != 10 || val == boost::lexical_cast< long >( str ) ) ;
	_ASSERTE( errno != ERANGE ) ;
	if( errno == ERANGE ) 
	{
		std::string message ("Failed to convert string to double ") ;
		message += str + " to long";
		throw std::exception(message.c_str()) ;
	}
	return val ;
}

inline long string2long( const wstring &str, int base=10 )
{
	return string2long(string2string(str), base) ;
}

inline unsigned long string2ulong( const string &str, int base=10 )
{
	errno = 0 ;
	const unsigned long val = strtoul( str.c_str(), NULL, base ) ;

	_ASSERTE( base != 10 || val == boost::lexical_cast< unsigned long >( str ) ) ;
	_ASSERTE( errno != ERANGE ) ;
	if( errno == ERANGE ) 
	{
		std::string message = std::string("Failed to convert string to string ") +
			str +
			" to ulong";
		throw std::exception(message.c_str()) ;
	}
	return val ;
}

inline unsigned long string2ulong( const wstring &str, int base=10 )
{
	return string2ulong(string2string(str), base) ;
}


inline bool string2bool( const string &flag )
{
	return ( 0 == _stricmp(flag.c_str(), "true" ) ) ;
}
inline bool string2bool( const wstring &flag )
{
	return ( 0 == _wcsicmp(flag.c_str(), L"true" ) ) ;
}

inline string bool2string( const bool flag )
{
	if ( flag ) return string( "true" ) ;

	return string( "false" ) ;
}

inline tstring bool2tstring( const bool flag )
{
	if ( flag ) return tstring(_T("true") ) ;

	return 	 tstring(_T("false") ) ;
}


inline wstring bool2wstring( const bool flag )
{
	if ( flag ) return wstring( L"true" ) ;

	return wstring( L"false" ) ;
}

inline string int2string( const int i, const int radix = 10 )
{
	char buf[35] = { 0 } ;

	if (EINVAL == _itoa_s( i, buf, 35, radix ))
	{
		std::string message("Failed to convert int to string");
		throw std::exception(message.c_str()) ;
	}

	return string( buf ) ;
}


inline wstring int2wstring( const int i, const int radix = 10 )
{
	wchar_t buf[35] = { 0 } ;

	if (EINVAL == _itow_s( i, buf, 35, radix ))
	{
		std::string message ("Failed to convert int to wstring");
		throw std::exception(message.c_str()) ;
	}

	return wstring( buf ) ;
}


inline tstring int2tstring( const int i, const int radix = 10 )
{
#ifdef _UNICODE
	return int2wstring( i, radix ) ;
#else
	return int2string( i, radix ) ;
#endif
}


inline wstring ulong2wstring( const unsigned long i, const int radix = 10 )
{
	wchar_t buff[MAX_NUM_STRLEN] ;

	if (EINVAL == _ultow_s( i, buff, MAX_NUM_STRLEN, radix ))
	{
		std::string message ("Failed to convert ulong to string");
		throw std::exception(message.c_str()) ;
	}

	return wstring(buff) ;
}

inline string ulong2string( const unsigned long i, const int radix = 10 )
{
	return string2string(ulong2wstring(i, radix)) ;
}

inline wstring ulong2formatted_wstring( const unsigned long i )
{
	wchar_t out_buffer[MAX_NUM_STRLEN] = {0} ;

	const size_t len = GetNumberFormatW( LOCALE_USER_DEFAULT, 
										 LOCALE_NOUSEROVERRIDE, 
										 ulong2wstring(i).c_str(), 
										 NULL, 
										 out_buffer, 
										 MAX_NUM_STRLEN) ;
	
	const wstring output_str(out_buffer, len) ;
	size_t decimal_pos = output_str.find_first_of( L'.' ) ;
	return output_str.substr( 0, decimal_pos ) ;
}


inline wstring double2wstring( const double d, const size_t precision = 5 )
{
	try
	{
		const wstring fmt_string = (wformat(L"%%1$.%1%lf") % precision).str() ;
		return (wformat(fmt_string) % d).str() ;
	}
	catch (std::exception &e)
	{
		std::string message = std::string("Failed to convert double to wstring: ") + e.what();
		throw std::exception(message.c_str()) ;
	}
}

inline string double2string( const double d, const size_t precision=5 )
{
	return string2string(double2wstring( d, precision )) ;
}


inline tstring double2tstring( const double d  )
{
#ifdef _UNICODE
	return double2wstring( d ) ;
#else
	return double2string( d ) ;
#endif
}

/*!
 * Convert a file size into a wstring, with correct unit.
 */
inline wstring file_size2wstring( const size_t file_size )
{
	if( file_size < ONEK )
	{
		return ulong2formatted_wstring( static_cast< unsigned long >( file_size )  ) + L" Bytes" ;
	}

	wstring desc ;
	_variant_t var ;

	if ( file_size < ONEM )
	{
		var = (double)file_size / (double)ONEK ;
		desc = L" KBytes" ;
	}
	else if ( file_size < ONEG )
	{
		var = (double)file_size / (double)ONEM ;
		desc = L" MBytes" ;
	}
	else 
	{
		var = (double)file_size / (double)ONEG ;
		desc = L" GBytes" ;
	}

	var.ChangeType( VT_BSTR, NULL ) ;
	_ASSERTE( var.vt == VT_BSTR ) ;

	const int BUFLEN = 50 ;
	TCHAR out_buffer[BUFLEN] = {0} ;
	const int len = GetNumberFormat( LOCALE_USER_DEFAULT, 
									 LOCALE_NOUSEROVERRIDE, 
									 ATL::CW2CT( var.bstrVal ), 
									 NULL, 
									 out_buffer, 
									 BUFLEN ) ;

	for (int i = len-2 ; i >= 0 ; --i)
	{
		switch(out_buffer[i])
		{
		case L'0':
			out_buffer[i] = 0 ;
			break ;
		case L'.':
			out_buffer[i] = 0 ;
		// fall through...
		default:
			return wstring((LPCWSTR)ATL::CT2W(out_buffer)) + desc ;
		}
	}
	return wstring((LPCWSTR)ATL::CT2W(out_buffer)) + desc ;
}
inline string file_size2string( const size_t file_size )
{
	return string2string(file_size2wstring(file_size)) ;
}
inline tstring ulong2tstring( const unsigned long i, const int radix = 10 )
{
#ifdef _UNICODE
	return ulong2wstring( i, radix ) ;
#else
	return ulong2string( i, radix ) ;
#endif
}



/*
 * 0.5 = 50%
 * 1.0 = 100%
 * etc.
 */
inline string double2percent_string( const double value, bool round = true )
{
	const double hundred_percent = 100.0 ;
	int percent_num = static_cast< int >( value * hundred_percent ) ;

	if ( round )
	{
		double non_cutoff = value * hundred_percent ;
		if ( ( non_cutoff - (double)percent_num ) >= 0.5 )
		{
			percent_num++ ;
		}
	}
	string percent ;
	percent += int2string( percent_num ) ;
	percent += "%" ;
	return percent ;
}

inline wstring double2percent_wstring( const double val, bool round = true )
{
	return string2wstring( double2percent_string( val, round ) ) ;
}



// conversion from resource id

#if _WIN32_IE > 0


inline tstring resource2tstring( const UINT uid, HINSTANCE resource_inst ) 
{
#ifdef _UNICODE
	return resource2wstring( uid, resource_inst ) ;
#else
	// Create the buffer
	str::buffer buff ;

	if ( 0 == ::LoadStringA( resource_inst, uid, buff.buffer( MAX_PATH ), MAX_PATH ) )
		return tstring() ;

	return buff.str() ;
#endif
}

inline wstring resource2wstring( const UINT uid, const HINSTANCE instance )
{
	const UINT block = (uid >> 4) + 1;   // Compute block number.
	const UINT num = uid & 0xf;      // Compute offset into block.
	
	HRSRC hRC = FindResourceEx( instance,
	 RT_STRING,
	 MAKEINTRESOURCE(block),
	 MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));

	_ASSERTE( hRC != NULL ) ;
	if ( hRC == NULL )
		return wstring() ;

	HGLOBAL hgl = LoadResource(instance, hRC);

	_ASSERTE( hgl != NULL ) ;
	if ( hgl == NULL )
		return wstring() ;

	LPWSTR res_str = (LPWSTR)LockResource(hgl);

	_ASSERTE( res_str != NULL ) ;
	if ( res_str == NULL )
		return wstring() ;

	for ( UINT i = 0; i < num; i++)
	{
		res_str += *res_str + 1;
	}

	return wstring( res_str + 1,  *res_str );
}

inline string resource2string( const UINT uid, HINSTANCE resource_inst )
{
	const tstring tres = resource2tstring( uid, resource_inst ) ;

	return string2string( tres ) ;
}

#define R2S( id )    resource2string ( id, _Module.GetResourceInstance() )
#define R2STR( id )  resource2string ( id, _Module.GetResourceInstance() ) 
#define R2WSTR( id ) resource2wstring( id, _Module.GetResourceInstance() ) 
#define R2TSTR( id ) resource2tstring( id, _Module.GetResourceInstance() ) 


#endif // #if _WIN32_IE > 0

inline string escape_char( BYTE the_char )
{
	const int hex_radint = 16 ;
	// 0x1A -> \'1A
	string escaped_char = "\\'" ;
	const string hex_rep = int2string( the_char, hex_radint ) ;
	if ( hex_rep.empty() )
		escaped_char += "0" ;
	if ( hex_rep.length() < 2 )
		escaped_char += "0" ;
	escaped_char += hex_rep ;
	// \'1A -> \'1a
	boost::to_lower( escaped_char ) ;
	return escaped_char ;
}

inline string escape_string( const string &str )
{
	string out ;

	foreach( char c, str )
	{
		const BYTE the_char = static_cast<BYTE>(c) ;

		if ( the_char < 0x20 )
			out += escape_char( the_char ) ;
		else if ( 0x80 <= the_char && the_char <= 0xFF )
			out += escape_char( the_char ) ;
		else switch ( the_char )		
		{
			case 0x5C:
			case 0x7B:
			case 0x7D:
				out += escape_char( the_char ) ; 
				break ;
			default:
				out += c ;
		}
	}
	return out ;
}

inline string escape_string( const wstring &str )
{
	return escape_string( string2string( str ) ) ;
}

inline string unescape_string( const string &str )
{
	textstream_reader< char > reader ;
	reader.set_buffer( str.c_str() ) ;

	string output ;
	string chunk ;

	while( reader.peek() != char(0) )
	{
		if ( reader.peek() == '\\' )
		{
			reader.advance() ;
			if ( reader.peek() == '\'' )
			{
				reader.advance() ;
				reader.get( chunk, 2 ) ;
				output += char( string2long( chunk, 16 ) ) ;
				chunk.erase() ;
			}
			else
			{
				output += '\\' ;
				reader.get( output ) ;
			}
		}
		else
		{
			reader.get( output ) ;
		}
	}
	return  output ;
}