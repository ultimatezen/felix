/*!
	@file resource_string.h
	@brief Helper functions for resource strings
	@date 2004/12/28
	Time: 22:34:51
	@author Ryan Ginstrom
 */

#pragma once

#include "atlstr.h"

ATL::CStringA resource_string_a( UINT id ) ;
ATL::CStringW resource_string_w( UINT id ) ;

#ifdef _UNICODE
// return a CStringW
#define resource_string resource_string_w
#else
// return a CStringA
#define resource_string resource_string_a
#endif

#define R2T( id ) static_cast< LPCTSTR >( resource_string( id ) ) 
#define R2A( id ) static_cast< LPCSTR >( resource_string_a( id ) ) 
#define R2W( id ) static_cast< LPCWSTR >( resource_string_w( id ) ) 

#define R2TS( id ) resource_string( id )
#define R2AS( id ) resource_string_a( id )
#define R2WS( id ) resource_string_w( id )


