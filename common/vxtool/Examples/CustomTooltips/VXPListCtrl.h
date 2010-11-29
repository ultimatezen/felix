#if !defined(AFX_VXPLISTCTRL_H__1F809F7A_84CC_4E37_A7A9_E68753BDFE95__INCLUDED_)
#define AFX_VXPLISTCTRL_H__1F809F7A_84CC_4E37_A7A9_E68753BDFE95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VXPListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVXPListCtrl window

class CVXPListCtrl : public CListCtrl, public CVXPTooltipManager
{
// Construction
public:
	CVXPListCtrl();

	virtual ~CVXPListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVXPListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

	void SetOneHeaderTooltip(bool bOneHeaderTooltip);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CVXPListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	// Overriding event of CVXPTooltipManager:
	virtual BSTR OnActivateCustomTooltip(HWND hWnd, LPCTSTR ClassName, short x, short y);

	// Overriding event of CVXPQuickHTML:
	virtual BSTR OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID);

	DECLARE_MESSAGE_MAP()

private:

	_bstr_t m_strItemText;		// Text in the current list cell
	_bstr_t m_strHeaderTitle;	// Title of the column's header
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VXPLISTCTRL_H__1F809F7A_84CC_4E37_A7A9_E68753BDFE95__INCLUDED_)
