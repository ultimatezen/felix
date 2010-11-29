#if !defined(AFX_VXPTREECTRL_H__35E81D02_C045_4262_929E_DBEF0D546757__INCLUDED_)
#define AFX_VXPTREECTRL_H__35E81D02_C045_4262_929E_DBEF0D546757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VXPTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVXPTreeCtrl window

class CVXPTreeCtrl : public CTreeCtrl, public CVXPTooltipManager
{
// Construction
public:
	CVXPTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVXPTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVXPTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVXPTreeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	// Overriding event of CVXPTooltipManager:
	virtual BSTR OnActivateCustomTooltip(HWND hWnd, LPCTSTR ClassName, short x, short y);

	// Overriding event of CVXPQuickHTML:
	virtual BSTR OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID);

	DECLARE_MESSAGE_MAP()

private:

	_bstr_t m_strItemText;
	HTREEITEM m_hItem;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VXPTREECTRL_H__35E81D02_C045_4262_929E_DBEF0D546757__INCLUDED_)
