// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__DBCEF211_B846_47FA_AF43_DFCDB1924DE2__INCLUDED_)
#define AFX_MAINFRM_H__DBCEF211_B846_47FA_AF43_DFCDB1924DE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// We need to declare this class only because we need to handle
// event OnSetDefaults() of CVXPQuickHTML to select
// the default font for all menu items the same as the font for
// menus used in the system.
// Additionally, we setup Transparent=true in the class.
class CMenuItem:public CVXPQuickHTML
{
public:
	CMenuItem()
	{
		// This will provide that CVXPQuickHTML will be doing
		// drawing right on the screen, and without any buffering:
		Transparent = true;
	}
	// This method is just to make easier to
	// associate text with menu items:
	CMenuItem & operator = (LPCTSTR Text)
	{
		SetSourceText(Text);
		return *this;
	}
protected:
	void OnSetDefaults()
	{
		// Selecting the font used for menus in the system:
		settings.font.SetFromSystemFont(fontMenu);
		// Alternatively, we could add to each HTML document:
		// <BODY FONT=Menu>, which would do the same
	}
};

class CMainFrame : public CFrameWnd, public CVXPMenuHook
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// This variable will be used for drawing some intro
// text right in the middle of the main view:
	CVXPQuickHTML m_info;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual void OnMeasureItem(long ItemID, short & ItemWidth, short & ItemHeight);
	virtual void OnDrawItem(HMENU hMenu, HDC hDC, long ItemID, VXPMenuItemFlags Flags, short X, short Y, short W, short H);
	virtual void OnSettingsChanged();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTransparentMenus();
	afx_msg void OnTransparentWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

// Here we chose a very simple way of organizing
// drawing for each menu item. We use two separate copies
// of CMenuItem to draw each menu item:
// - One instance is used for drawing the item when it is selected;
// - The other - when the item is in its normal state.

	CMenuItem m_ItemNew;
	CMenuItem m_ItemNewSelected;

	CMenuItem m_ItemOpen;
	CMenuItem m_ItemOpenSelected;

	CMenuItem m_ItemSave;
	CMenuItem m_ItemSaveSelected;

	CMenuItem m_ItemPrint;
	CMenuItem m_ItemPrintSelected;

	CMenuItem m_ItemPreview;
	CMenuItem m_ItemPreviewSelected;

	CMenuItem m_ItemExit;
	CMenuItem m_ItemExitSelected;

// This instance of CVXPTooltipManager we will use to
// attach tooltips for each menu item that we modify.
// Note: You do not need to modify your menu items,
// if all you need is to attach tooltips to them.
	CVXPTooltipManager m_tool;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__DBCEF211_B846_47FA_AF43_DFCDB1924DE2__INCLUDED_)
