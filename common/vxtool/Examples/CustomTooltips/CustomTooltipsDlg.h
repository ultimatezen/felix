// CustomTooltipsDlg.h : header file
//

#if !defined(AFX_CUSTOMTOOLTIPSDLG_H__8FC77DEC_2584_4F66_A2B8_ADA841F959F7__INCLUDED_)
#define AFX_CUSTOMTOOLTIPSDLG_H__8FC77DEC_2584_4F66_A2B8_ADA841F959F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Including our extended list and tree controls:
#include "VXPListCtrl.h"
#include "VXPTreeCtrl.h"
///////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsDlg dialog

class CCustomTooltipsDlg : public CDialog
{
// Construction
public:
	CCustomTooltipsDlg(CWnd* pParent = NULL);	// standard constructor

	~CCustomTooltipsDlg();	// standard destructor

// Dialog Data
	//{{AFX_DATA(CCustomTooltipsDlg)
	enum { IDD = IDD_CUSTOMTOOLTIPS_DIALOG };

	CVXPTreeCtrl m_Tree;
	CVXPListCtrl m_List;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomTooltipsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCustomTooltipsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOneHeaderClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HBRUSH m_hBrush;
	CVXPTooltipManager m_tt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMTOOLTIPSDLG_H__8FC77DEC_2584_4F66_A2B8_ADA841F959F7__INCLUDED_)
