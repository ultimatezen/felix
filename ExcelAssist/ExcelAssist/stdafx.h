/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

// Change these values to use different versions
#define WINVER			0x0500  // Windows 2000
#define _WINVER			0x0500  // Windows 2000
#define _WIN32_IE		0x0600  // we need 6 or higher...
#define WIN32_IE		0x0600  // we need 6 or higher...
#define _RICHEDIT_VER	0x0300  // richedit2 and above has the unicode version

#pragma warning( disable : 4267 ) // 'size_t' から 'DWORD' に変換しました。データが失われているかもしれません。
#pragma warning( disable : 4312 ) // 'reinterpret_cast' : 'LONG' からより大きいサイズの 'HINSTANCE' へ変換します。

#ifndef STRICT
#define STRICT
#endif

// new defines for Atl7
#define _ATL_ALL_WARNINGS
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

// WTL 7 handlers for property sheets
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

#define _CRTDBG_MAP_ALLOC
#define DEBUG_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__) 

#include "resource.h"

#include "cstringinterface.h"
#include <atltypes.h>
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#define _WTL_NO_UNION_CLASSES

// stl libraries we are using
#include <map>							// for record data
#include <set>							// multisets of tags
#include <list>
#include <vector>

// boost
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope

#pragma warning( disable : 4100 ) // unreferenced formal parameter

#include <boost/format.hpp>
using boost::format;
using boost::wformat ;
typedef boost::basic_format< TCHAR > tformat;

#pragma warning( default : 4100 ) // unreferenced formal parameter


#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>
#pragma warning( disable : 4180 ) // qualifier applied to function type has no meaning; ignored
#include <boost/bind.hpp>

#pragma warning( disable : 4701 ) // 初期化されていない可能性のあるローカル変数 'result' が使用されます
#include <boost/lexical_cast.hpp>
#pragma warning( default : 4701 )

#include <boost/foreach.hpp>
using namespace boost::foreach ;
#define foreach BOOST_FOREACH

#include <atlbase.h>
#include <atlcom.h>

#pragma warning( disable : 4996 ) 
#include "atlapp.h"
#pragma warning( default : 4996 ) 

#include "stringex.h"
#include "stringconversions.h"

#include "textstream_reader.h"

#include <iostream>

#pragma warning( disable : 4278 )
#pragma warning( disable : 4146 )
	//The following #import imports the IDTExtensibility2 interface based on it's LIBID
	#import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" version("1.0") lcid("0")  raw_interfaces_only named_guids

#pragma warning( default : 4146 )
#pragma warning( default : 4278 )

class DECLSPEC_UUID("A6FF7939-E14C-4C26-89B9-4803617A7DB6") ExcelAssistLib;

using namespace ATL;

#include "resource_string.h"

/*
 * Application module
 */
class CAddInModule : public CAtlDllModuleT< CAddInModule >
{
public:
	CAddInModule() : m_hInstance(NULL)
	{
	}

	DECLARE_LIBID(__uuidof(ExcelAssistLib))

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

#include <map>

#ifdef UNIT_TEST
#include "FakeWindow.h"
#define TWindow CFakeWindow
#define DECLARE_SENSING_VAR std::vector<string> m_sensing_variable
#define SENSE(x) m_sensing_variable.push_back(string(x))
#else
#define TWindow CWindow
#define DECLARE_SENSING_VAR
#define SENSE(x) (void)0
#endif


#define VERSION "1.6"

#define LOGGING_SERVER L"FelixHelpers.ExcelLogger"
#define LOGFILE_NAME _T("excel_assist.log")
