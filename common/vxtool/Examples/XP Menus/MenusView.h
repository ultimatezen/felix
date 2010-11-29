// MenusView.h : interface of the CMenusView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSVIEW_H__A710187E_76ED_4C91_95C0_99527F36DE98__INCLUDED_)
#define AFX_MENUSVIEW_H__A710187E_76ED_4C91_95C0_99527F36DE98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMenusView : public CView
{
protected: // create from serialization only
	CMenusView();
	DECLARE_DYNCREATE(CMenusView)

// Attributes
public:
	CMenusDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenusView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// This is to display a tooltip for the main
	// view of the application:
	CVXPTooltipManager m_tool;

// Generated message map functions
protected:
	//{{AFX_MSG(CMenusView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MenusView.cpp
inline CMenusDoc* CMenusView::GetDocument()
   { return (CMenusDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUSVIEW_H__A710187E_76ED_4C91_95C0_99527F36DE98__INCLUDED_)
