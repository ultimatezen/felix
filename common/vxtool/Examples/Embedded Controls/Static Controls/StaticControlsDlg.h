// StaticControlsDlg.h : header file
//

#if !defined(AFX_SIMPLECONTROLSDLG_H__8BC5A583_CC2E_4DFC_ACF1_8937CB8C68A6__INCLUDED_)
#define AFX_SIMPLECONTROLSDLG_H__8BC5A583_CC2E_4DFC_ACF1_8937CB8C68A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VXPLib.h"
#include "HyperLink.h"

// We use this template just to organize a common way of accessing
// the cursor. Class T is supposed to be derived from class CHyperLink:
template<class T = CHyperLink>
class TCursorOverride:public T
{
protected:
	virtual HCURSOR GetHighlightingCursor()
	{
		return theApp.GetCursor();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CStaticControlsDlg dialog

class CStaticControlsDlg : public CDialog
{
// Construction
public:
	CStaticControlsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStaticControlsDlg)
	enum { IDD = IDD_STATICCONTROLS_DIALOG };
	CButton	m_skin3;
	CButton	m_skin2;
	CButton	m_skin1;
	CEdit	m_editbox;
	CButton	m_checkbox;
	TCursorOverride<CHyperLink>	m_hlink1;
	CButton	m_button3;
	CButton	m_button2;
	CButton	m_button1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticControlsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStaticControlsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckbox();
	afx_msg void OnSkinDefault();
	afx_msg void OnSkinSky();
	afx_msg void OnSkinFire();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitControls();
	CVXPQuickHTML m_html;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLECONTROLSDLG_H__8BC5A583_CC2E_4DFC_ACF1_8937CB8C68A6__INCLUDED_)
