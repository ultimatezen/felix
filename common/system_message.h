#pragma once

#include "resource_string.h"
#include "atlstr.h"

CStringA int_arg_a( int arg, bool format=true ) ;
CStringW int_arg_w( int arg, bool format=true ) ;


CStringA system_message_a( UINT id, ... ) ;

CStringW system_message_w( UINT id, ... ) ;

#ifdef _UNICODE

	#define int_arg				int_arg_w 
	#define system_message		system_message_w

#else

	#define int_arg				int_arg_a 
	#define system_message		system_message_a

#endif

#define INT_ARG( x ) (LPCTSTR)int_arg( x ) ;
