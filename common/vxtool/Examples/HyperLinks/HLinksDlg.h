/////////////////////////////////////////////////////////////
// HyperLinks Demo Program by Vitaly Tomilov
//
// This demo program is part of VXPLib Examples that can be found
// on www.Tooltips.NET
// 
// The program shows how to create and use hyperlink objects
// of two levels:  standard, by using class CHyperLink,
// and advanced hyperlinks using class CVXPHyperLink.
//
// Last Modified on Jan 13, 2003
//
//
// For updates or any question visit www.tooltips.net
//////////////////////////////////////////////////////////////

#if !defined(AFX_HLINKSDLG_H__5FA0B672_CC9C_4097_816B_3B506BEA0CE6__INCLUDED_)
#define AFX_HLINKSDLG_H__5FA0B672_CC9C_4097_816B_3B506BEA0CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHLinksDlg dialog

#include "VXPHyperLink.h"

// We use this template just to organize a common way of accessing
// the cursor and the flag to use it (both stored in the application class).
// Class T is supposed to be derived from class CHyperLink:
template<class T = CHyperLink>
class TCursorOverride:public T
{
protected:
	virtual HCURSOR GetHighlightingCursor()
	{
		return theApp.GetCursor();
	}
	// This method is to notify the dialog box about
	// highlighting a hyperlink:
	virtual void OnHighlight(bool bHighlighted);
};

// This class represents a simple hyperlink with all
// default properties of class CHyperLink.
// The only reason we derive it from TCursorOverride is
// to provide us with the cursor feature:
class CMyLink:public TCursorOverride<>
{
public:
	// Constructor:
	CMyLink()
	{
		// Try to uncomment these lines to see the result
		//
		// bTransparent = false;				// Draw text using background color
		// bgColor = RGB(255,255,150);			// Setting background color for text
		//
		// style |= HLS_UNDERLINE_NORMAL;		// Underline text when in normal state
		// style &= ~HLS_UNDERLINE_HIGHLIGHT;	// Do not underline highlighted text
	}
};

// For all HTML hyperlinks we are not going to use two extra QHTML
// documents to represent the "Visited" state. Therefore, we override
// class CVXPHyperLink just to remove the appropriate style flags.
// Additionally, we get the cursor feature provided by template
// class TCursorOverride:
class CMyQHTMLLink:public TCursorOverride<CVXPHyperLink>
{
public:
	CMyQHTMLLink()
	{
		// Do not use styles HLS_NORMAL_VISITED and HLS_HIGHLIGHT_VISITED:
		style &= ~(HLS_NORMAL_VISITED | HLS_HIGHLIGHT_VISITED);
	}
	virtual void OnActivate()
	{
		if(!url.IsEmpty())
			TCursorOverride<CVXPHyperLink>::OnActivate();
	}
};

// In this class we need to do the trick with the OnClick event
// to make the hyperlink behave like a check box with two
// possible states: checked and unchecked:
class CMyCheckBox:public TCursorOverride<CVXPHyperLink>
{
protected:
	virtual void OnActivate()
	{
		bVisited = theApp.m_bUseHandCursor;					// Changing the "Visited" state of the hyperlink
		theApp.m_bUseHandCursor = !theApp.m_bUseHandCursor; // Toggling the checkbox
		UpdateCursor();										// Updating the cursor appearance
		UpdateLink();										// Updating the hyperlink
	}
};

class CHLinksDlg : public CDialog
{
// Construction
public:
	CHLinksDlg(CWnd* pParent = NULL);	// standard constructor

	void OnHighligh(CHyperLink * pLink, bool bHighlighted);

// Dialog Data
	//{{AFX_DATA(CHLinksDlg)
	enum { IDD = IDD_HLINKS_DIALOG };
	CVXPTooltipManager m_tt;
	CStatic	m_StatusBar;
	CMyCheckBox m_UseHand;	// Check-box hyperlink
	CMyQHTMLLink m_tip;
	CMyQHTMLLink m_Bart;
	CMyQHTMLLink m_qhtml9;
	CMyQHTMLLink m_qhtml8;
	CMyQHTMLLink m_qhtml7;
	CMyQHTMLLink m_qhtml6;
	CMyQHTMLLink m_qhtml5;
	CMyQHTMLLink m_qhtml4;
	CMyQHTMLLink m_qhtml3;
	CMyQHTMLLink m_qhtml2;
	CMyQHTMLLink m_qhtml1;
	CMyLink m_hlink11;
	CMyLink m_hlink10;
	CMyLink m_hlink9;
	CMyLink m_hlink8;
	CMyLink m_hlink7;
	CMyLink m_hlink6;
	CMyLink m_hlink5;
	CMyLink m_hlink4;
	CMyLink m_hlink3;
	CMyLink m_hlink2;
	CMyLink m_hlink1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLinksDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHLinksDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBart();
	afx_msg void OnDisable();
	afx_msg void OnSink();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLINKSDLG_H__5FA0B672_CC9C_4097_816B_3B506BEA0CE6__INCLUDED_)
