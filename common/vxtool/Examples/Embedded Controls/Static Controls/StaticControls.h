// StaticControls.h : main header file for the SIMPLECONTROLS application
//

#if !defined(AFX_SIMPLECONTROLS_H__05C6A330_31D0_4C3A_B212_62E100866A5B__INCLUDED_)
#define AFX_SIMPLECONTROLS_H__05C6A330_31D0_4C3A_B212_62E100866A5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStaticControlsApp:
// See StaticControls.cpp for the implementation of this class
//

class CStaticControlsApp : public CWinApp
{
public:
	CStaticControlsApp();

	HCURSOR GetCursor();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticControlsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStaticControlsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HCURSOR m_hCursor;
};

extern CStaticControlsApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLECONTROLS_H__05C6A330_31D0_4C3A_B212_62E100866A5B__INCLUDED_)
