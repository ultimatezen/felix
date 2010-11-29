#if !defined(AFX_STDAFX_H__51F5FC10_8CF5_4F20_AA31_55D40252C76A__INCLUDED_)
#define AFX_STDAFX_H__51F5FC10_8CF5_4F20_AA31_55D40252C76A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define VXPLIB_WITH_EVENTS // Use events via ATL library
#include "VXPLib.h"

#ifndef _WINDOWS_
#include "afxver_.h"
#endif

#pragma message("VXPLib Examples: <Parser Test> for VC++ 6 and 7; Visit www.Tooltips.NET for updates.")
#pragma warning(disable: 4100)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__51F5FC10_8CF5_4F20_AA31_55D40252C76A__INCLUDED_)
