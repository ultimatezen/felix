// HLinks.h : main header file for the HLINKS application
//

#if !defined(AFX_HLINKS_H__8CDFC7E5_CC4B_45FF_BA9E_85F0B947DD4E__INCLUDED_)
#define AFX_HLINKS_H__8CDFC7E5_CC4B_45FF_BA9E_85F0B947DD4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "VXPHyperLink.h"

/////////////////////////////////////////////////////////////////////////////
// CHLinksApp:
// See HLinks.cpp for the implementation of this class
//
class CHLinksApp : public CWinApp
{
public:
	CHLinksApp();

	HCURSOR GetCursor();
	bool m_bUseHandCursor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLinksApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHLinksApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HCURSOR m_hHandCursor;
};

extern CHLinksApp theApp;

/////////////////////////////////////////////////////////////////////////////

// The only purpose of overriding class CHyperLink here
// is to access the Hand cursor loaded by the application class:
class CSimpleLink:public CHyperLink
{
protected:
	virtual HCURSOR GetHighlightingCursor()
	{
		return theApp.GetCursor();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	CSimpleLink m_ref;	// This one is to draw hyperlink www.tooltips.net
	CVXPTooltipManager m_tt; // Tooltips
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLINKS_H__8CDFC7E5_CC4B_45FF_BA9E_85F0B947DD4E__INCLUDED_)
