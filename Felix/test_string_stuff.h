#pragma once

#ifdef _DEBUG

#include "system_message.h"
#include "resource.h"

inline void test_system_message()
{
	ATLTRACE( "." ) ;

	ATLASSERT ( _T("Found 2 matches." ) == system_message( IDS_FOUND_X_MATCHES, int_arg( 2 ) ) ) ;
	ATLASSERT ( "Found 2 matches." == system_message_a( IDS_FOUND_X_MATCHES, int_arg_a( 2 ) ) ) ;
	ATLASSERT ( L"Found 2 matches." == system_message_w( IDS_FOUND_X_MATCHES, int_arg_w( 2 ) ) ) ;
}






inline void test_string_stuff()
{
	test_system_message() ;

}

#endif // #ifdef _DEBUG
