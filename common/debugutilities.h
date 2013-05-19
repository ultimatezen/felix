/*!
	@file DebugUtilities.h
	@brief various typedefs and functions useful for debugging
	@date 2004/12/16
	@time 10:31:28
	@author Ryan Ginstrom
 */
#pragma once

#include <cstdlib>			// define memory allocation functions


inline CString FormatWinError(DWORD error)
{
	LPVOID buff(0)  ;
	if ( 0 == 
		::FormatMessage
		(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM ,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &buff,
		0,
		NULL 
		)
		)
	{
		if ( buff != NULL )	::LocalFree( buff ) ;
		return CString( _T("Unknown Error Type") ) ;
	}
	ATLASSERT( buff != NULL ) ;
	CString msg( static_cast< LPCTSTR >( buff ) ) ;
	::LocalFree( buff ) ;

	return msg ;
}


#ifdef _DEBUG 

#include <crtdbg.h>		// redefine memory allocation functions
#include "comdef.h"
#pragma warning( disable:4310 ) // cast truncates constant value

/*! Checks for memory leaks.

Making it a module-level variable ensures that it sets the leak-check flags 
before other program code runs.
 */
class CLeakChecker

{
public:
	CLeakChecker()
	{	
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
	}
} ;


	// =================
	// tabs to prettify our debug output
	// =================
	inline int global_tab_count( int flag )
	{
		static int tab_count = -1 ;

		switch( flag )
		{
		case 0: return tab_count ;
		case -1: return --tab_count ;
		default: return ++tab_count ;
		}
	}
	struct tab_counter
	{
		tab_counter() { global_tab_count( 1 ) ; }
		~tab_counter() { global_tab_count( -1 ) ; }
	} ;

	#define WRITE_TABS	do { for ( int _num_tabs=0 ; _num_tabs<global_tab_count(0) ; ++_num_tabs ) ATLTRACE( "\t" ) ; } while (0) ;

	// =================
	// to_cstr
	// std::string conversion
	// =================
	inline const char *to_cstr( UINT id )
	{
		ATL::CString msg ;
		ATLVERIFY(msg.LoadString( id )) ;
		return ATL::CT2A( static_cast< LPCTSTR >( msg ) ) ;
	}
	// Function name	: char *to_cstr
	// Description	    : 
	// Return type		: inline const 
	// Argument         : LPCSTR s
	inline const char *to_cstr( LPCSTR s )
	{

		return s ;
	}

	// Function name	: char *to_cstr
	// Description	    : 
	// Return type		: inline const 
	// Argument         : const std::string &s
	inline const char *to_cstr( const std::string &s )
	{
		return s.c_str() ;
	}

	// Function name	: char *to_cstr
	// Description	    : 
	// Return type		: inline const 
	// Argument         : const std::string &s
	inline const char *to_cstr( const std::wstring &s )
	{
		return ATL::CW2A( s.c_str() ) ;
	}

	// Function name	: char *to_cstr
	// Description	    : 
	// Return type		: inline const 
	// Argument         : LPCWSTR s
	inline const char *to_cstr( LPCWSTR s )
	{
		return ATL::CW2A( s ) ;
	}

	// Function name	: char *to_cstr
	// Description	    : 
	// Return type		: inline const 
	// Argument         : const BSTR s
	inline const char *to_cstr( const BSTR s )
	{
		if ( s == NULL ) return "" ;
		return ATL::CW2A( s ) ;
	}
	// Function name	: TraceWinError
	// Description	    : 
	// Return type		: inline void 
	// Argument         : DWORD error = ::GetLastError()
	inline void TraceWinError( DWORD error, LPCTSTR file_name, int line )
	{
		if ( error == ERROR_SUCCESS )
		{
			ATLTRACE( "%s(%d): %d (ERROR_SUCCESS)\n", file_name, line, error ) ;
			return ;
		}

		CString message = FormatWinError(error) ;
		ATLTRACE( "%s(%d): %d (%s)\n", file_name, line, error, message ) ;
	}
	// trace routines

	// Function name	: TraceHresult 
	// Description	    : 
	// Return type		: inline void 
	// Argument         : HRESULT hr
	inline void TraceHresult ( HRESULT hr, LPCTSTR file_name, int line )
	{
		if ( hr == S_OK )
		{
			ATLTRACE( "%s(%d): 0x%0X (S_OK)\n", file_name, line, hr ) ;
			return ;
		}
		if ( hr == S_FALSE ) 
		{
			ATLTRACE( "%s(%d): 0x%0X (FALSE)\n", file_name, line, hr ) ;
			return ;
		}
		if ( SUCCEEDED( hr ) )
		{
			ATLTRACE( "%s(%d): 0x%0X (SUCCESS)\n", file_name, line, hr ) ;
			return ;
		}

		ATLTRACE( "\n" ) ;
		_com_error e(hr) ;
		ATLTRACE( _T("%s(%d): 0x%0X %s\n"), file_name, line, hr, (TCHAR *)e.ErrorMessage() ) ;
		::Sleep( 50 ) ; // why?
	}
	// ==================
	// VariableTracer
	// ==================
	inline void CharTracer( const char c )
	{
		char str[2] ;
		str[0] = c ;
		str[1] = 0 ;
		OutputDebugStringA( str ) ;
		ATLTRACE( " (%d)", (int)c ) ;
	}
	inline void CharTracer( const unsigned char c )
	{
		unsigned char str[2] ;
		str[0] = c ;
		str[1] = 0 ;
		OutputDebugStringA( (char*)str ) ;
		ATLTRACE( " (%d)", (int)c ) ;
	}
	inline void CharTracer( const wchar_t c )
	{
		wchar_t str[2] ;
		str[0] = c ;
		str[1] = 0 ;
		OutputDebugStringW( str ) ;
		ATLTRACE( " (%d)", (int)c ) ;
	}
	inline void CharTracer( const wchar_t *c )
	{
		if ( c == NULL )
		{
			ATLTRACE( "NULL" ) ;
			return ;
		}
		wchar_t str[2] ;
		str[0] = *c ;
		str[1] = 0 ;
		OutputDebugStringW( str ) ;
		ATLTRACE( " (%d)", (int)*c ) ;
	}
	inline void CharTracer( const char *c )
	{
		if ( c == NULL )
		{
			ATLTRACE( "NULL" ) ;
			return ;
		}
		const char *next_pos = CharNextA( c ) ;
		std::string next_char( c, next_pos ) ;
		ATLTRACE( "%s", next_char.c_str() ) ;
		if ( next_char.length() == 1 )
			ATLTRACE( " (%d)", (int)next_char[0] ) ;
		else if ( next_char.length() == 2 )
			ATLTRACE( " (%d, %d)", (int)next_char[0], (int)next_char[1] ) ;
	}
	inline void VariableTracer( const RECT &v )
	{ 
		ATLTRACE("{ left=%d, top=%d, right=%d, bottom=%d }", v.left, v.top, v.right, v.bottom ) ; 
	}

	inline void VariableTracer ( const POINT &v ) 
	{ 
		ATLTRACE("{ x=%d, y=%d }", v.x, v.y  ) ; 
	}
	inline void VariableTracer ( const VARIANT_BOOL v ) 
	{ 
		ATLTRACE("%s", ( v == VARIANT_TRUE ? "TRUE" : "FALSE" ) ) ; 
	}
	inline void VariableTracer ( const bool v ) 
	{ 
		ATLTRACE("%s", ( v == false ? "false" : "true" ) ) ; 
	}
	inline void VariableTracer ( const int v ) 
	{ 
		ATLTRACE("%d", v ) ; 
	}
	inline void VariableTracer ( const unsigned long v ) 
	{ 
		ATLTRACE("%d", v ) ; 
	}
	inline void VariableTracer ( const size_t v ) 
	{ 
		ATLTRACE("%d", v ) ; 
	}
	inline void VariableTracer ( const double v ) 
	{ 
		ATLTRACE("%lf", v ) ; 
	}
	// the following use OutputDebugString to be able to handle long output
	inline void VariableTracer ( LPCSTR  v ) 
	{ 
		if ( v == NULL )
			OutputDebugStringA( "NULL" ) ;
		else
			OutputDebugStringA( v ) ;
	}
	inline void VariableTracer ( const std::string &v ) 
	{ 
		if ( v.empty() )
			OutputDebugStringA( """" ) ;
		else
			OutputDebugStringA( v.c_str() ) ;
	}
	inline void VariableTracer ( const std::wstring &v )  
	{ 
		if ( v.empty() )
			OutputDebugStringA( """" ) ;
		else
			OutputDebugStringW( v.c_str() ) ;
	}
	inline void VariableTracer ( const _bstr_t &v ) 
	{ 
		OutputDebugStringW( static_cast<LPCWSTR>(v) ) ;
	}
	inline void VariableTracer ( const ATL::CComBSTR &v ) 
	{ 
		OutputDebugStringW( static_cast<LPCWSTR>(v) ) ;
	}
	inline void VariableTracer ( const BSTR v ) 
	{ 
		if ( ::SysStringLen( v ) > 0 ) 
		{
			OutputDebugStringW( static_cast<LPCWSTR>(v) ) ;
		}
	}
	inline void VariableTracer ( LPCWSTR v ) 
	{ 
		if ( v == NULL )
			OutputDebugStringA( "NULL" ) ;
		else
			OutputDebugStringW( v ) ;
	}

	// trace macros
	#define TRACE_CHAR(x)									\
	do	{ WRITE_TABS ATLTRACE( "character " #x " = " ) ; CharTracer( x ) ; ATLTRACE( "\n" ) ; }	while(0) ;												

	#define TRACE(x)										\
	do														\
	{														\
		ATLTRACE( "TRACE: [" #x "] = " ) ;		\
		VariableTracer( x ) ; ATLTRACE( "\n" ) ;			\
	}														\
	while(0)

	#define TRACE_LABEL(x)									\
	do														\
	{														\
		ATLTRACE( "TRACE: [" #x "] = " ) ;		\
		VariableTracer( x.as_wstring() ) ;					\
		ATLTRACE( "\n" ) ;									\
	}														\
	while(0)


#define BANNER(x) ATLTRACE("\n*************************\n%s(%d): %s\n*************************\n", __FILE__, __LINE__, x ) 

	#define TRACE_HRESULT( x ) TraceHresult( x, _T(__FILE__), __LINE__ ) ;
	
	#define ASSERT_HRESULT( x ) if ( FAILED( x ) ) { TraceHresult( x, _T(__FILE__), __LINE__ ) ; ATLASSERT( SUCCEEDED( x ) ) ; }
	#define CLEAR_WINERRORS		::SetLastError( ERROR_SUCCESS ) 

#define WINERROR_SENTINEL \
	{ \
		DWORD _err = ::GetLastError() ; \
		if ( _err != ERROR_SUCCESS ) \
		{ \
			TraceWinError( _err, _T(__FILE__), __LINE__ ) ; \
			ATLASSERT( _err == ERROR_SUCCESS ) ; \
			SetLastError( ERROR_SUCCESS ) ; \
		} \
	}
												
#define _HR0( x ) do { HRESULT _hr = (x) ;		\
		ASSERT_HRESULT( _hr ) ;	} while (0)


#define TRACE_WINERR( x ) { if ( x != ERROR_SUCCESS ) TraceWinError( x, _T(__FILE__), __LINE__ ) ; }
	
#define ASSERT_WITH_WINERR( x ) \
{ \
	bool _expr = !! ( x ) ; \
	if ( ! _expr ) \
	{ \
		TraceWinError( GetLastError(), _T(__FILE__), __LINE__ ) ; \
		ATLASSERT( _expr ) ; \
	} \
}

#define NOT_IMPLEMENTED( x )	MessageBox(NULL, _T(x), _T("Not implemented"), MB_OK )

#else // _DEBUG not defined

#ifdef _DEBUG
	#error We have made a mistake with our ifdef's
#endif

	#define NOT_IMPLEMENTED( x )			(void) 0
	#define TRACE_CHAR( x )					(void) 0
	#define TRACE( x )						(void) 0
	#define TRACE_LABEL(x)					(void) 0
	#define BANNER( x )						(void) 0

	#define TRACE_HRESULT( x )				(void) 0
	#define to_cstr( x )					#x

	#define CLEAR_WINERRORS					(void) 0
	#define WINERROR_SENTINEL				(void) 0

	#define WRITE_TABS						(void) 0 ;
	#define TRACE_WINERR( x )				(void) 0
	#define ASSERT_WITH_WINERR(x)			(void) 0
	#define IS_VALID_POINTER( x )			(void) 0
	#define IS_VALID_READ_POINTER( x )		(void) 0
	#define CHECK_POINTER( x )				(void) 0
	#define CHECK_ARRAY( x, y )				(void) 0
	#define ASSERT_HRESULT( x )				(void) 0
	#define _HR0( x )						x ;

#endif // _DEBUG 

#define CASE( x ) case x : WRITE_TABS ATLTRACE( "case = " #x "\n" ) ;

#define ENSURE_FOCUS \
	if ( m_hWnd != pMsg->hwnd  && ! IsChild( pMsg->hwnd  ) ) return FALSE ;

#define ASSERT_WINERR ASSERT_WITH_WINERR

#define XCRASHREPORT_ERROR_LOG_FILE		_T("ERRORLOG.TXT")


#ifdef UNIT_TEST
	#define END_DLG  wID ; return 0L ;
#else
	#define END_DLG EndDialog(wID);return 0L ;
#endif

#pragma warning( default:4310 ) // cast truncates constant value
