#if !defined(AFX_INPUTPPG_H__3C0296CF_13B7_44CC_8BD3_0C7AB002D6BD__INCLUDED_)
#define AFX_INPUTPPG_H__3C0296CF_13B7_44CC_8BD3_0C7AB002D6BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPPG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputPPG dialog

class CInputPPG : public CPropertyPage
{
	DECLARE_DYNCREATE(CInputPPG)

// Construction
public:
	bool CanSave();
	void SetModified();

#ifdef _UNICODE // Making it possible to compile the project even without Unicode
	LPCTSTR GetText();
#else
	BSTR GetText();
#endif

	bool IsChanged();
	CInputPPG();

// Dialog Data
	//{{AFX_DATA(CInputPPG)
	enum { IDD = IDD_INPUT_PPG };
	CString	m_text;
	CFont m_font;
	LOGFONT m_LogFont;
	CEdit m_edit;
	//}}AFX_DATA

	CToolBar m_wndToolBar;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInputPPG)
	public:
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInputPPG)
	afx_msg void OnChangeInput();
	afx_msg void OnAbout();
	virtual BOOL OnInitDialog();
	afx_msg void OnFont();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnEditDelete();
	afx_msg void OnSelectAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	void UpdateTitle();
	bool DoSave(bool bSaveAs);
	bool m_bChanged;
	bool m_bEditChanged;	// Contains True, if the QHTML Edit field has changed

	CString m_strFileName;	// Current file name
	CString m_strFilePath;	// Current full path to the file

	CString m_templates;	// Full path to the application without the file name
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPPG_H__3C0296CF_13B7_44CC_8BD3_0C7AB002D6BD__INCLUDED_)
