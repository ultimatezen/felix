#if !defined(AFX_SETTINGSPPG_H__0A1547FE_60B4_4E1F_BF32_DCAE8CD23944__INCLUDED_)
#define AFX_SETTINGSPPG_H__0A1547FE_60B4_4E1F_BF32_DCAE8CD23944__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsPPG.h : header file
//

#include "list"
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSettingsPPG dialog

class CSettingsPPG : public CPropertyPage
{
	DECLARE_DYNCREATE(CSettingsPPG)

// Construction
public:
	CSettingsPPG();

	CString m_dir;
	list<CString> m_DirList;

// Dialog Data
	//{{AFX_DATA(CSettingsPPG)
	enum { IDD = IDD_SETTINGS_PPG };
	CListBox	m_paths;
	CString	m_resource;
	BOOL	m_bHideTooltips;
	long	m_cp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSettingsPPG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSettingsPPG)
	afx_msg void OnBrowse();
	afx_msg void OnAddpath();
	virtual BOOL OnInitDialog();
	afx_msg void OnHideTooltips();
	afx_msg void OnChangeCodepage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSPPG_H__0A1547FE_60B4_4E1F_BF32_DCAE8CD23944__INCLUDED_)
