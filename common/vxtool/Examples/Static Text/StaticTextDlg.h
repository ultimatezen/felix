// StaticTextDlg.h : header file
//

#if !defined(AFX_STATICTEXTDLG_H__9B278594_D919_4D66_999F_6F2B143523C2__INCLUDED_)
#define AFX_STATICTEXTDLG_H__9B278594_D919_4D66_999F_6F2B143523C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VXPStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticTextDlg dialog

class CStaticTextDlg : public CDialog
{
// Construction
public:
	CStaticTextDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStaticTextDlg)
	enum { IDD = IDD_STATICTEXT_DIALOG };
	CButton	m_Sunken;
	CButton	m_Disabled;
	CVXPStatic	m_Static;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStaticTextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDisabled();
	afx_msg void OnSunken();
	afx_msg LRESULT OnSettingsChanged(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICTEXTDLG_H__9B278594_D919_4D66_999F_6F2B143523C2__INCLUDED_)
