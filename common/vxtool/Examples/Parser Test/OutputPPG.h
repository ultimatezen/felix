#if !defined(AFX_OUTPUTPPG_H__E1CDD29C_CBE1_48F9_9721_7C4A6C1A4BA4__INCLUDED_)
#define AFX_OUTPUTPPG_H__E1CDD29C_CBE1_48F9_9721_7C4A6C1A4BA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputPPG.h : header file
//

#include "BitmapCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// COutputPPG dialog

class COutputPPG : public CPropertyPage, public CVXPQuickHTML
{
	DECLARE_DYNCREATE(COutputPPG)

// Construction
public:	
	COutputPPG();
	~COutputPPG();

#ifdef _UNICODE // Making it possible to compile the project even without Unicode
	void UpdateOutput(LPCTSTR text = NULL);
#else
	void UpdateOutput(BSTR text = NULL);	
#endif

// Dialog Data
	//{{AFX_DATA(COutputPPG)
	enum { IDD = IDD_OUTPUT_PPG };
	CListBox	m_events;
	//}}AFX_DATA

	CBitmapCtrl m_bmpCtrl;
	HBITMAP m_Bitmap;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COutputPPG)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Overriding virtual methods from CVXPQuickHTML:
	virtual void OnRecalculate(bool Begin, bool SourceChanged);
	virtual void OnSetDefaults();
	virtual void OnSourceChanged(bool IsEmpty);
	virtual void OnDocumentGetSize(LPCTSTR DocumentPath, LPCTSTR Param, short & Width, short & Height);
	virtual void OnDocumentDraw(LPCTSTR DocumentPath, LPCTSTR Param, HDC DC, short x, short y, short Width, short Height);
	virtual BSTR OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID);
	virtual void OnError(HRESULT ErrorCode, LPCTSTR ErrorMsg);
	virtual void OnControlGetSize(long ID, LPCTSTR Name, short & Width, short & Height, LPCTSTR Param);

	// Generated message map functions
	//{{AFX_MSG(COutputPPG)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddEvent(LPCTSTR txt);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTPPG_H__E1CDD29C_CBE1_48F9_9721_7C4A6C1A4BA4__INCLUDED_)
