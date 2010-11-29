/*!
destination_parser
*/

#pragma once

#include "StringEx.h"

#include "exdispid.h"
#include "StringConversions.h"
#include "logging.h"

/**
	@class destination_parser  
	@brief Parses an href in NUM::NUM format into a command for the frame window.
 */
class destination_parser  
{
	MSG				m_message ;

public:

	// Function name	: parse
	// Description	    : 
	// Return type		: LPMSG 
	// Argument         : wstring &destination
	LPMSG parse( const wstring destination )
	{
		ZeroMemory( &m_message, sizeof( MSG ) ) ;

		// check formatting
		const size_t colon_pos = destination.find( wchar_t(L':') ) ;
		
		if ( colon_pos == wstring::npos ) // not found
		{
			logging::log_warn( "destination_parser: no colon found -- this is not in a format we can parse." ) ;
			return NULL ;
		}

		// get wparam
		const wstring category = destination.substr( 0, colon_pos ) ;
		if ( false == str::is_uint_rep( category ) ) // is it a number?		 
		{
			logging::log_warn( "destination_parser: Category is not a number" ) ;
			return NULL ;
		}

		m_message.wParam = string2ulong( category ) ;

		// get lparam
		const wstring sub_category = destination.substr( colon_pos+1 ) ;
		if ( ! str::is_uint_rep( sub_category ) ) // is it a number?
		{
			ATLASSERT( ::PathIsURL( CW2CT( destination.c_str() ) ) || ::PathFileExists( CW2CT( destination.c_str() ) ) );
			WRITE_TABS ; ATLTRACE( "Sub-category is not a number" ) ;
			return NULL ;
		}

		m_message.lParam = string2ulong( sub_category ) ;
		return &m_message ;
	}
};


