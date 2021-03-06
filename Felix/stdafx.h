﻿/*! @mainpage

@section intro Introduction

Felix is the main program in the Assistant Suite; most of the other programs 
in the suite use this one as an automation server. Felix appears as a GUI
window managing/displaying the translation memories and glossaries, performing
lookup, etc.

Clients, such as the MS Office add-ins, call to Felix as a COM server, sending
queries, getting translations, etc.

Felix can also support remote TMs by interacting with Memory Serves (via HTTP).

@section details Details

Felix is a translation memory program.

Felix implements an automation server, and interacts with several application
interfaces and stand-alone applications.

The main GUI window class is MemoryWindowFrame. This implements the Felix window. The GlossaryWindowFrame class 
implements the glossary windows.

The main COM server class is CApp, which implements the "Felix.App" COM server.

The program's entry point is _tWinMain().

See README for dependencies.
*/

#pragma once

#pragma warning( disable : 4996 ) // #conditional expression is constant
#pragma warning(disable : 4520)

#define NOMINMAX
#include <algorithm>    // std::min
using std::min ;
using std::max ;

// This is to keep the compiler from complaining about insecure code in the STL (as if I can do anything about it!)
#define _SCL_SECURE_NO_WARNINGS

// Change these values to use different versions
#define WINVER			0x0600  // Windows Vista
#define _WINVER			0x0600  // Windows Vista
#define _WIN32_IE		0x0600  // we need 6 or higher...
#define WIN32_IE		0x0600  // we need 6 or higher...
#define _RICHEDIT_VER	0x0300  // richedit2 and above has the unicode version

#pragma warning( disable : 4127 ) // #conditional expression is constant
#pragma warning( disable : 4267 ) // 'size_t' から 'DWORD' に変換しました。データが失われているかもしれません。
#pragma warning( disable : 4503 ) // 装飾された名前の長さが限界を超えました。名前は切り捨てられます。
// #pragma warning( disable : 4995 ) // #pragma deprecated

#pragma warning( disable : 4510 ) // 'class' : default constructor could not be generated
#pragma warning( disable : 4610 ) // object 'class' can never be instantiated - user-defined constructor required


// new defines for Atl7
#define _ATL_ALL_WARNINGS

#define _CRTDBG_MAP_ALLOC

// no multithreading, thanks!
#define _ATL_APARTMENT_THREADED

// WTL 7 handlers for property sheets
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

// demo def
#ifndef UNIT_TEST
static const size_t MAX_MEMORY_SIZE_FOR_DEMO = 500 ;
#else
static const size_t MAX_MEMORY_SIZE_FOR_DEMO = 10 ;
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

#include <lua.hpp>

#include <Windows.h>
#include <commdlg.h>

#include <comdef.h>
#include "resource.h"

#include "cpptempl.h"

//////////////////////////////////////////////////////////////////////////
// stl libraries we are using
//////////////////////////////////////////////////////////////////////////

#include <map>							// for record data
#include <set>							
#include <list>
#include <vector>
#include <string>
#include <algorithm> 
#include <iterator>
#include <stack>
#include <exception>			// for exceptions thrown by STL

using std::wstring ;
using std::string ;

/*
	We need to disable this warning because boost::assign causes it.
*/
#pragma warning( disable : 4512 ) // assignment operator could not be generated

//////////////////////////////////////////////////////////////////////////
// boost
//////////////////////////////////////////////////////////////////////////
#pragma warning( disable : 6334 )

#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <boost/assign/std/set.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign; // bring 'operator+=()' into scope

#include <boost/format.hpp>
using boost::format;
using boost::wformat ;
typedef boost::basic_format< TCHAR > tformat;

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_array.hpp>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/regex.hpp>

#include <boost/foreach.hpp>
using namespace boost::foreach ;
#define FOREACH BOOST_FOREACH

#pragma warning( disable : 4701 ) // 初期化されていない可能性のあるローカル変数 'result' が使用されます
#include <boost/lexical_cast.hpp>
using boost::lexical_cast ;
#pragma warning( default : 4701 )

