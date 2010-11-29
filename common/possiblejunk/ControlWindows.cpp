// ControlWindows.cpp: implementation of the ControlWindows class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ControlWindows.h"
#include "Exceptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

#pragma warning( disable:4239 ) //  reference that is not to 'const' cannot be bound to a non-lvalue

//// MEditStreamInCallback
//
//// Function name	: MEditStreamInCallback
//// Description	    : 
//// Return type		: DWORD __stdcall 
//// Argument         : DWORD dwCookie
//// Argument         : LPBYTE pbBuff
//// Argument         : LONG cb
//// Argument         : LONG *pcb
//DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
//{
//	BANNER( "MEditStreamInCallback" ) ;
//
//	wstring *psBuffer = (wstring *)dwCookie;
//
//	ATLASSERT( psBuffer != NULL ) ;
//
//	if ( static_cast<size_t>(cb) > ( psBuffer->length() * sizeof( wchar_t ) ) )
//		cb = psBuffer->length() * sizeof( wchar_t ) ;
//
//	if ( cb % 2 != 0 ) --cb ;
//	if ( cb != 0 ) 
//	{
//		memcpy( pbBuff, psBuffer->substr(0, cb / sizeof( wchar_t ) ).c_str(), cb ) ;		
//	}
//
//	*pcb = cb;
//
//	psBuffer->erase(0, cb / sizeof( wchar_t ) ) ;
//
//	return 0;
//}
//
//// MEditStreamInCallback
//
//// Function name	: MEditRichStreamInCallback
//// Description	    : 
//// Return type		: DWORD __stdcall 
//// Argument         : DWORD dwCookie
//// Argument         : LPBYTE pbBuff
//// Argument         : LONG cb
//// Argument         : LONG *pcb
//DWORD __stdcall MEditRichStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
//{
//	BANNER( "MEditRichStreamInCallback" ) ;
//
//	string *psBuffer = (string *)dwCookie;
//
//	ATLASSERT( psBuffer != NULL ) ;
//
//	if ( static_cast<size_t>(cb) > ( psBuffer->length()  ) )
//		cb = psBuffer->length()  ;
//
//	memcpy( pbBuff, psBuffer->substr(0, cb ).c_str(), cb ) ;		
//
//	*pcb = cb;
//
//	psBuffer->erase(0, cb ) ;
//
//	return 0;
//}
//
//// MEditStreamOutCallback
//
//// Function name	: MEditStreamOutCallback
//// Description	    : 
//// Return type		: DWORD __stdcall 
//// Argument         : DWORD dwCookie
//// Argument         : LPBYTE pbBuff
//// Argument         : LONG cb
//// Argument         : LONG *pcb
//DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
//{
//	BANNER( "MEditStreamOutCallback" ) ;
//
//	wstring *psBuffer = (wstring *)dwCookie;
//
//	psBuffer->append( (const wchar_t*)pbBuff, cb / sizeof( wchar_t ) ) ;
//
//	*pcb = cb ;
//	return 0;
//}

#pragma warning( default:4239 ) //  reference that is not to 'const' cannot be bound to a non-lvalue

