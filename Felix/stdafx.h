/*!
	@brief Precompiled header file for Felix
	@author Ryan Ginstrom
 */

#pragma once

// Change these values to use different versions
#define WINVER			0x0500  // Windows 2000
#define _WINVER			0x0500  // Windows 2000
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
#define DEBUG_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__) 

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


#include <Windows.h>
#include <commdlg.h>

#include <comdef.h>
#include "resource.h"

#include <atlstr.h>
#include <atltypes.h>
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#define _WTL_NO_UNION_CLASSES

// stl libraries we are using
#include <map>							// for record data
#include <set>							// multisets of tags
#include <list>
#include <vector>

// Loki
#include "loki/AssocVector.h"

// boost
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <boost/assign/std/set.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign; // bring 'operator+=()' into scope

#include <boost/format.hpp>
using boost::format;
using boost::wformat ;
typedef boost::basic_format< TCHAR > tformat;

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_array.hpp>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/foreach.hpp>
using namespace boost::foreach ;
#define foreach BOOST_FOREACH

#pragma warning( disable : 4701 ) // 初期化されていない可能性のあるローカル変数 'result' が使用されます
#include <boost/lexical_cast.hpp>
#pragma warning( default : 4701 )

#include <boost/spirit/include/phoenix1.hpp>

#pragma warning( disable : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#pragma warning( default : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch

namespace fs = boost::filesystem;

// WTL Headers
#include <atlbase.h>

#pragma warning( disable : 4996 ) 
#include "atlapp.h"
#pragma warning( default : 4996 ) 

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

#include <atlwin.h>

#ifdef UNIT_TEST
#include "FakeWindow.h"
#define TWindow CFakeWindow
#define DECLARE_SENSING_VAR std::vector<string> m_sensing_variable
#define SENSE(x) m_sensing_variable.push_back(string(x))
#define VISIBLE_TO_TESTS public:
#else
#define TWindow CWindow
#define DECLARE_SENSING_VAR
#define SENSE(x) (void)0
#define VISIBLE_TO_TESTS
#endif

#include <atlctrls.h>
#include <atlddx.h> 
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

#pragma warning( disable:4995 ) // #pragma deprecated
#include <atlframe.h>
#pragma warning( default:4995 ) // #pragma deprecated

#include "resource_string.h"

static const int SEND_BUTTON_ID = 17 ;
static const int DONT_SEND_BUTTON_ID = 18 ;
static const int CRASH_PAGE_ID = 19 ;
static const int ERROR_TITLE_ID = 57672 ;

// use Unicode version of RTF
#undef RICHEDIT_CLASS
#define RICHEDIT_CLASS RICHEDIT_CLASSW

#ifndef FLOAT_EQ
static const double EPSILON = 0.00001 ;
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#endif 

#define VERSION "1.5.5"
#define LOGGING_SERVER L"FelixHelpers.FelixLogger"
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