#pragma warning( disable : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch
#include <boost/filesystem.hpp>   // includes all needed Boost.Filesystem declarations
#pragma warning( default : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch

namespace fs = boost::filesystem;

#include <boost/static_assert.hpp>

#include <boost/range/adaptor/map.hpp>
#include <boost/flyweight.hpp>

#include <boost/timer.hpp>
#pragma warning( default : 6334 )


//////////////////////////////////////////////////////////////////////////
// WTL Headers
//////////////////////////////////////////////////////////////////////////

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#define _WTL_NO_UNION_CLASSES

#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>
#include <atltypes.h>
#include <atlstr.h>

#pragma warning( disable : 4996 ) 
#include <atlapp.h>		// must be included before atlctrls.h
#pragma warning( default : 4996 ) 
#include <atlframe.h>
#include <AtlMisc.h>
#include <atlctrls.h>
#include <atlcrack.h>
#include <ATLCTRLX.H>

#include <Mshtml.h>
// shell API support
#include <shellapi.h>
#include <shlobj.h>

#include "DebugUtilities.h"
#include "StringEx.h"

#include "stringconversions.h"
#include "textstream_reader.h"

#include "LocalizedServerAppModule.h"

extern CLocalizedServerAppModule _Module;

/*!
 * \hack
 * So we can use CLocalizer
 */
#define _AtlModule _Module 



#ifdef UNIT_TEST
	#define DECLARE_SENSING_VAR std::vector<std::string> m_sensing_variable
	#define SENSE(x) m_sensing_variable.push_back(std::string(x))
	#define VISIBLE_TO_TESTS public:
#else
	#define DECLARE_SENSING_VAR
	#define SENSE(x) (void)0
	#define VISIBLE_TO_TESTS
#endif

#include <atlgdix.h>				// must be included before CustomTabCtrl.h
#include <atlddx.h> 
#include "atlscintilla.h"
#include <atldlgs.h>

#pragma warning( disable:4995 ) // #pragma deprecated
#include <atlctrlw.h>
#pragma warning( default:4995 ) // #pragma deprecated

#pragma warning( disable:4711 ) // function 'function' selected for inline expansion

// This is here only to tell VC7 Class Wizard this is an ATL project
#ifdef ___VC7_CLWIZ_ONLY___
CComModule
CExeModule
#endif

#include <atlcom.h>	// yous com bubbies is bad boys!
#include <atlhost.h>

#include <commctrl.h> // LPNMREBARCHEVRON
#include <prsht.h> // PSW_HWNDTOINDEX
#include "comutil.h"

#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")


#pragma warning( disable:4995 ) // #pragma deprecated
#include <atlframe.h>
#pragma warning( default:4995 ) // #pragma deprecated

#include "resource_string.h"
#include "stringex.h"
#include "ColorRef.h"

#include "xpmenu/MenuXP.h"
#include "xpmenu/ToolBarXP.h"
#include "xpmenu/Tools.h"

// We need to rename the functions below, because they clash
// with our namespace. If only we could pull these defs into their
// own namespace...
#import "mshtml.tlb" no_auto_exclude, \
		named_guids, \
		rename( "TranslateAccelerator", "HtmlTranslateAccelerator" ), \
		rename("min", "HtmlMin"), \
		rename("max", "HtmlMax")

#include "mshtml.h"
#include "pugxml/src/pugixml.hpp"

#include "unit_testing.h"

static const int SEND_BUTTON_ID = 17 ;
static const int DONT_SEND_BUTTON_ID = 18 ;
static const int CRASH_PAGE_ID = 19 ;
static const int ERROR_TITLE_ID = 57672 ;

#include "command_edit.h"

#ifndef FLOAT_EQ
static const double EPSILON = 0.00001 ;
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#endif 

#define FELIX_APPLICATION
static const char* VERSION = "1.7.3" ;
static const wchar_t* LOGGING_SERVER = L"FelixHelpers.FelixLogger" ;
#define LOGFILE_NAME _T("felix.log")
 

#ifdef _EMBEDDED_MANIFEST
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif // _EMBEDDED_MANIFEST


