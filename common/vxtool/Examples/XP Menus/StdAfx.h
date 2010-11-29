// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8F819A5B_48C1_4408_AE0E_ACBF68A06B46__INCLUDED_)
#define AFX_STDAFX_H__8F819A5B_48C1_4408_AE0E_ACBF68A06B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define VXPLIB_WITH_EVENTS
#include "VXPLib.h"

#pragma message("VXPLib Examples: <XP Menus> for VC++ 6 and 7; Visit www.Tooltips.NET for updates.")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8F819A5B_48C1_4408_AE0E_ACBF68A06B46__INCLUDED_)
