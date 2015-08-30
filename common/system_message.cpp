#include "StdAfx.h"
#include ".\system_message.h"
#include "logging.h"
#include "NumberFmt.h"

CStringA int_arg_a( int arg, bool format )
{
	if (!format)
	{
		return CStringA(boost::lexical_cast<string>(arg).c_str()) ;
	}
	CNumberFmt fmt ;
	const CStringA m_arg = CT2CA( fmt.Format( arg ) ) ;
	return m_arg ;
}

CStringW int_arg_w( int arg, bool format )
{
	if (!format)
	{
		return CStringW(boost::lexical_cast<wstring>(arg).c_str()) ;
	}
	CNumberFmt fmt ;
	const CStringW m_arg = CT2CW( fmt.Format( arg ) ) ;
	return m_arg ;
}


CStringA system_message_a( UINT id, ... )
{
	CStringA formatted_string ;

	// get format string from string table
	CStringA strFormat ;
	if( ! strFormat.LoadString( id ) )
	{
		logging::log_warn("Failed to load format string " + int2string(id)) ;
	}

	ATLASSERT( strFormat.GetLength() > 0 ) ;
	if(strFormat.GetLength() == 0)
	{
		return formatted_string ;
	}

	va_list argList;
	va_start( argList, id );

	// format message into temporary buffer pszTemp
	CHeapPtr< char, CLocalAllocator > pszTemp;
	ATLVERIFY( 0 != ::FormatMessageA( FORMAT_MESSAGE_FROM_STRING|	FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		strFormat, 0, 0, reinterpret_cast< LPSTR >( &pszTemp ), 0, &argList ) ) ;

	formatted_string = pszTemp;

	va_end( argList );

	return formatted_string ;
}

CStringW system_message_w( UINT id, ... )
{
	CStringW formatted_string ;

	// get format string from string table
	CStringW strFormat ;
	if( ! strFormat.LoadString( id ) )
	{
		logging::log_warn("Failed to load format string " + int2string(id)) ;
	}

	ATLASSERT( strFormat.GetLength() > 0 ) ;
	if(strFormat.GetLength() == 0)
	{
		return formatted_string ;
	}

	va_list argList;
	va_start( argList, id );

	// format message into temporary buffer pszTemp
	LPWSTR format_str = strFormat.GetBuffer( ) ;
	CHeapPtr< wchar_t, CLocalAllocator > pszTemp;
	ATLVERIFY( 0 != ::FormatMessageW( FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		format_str, 0, 0, reinterpret_cast< LPWSTR >( &pszTemp ), 0, &argList ) ) ;

	formatted_string = pszTemp;

	va_end( argList );

	return formatted_string ;
}
