// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F406D2D0_3B30_43D4_BFE5_2F309D0176E3__INCLUDED_)
#define AFX_STDAFX_H__F406D2D0_3B30_43D4_BFE5_2F309D0176E3__INCLUDED_

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

#pragma message("VXPLib Examples: <Custom Tooltips> for VC++ 6 and 7; Visit www.Tooltips.NET for updates.")

// Including VXPLib support library for VC++: ////////////////////////
#define VXPLIB_WITH_EVENTS // We're going to use events of the library
#include "VXPLib.h"
//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F406D2D0_3B30_43D4_BFE5_2F309D0176E3__INCLUDED_)
