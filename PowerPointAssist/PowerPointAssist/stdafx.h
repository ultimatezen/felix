// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#pragma warning( disable : 4267 ) // 'size_t' から 'DWORD' に変換しました。データが失われているかもしれません。
#pragma warning( disable : 4312 ) // 'reinterpret_cast' : 'LONG' からより大きいサイズの 'HINSTANCE' へ変換します。

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0600		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0600	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0600 // Change this to the appropriate value to target Windows Me or later.
#endif

#define _WIN32_IE		0x0600  // we need IE 6 or higher anyway...
#define WIN32_IE		0x0600  // we need IE 6 or higher anyway...


#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// WTL 7 handlers for property sheets
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include "resource.h"

// stl libraries we are using
#include <map>							// for record data
#include <set>							// multisets of tags
#include <list>
#include <vector>
#include <string>
#include <stack>
#include <exception>

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#define _WTL_NO_UNION_CLASSES

#include <atlbase.h>
#include <atlwin.h>
#include <atlcom.h>
#include <atltypes.h>
#include <atlstr.h>

#pragma warning( disable : 4996 ) 
#include "atlapp.h"
#pragma warning( default : 4996 ) 

#include <atlctrls.h> // for member controls

using namespace ATL ;

#include <comutil.h>
#include <atldlgs.h>
#include <shlobj.h>

#define POWERPOINT_ASSIST_APP

// boost
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope

#include <boost/format.hpp>
using boost::format;
using boost::wformat ;
typedef boost::basic_format< TCHAR > tformat;

#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>

#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/cast.hpp>

#pragma warning( disable : 4701 ) // 初期化されていない可能性のあるローカル変数 'result' が使用されます
#include <boost/lexical_cast.hpp>
#pragma warning( default : 4701 )
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <boost/foreach.hpp>
using namespace boost::foreach ;
#define FOREACH BOOST_FOREACH

#pragma warning( disable : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#pragma warning( default : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch

namespace fs = boost::filesystem;


#include "stringex.h"

#pragma warning( disable : 4278 )
#pragma warning( disable : 4146 )
//The following #import imports the IDTExtensibility2 interface based on its LIBID
#import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" version("1.0") lcid("0")  raw_interfaces_only named_guids

#pragma warning( default : 4146 )
#pragma warning( default : 4278 )

class DECLSPEC_UUID("828EC85D-73CA-4B7D-B98C-6287F543C894") PowerPointAssistLib;

using namespace ATL;

class CAddInModule : public CAtlDllModuleT< CAddInModule >
{
public:
	CAddInModule()
	{
		m_hInstance = NULL;
	}

	DECLARE_LIBID(__uuidof(PowerPointAssistLib))

	inline HINSTANCE GetResourceInstance()
	{
		return m_hInstance;
	}

	inline void SetResourceInstance(HINSTANCE hInstance)
	{
		m_hInstance = hInstance;
	}

private:
	HINSTANCE m_hInstance;
};

extern CAddInModule _AtlModule;


#include "File.h"
#include "CStringInterface.h"
#include "resource_string.h"


#ifndef FLOAT_EQ
#define EPSILON 0.00001
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#endif 

#ifdef UNIT_TEST
	#define DECLARE_SENSING_VAR std::vector<std::string> m_sensing_variable
	#define SENSE(x) m_sensing_variable.push_back(std::string(x))
	#define VISIBLE_TO_TESTS public:
#else
	#define DECLARE_SENSING_VAR
	#define SENSE(x) (void)0
	#define VISIBLE_TO_TESTS
#endif

#define VERSION "1.7"

#define LOGGING_SERVER L"FelixHelpers.PowerPointLogger"
#define LOGFILE_NAME _T("ppt_assist.log")

#include "logging.h"
