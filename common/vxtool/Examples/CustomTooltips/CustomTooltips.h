// CustomTooltips.h : main header file for the CUSTOMTOOLTIPS application
//

#if !defined(AFX_CUSTOMTOOLTIPS_H__A04006E5_4E65_4920_93B3_CF614394EF42__INCLUDED_)
#define AFX_CUSTOMTOOLTIPS_H__A04006E5_4E65_4920_93B3_CF614394EF42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsApp:
// See CustomTooltips.cpp for the implementation of this class
//

class CCustomTooltipsApp : public CWinApp
{
public:
	CCustomTooltipsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomTooltipsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCustomTooltipsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMTOOLTIPS_H__A04006E5_4E65_4920_93B3_CF614394EF42__INCLUDED_)
