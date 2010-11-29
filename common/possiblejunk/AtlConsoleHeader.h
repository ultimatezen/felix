#if !defined(ATL_CONSOLE_HEADER_H__3593B001_0CE6_48D1_B5C7_0690EB49D352__INCLUDED_)
#define ATL_CONSOLE_HEADER_H__3593B001_0CE6_48D1_B5C7_0690EB49D352__INCLUDED_

#pragma once

// Change these values to use different versions
#ifndef WINVER
#define WINVER			0x0400
// #define _WIN32_WINNT	0x0400
#define _WIN32_IE		0x0501  // we need 5 or higher anyway...
#define _RICHEDIT_VER	0x0300 // richedit2 and above has the unicode version
#endif

// no multithreading, thanks!
#define _ATL_APARTMENT_THREADED

#define DO_EFFICIENT_STRING_CONVERSIONS // this keeps static buffers around

// WTL 7 handlers
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

// demo def
#ifdef _DEBUG
#define	MAX_MEMORY_SIZE_FOR_DEMO 10000
#else
#define	MAX_MEMORY_SIZE_FOR_DEMO 150
#endif

// trace defs
#ifdef ATL_TRACE_LEVEL
#undef ATL_TRACE_LEVEL
#endif

#ifdef _DEBUG
#define ATL_TRACE_LEVEL 3
//	#define _ATL_DEBUG_QI
//	#define _ATL_DEBUG_INTERFACES 
#endif 

//#define SHOW_FUCKED_UP_MESSAGES

#ifdef SHOW_FUCKED_UP_MESSAGES
#define FUCKED_UP_MESSAGE( foo ) MessageBox( _T(foo), _T("Fucked Up Message"), MB_ICONEXCLAMATION | MB_OK )
#else
#define FUCKED_UP_MESSAGE( foo ) (void)0
#endif

// #include "AggressiveOptimize.h"

#include "CStringInterface.h"

#include <string>
using std::string ;
using std::wstring ;
typedef std::basic_string< TCHAR > tstring ;

#pragma warning( disable:4711 ) // function 'function' selected for inline expansion

#include <atlbase.h>

#include "LocalizedServerAppModule.h"
extern CLocalizedServerAppModule _Module;

// This is here only to tell VC7 Class Wizard this is an ATL project
#ifdef ___VC7_CLWIZ_ONLY___
CComModule
CExeModule
#endif

#pragma warning(disable:4995) // deprecated 
#pragma warning(disable:4100) // unreferenced formal parameter 
#pragma warning(disable:4244) // conversion from 'int' to 'unsigned short', possible loss of data
#pragma warning(disable:4189) // ocal variable is initialized but not referenced 
#include <atlcom.h>	// yous com bubbies is bad boys!
#pragma warning(default:4995) // deprecated 
#pragma warning(default:4100) // unreferenced formal parameter 
#pragma warning(default:4244) // conversion from 'int' to 'unsigned short', possible loss of data
#pragma warning(default:4189) // local variable is initialized but not referenced 


#ifndef _WORD_ADDIN

#include <atlhost.h>
#include <atlwin.h>


#include <commctrl.h> // LPNMREBARCHEVRON
#include <prsht.h> // PSW_HWNDTOINDEX

#pragma warning( disable:4995 ) // #pragma deprecated
#include <atlframe.h>
#pragma warning( default:4995 ) // #pragma deprecated

#include <atlapp.h>			// must be included before atlctrls.h
#include <atlctrls.h>
#include <atldlgs.h>

#pragma warning( disable:4995 ) // #pragma deprecated
#include <atlctrlw.h>
#pragma warning( default:4995 ) // #pragma deprecated

#endif

#define SEND_BUTTON_ID					17
#define DONT_SEND_BUTTON_ID			18
#define CRASH_PAGE_ID					19
#define ERROR_TITLE_ID					57672

// use unicode version of RTF
#undef RICHEDIT_CLASS
#define RICHEDIT_CLASS RICHEDIT_CLASSW

#ifndef FLOAT_EQ
#define EPSILON 0.00001
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#endif 

/*
INCLUDE THIS IN CPP FILE:

	#include "Exceptions.h"

	#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
	#endif

	CServerAppModule _Module;

	int main(int argc, char* argv[])
	{
		// convert structured exceptions into c++ exceptions
		_set_se_translator(TranslateSEHtoCE) ;
		// memory debugging
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ) ;
		BANNER( "Launching App" ) ;
*/

#endif
