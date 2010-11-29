// Menus.h : main header file for the MENUS application
//

#if !defined(AFX_MENUS_H__236E77E9_4D54_4932_BBC4_6907A91D5817__INCLUDED_)
#define AFX_MENUS_H__236E77E9_4D54_4932_BBC4_6907A91D5817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMenusApp:
// See Menus.cpp for the implementation of this class
//

class CMenusApp : public CWinApp
{
public:
	CMenusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenusApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMenusApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUS_H__236E77E9_4D54_4932_BBC4_6907A91D5817__INCLUDED_)
