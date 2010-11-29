// StaticText.h : main header file for the STATICTEXT application
//

#if !defined(AFX_STATICTEXT_H__BA2E4712_9714_4FC5_9B4D_58489DEF8B8E__INCLUDED_)
#define AFX_STATICTEXT_H__BA2E4712_9714_4FC5_9B4D_58489DEF8B8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStaticTextApp:
// See StaticText.cpp for the implementation of this class
//

class CStaticTextApp : public CWinApp
{
public:
	CStaticTextApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticTextApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStaticTextApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICTEXT_H__BA2E4712_9714_4FC5_9B4D_58489DEF8B8E__INCLUDED_)
