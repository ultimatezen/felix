// DynamicControlsDlg.h : header file
//

#if !defined(AFX_SIMPLECONTROLSDLG_H__8BC5A583_CC2E_4DFC_ACF1_8937CB8C68A6__INCLUDED_)
#define AFX_SIMPLECONTROLSDLG_H__8BC5A583_CC2E_4DFC_ACF1_8937CB8C68A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VXPHyperLink.h"

// We use this template just to organize a common way of accessing
// the cursor. Class T is supposed to be derived from class CHyperLink:
template<class T = CHyperLink>
class TCursorOverride:public T
{
public:
	TCursorOverride()
	{
		// Do not support styles Normal Visited and Highlighted Visited:
		style &= ~(HLS_NORMAL_VISITED | HLS_HIGHLIGHT_VISITED);
	}
protected:
	virtual HCURSOR GetHighlightingCursor()
	{
		return theApp.GetCursor();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDynamicControlsDlg dialog

class CDynamicControlsDlg : public CDialog, public CVXPQuickHTML
{
// Construction
public:
	CDynamicControlsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDynamicControlsDlg)
	enum { IDD = IDD_DYNAMICCONTROLS_DIALOG };

	TCursorOverride<CVXPHyperLink>	m_hlink1;
	TCursorOverride<CVXPHyperLink>	m_hlink2;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicControlsDlg)
	protected:
	virtual void OnControlGetSize(long ID, LPCTSTR Name, short & Width, short & Height, LPCTSTR Param);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDynamicControlsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheckbox();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHyperLink1();
	afx_msg void OnHyperLink2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLECONTROLSDLG_H__8BC5A583_CC2E_4DFC_ACF1_8937CB8C68A6__INCLUDED_)
