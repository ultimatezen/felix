// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Menus.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CComModule _Module;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TRANSPARENT_MENUS, OnTransparentMenus)
	ON_COMMAND(ID_TRANSPARENT_WINDOW, OnTransparentWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
// Here we setup QHTML resources for each menu item using tag <TXT>
// We use this approach for three reasons:
// 1. In the MFC library class CString cannot load an HTML resource,
//    and we do not want to write our own procedure to do that.
// 2. Putting lots of HTML right into code won't look good,
//    it is definitely should be in the resources
// 3. Viewing and editing HTML contents in the resource editor
//    is much easier than in the code.
//
// Basically, here we every time use tag <txt> specified to load
// an HTML resource that we need:

	m_ItemNew = _T("<txt res='html:idr_new'>");
	m_ItemNewSelected = _T("<txt res='html:idr_new_selected'>");

	m_ItemOpen = _T("<txt res='html:idr_open'>");
	m_ItemOpenSelected = _T("<txt res='html:idr_open_selected'>");

	m_ItemPrint = _T("<txt res='html:idr_print'>");
	m_ItemPrintSelected = _T("<txt res='html:idr_print_selected'>");

	m_ItemSave = _T("<txt res='html:idr_save'>");
	m_ItemSaveSelected = _T("<txt res='html:idr_save_selected'>");

	m_ItemPreview = _T("<txt res='html:idr_preview'>");
	m_ItemPreviewSelected = _T("<txt res='html:idr_preview_selected'>");

	m_ItemExit = _T("<txt res='html:idr_exit'>");
	m_ItemExitSelected = _T("<txt res='html:idr_exit_selected'>");

	// Telling the tooltip manager to display tooltips just after 0.3
	// second after the mouse cursor stops over a menu item:
	m_tool.ShowDelay = 300;

	// Here we attach a tooltip for each menu item.
	// Note that we use the same contents like for unselected
	// menu items. This indeed will display the same in the tooltips.
	// Author hopes his laziness will be forgiven, for it was done just
	// for simplicity to avoid writing extra HTML resources.
	// In practice, you, of course will have tooltips with a contents
	// different from what is displayed within the menu item.
	m_tool.commands.Add(ID_FILE_NEW, _T("<txt res='html:idr_new'>"));
	m_tool.commands.Add(ID_FILE_OPEN, _T("<txt res='html:idr_open'>"));
	m_tool.commands.Add(ID_FILE_PRINT, _T("<txt res='html:idr_print'>"));
	m_tool.commands.Add(ID_FILE_PRINT_PREVIEW, _T("<txt res='html:idr_preview'>"));
	m_tool.commands.Add(ID_FILE_SAVE, _T("<txt res='html:idr_save'>"));

	// This one will also attach the tooltip to the Exit
	// button on the toolbar, since they share one command ID:
	m_tool.commands.Add(ID_APP_EXIT, _T("<txt res='html:idr_exit'>"));

	// This is to be displayed in the main view window:
	m_info = _T("<txt res='html:idr_info'>");
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	// be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// ********************************************************************
	// *** Modifying menu items in the "File" menu to be ownerdrawn:
	// ********************************************************************
	if(m_tool.IsAttached()) // If VXPLib has been installed on the PC
	{
		HookWnd(m_hWnd);
		UINT MenuItems[] = {ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_PRINT, ID_FILE_SAVE,
							ID_FILE_PRINT_PREVIEW, ID_APP_EXIT};
		// Making all items ownerdrawn:
		for(int i = 0;i < sizeof(MenuItems) / sizeof(UINT);i ++)
			Override(MenuItems[i]);
	}
	else
		AfxMessageBox(IDS_ERROR);
	// *********************************************************************

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// Called to measure dimension for each ownerdrawn menu item:
void CMainFrame::OnMeasureItem(long ItemID, short & ItemWidth, short & ItemHeight)
{
	switch(ItemID)
	{
	case ID_FILE_NEW:
		{
			ItemWidth = m_ItemNew.Width;
			ItemHeight = m_ItemNew.Height;
			break;
		}
	case ID_FILE_OPEN:
		{
			ItemWidth = m_ItemOpen.Width;
			ItemHeight = m_ItemOpen.Height;
			break;
		}
	case ID_FILE_PRINT:
		{
			ItemWidth = m_ItemPrint.Width;
			ItemHeight = m_ItemPrint.Height;
			break;
		}
	case ID_FILE_SAVE:
		{
			ItemWidth = m_ItemSave.Width;
			ItemHeight = m_ItemSave.Height;
			break;
		}
	case ID_FILE_PRINT_PREVIEW:
		{
			ItemWidth = m_ItemPreview.Width;
			ItemHeight = m_ItemPreview.Height;
			break;
		}
	case ID_APP_EXIT:
		{
			ItemWidth = m_ItemExit.Width;
			ItemHeight = m_ItemExit.Height;
			break;
		}
	default:
		break;
	}
}

// This method is called when it is time to draw all ownerdrawn
// menu items (this method is called for each item):
void CMainFrame::OnDrawItem(HMENU /*hMenu*/, HDC hDC, long ItemID, VXPMenuItemFlags Flags, short X, short Y, short /*W*/, short /*H*/)
{
// Drawing menu item background;
// Note: you only can call this function within OnDrawItem()
	DrawItemBackground();

// Drawing contents for a menu item depending on the item ID:
	switch(ItemID)
	{
	case ID_FILE_NEW:
		{
			if(Flags & mfHiLite)
				m_ItemNewSelected.Draw(hDC, X, Y);
			else
				m_ItemNew.Draw(hDC, X, Y);
			break;
		}
	case ID_FILE_OPEN:
		{
			if(Flags & mfHiLite)
				m_ItemOpenSelected.Draw(hDC, X, Y);
			else
				m_ItemOpen.Draw(hDC, X, Y);

			break;
		}
	case ID_FILE_PRINT:
		{
			if(Flags & mfHiLite)
				m_ItemPrintSelected.Draw(hDC, X, Y);
			else
				m_ItemPrint.Draw(hDC, X, Y);
			break;
		}
	case ID_FILE_SAVE:
		{
			if(Flags & mfHiLite)
				m_ItemSaveSelected.Draw(hDC, X, Y);
			else
				m_ItemSave.Draw(hDC, X, Y);
			break;
		}
	case ID_FILE_PRINT_PREVIEW:
		{
			if(Flags & mfHiLite)
				m_ItemPreviewSelected.Draw(hDC, X, Y);
			else
				m_ItemPreview.Draw(hDC, X, Y);
			break;
		}
	case ID_APP_EXIT:
		{
			if(Flags & mfHiLite)
				m_ItemExitSelected.Draw(hDC, X, Y);
			else
				m_ItemExit.Draw(hDC, X, Y);
			break;
		}
	default:
		break;
	}
}

// This method is called in responce to message WM_SETTINGCHANGE
// to allow the application to update all its visual elements.
// Here we recalculate layouts for all ownerdrawn menu items:
void CMainFrame::OnSettingsChanged()
{
	m_ItemNew.RecalculateLayout();
	m_ItemNewSelected.RecalculateLayout();

	m_ItemOpen.RecalculateLayout();
	m_ItemOpenSelected.RecalculateLayout();

	m_ItemSave.RecalculateLayout();
	m_ItemSaveSelected.RecalculateLayout();

	m_ItemPrint.RecalculateLayout();
	m_ItemPrintSelected.RecalculateLayout();

	m_ItemPreview.RecalculateLayout();
	m_ItemPreviewSelected.RecalculateLayout();

	m_ItemExit.RecalculateLayout();
	m_ItemExitSelected.RecalculateLayout();
}

void CMainFrame::OnTransparentMenus()
{
	if(sysFadedMenus)
		AfxMessageBox(IDS_SYSFADING);
	else
		MenuTransparency = (GetMenu()->CheckMenuItem(ID_TRANSPARENT_MENUS, MF_BYCOMMAND|(MenuTransparency == 255)?MF_CHECKED:MF_UNCHECKED) == MF_UNCHECKED)?(UCHAR)185:(UCHAR)255;

	// Setting tooltips to have the same transparency
	// as all pop-up menus:
	m_tool.Transparency = MenuTransparency;
}

void CMainFrame::OnTransparentWindow()
{
	WindowTransparency = (GetMenu()->CheckMenuItem(ID_TRANSPARENT_WINDOW, MF_BYCOMMAND|(WindowTransparency == 255)?MF_CHECKED:MF_UNCHECKED) == MF_UNCHECKED)?(UCHAR)185:(UCHAR)255;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cx = m_info.Width + 100;
	cs.cy = m_info.Height + 150;
	cs.x = (GetSystemMetrics(SM_CXSCREEN) - cs.cx) / 2;
	cs.y = (GetSystemMetrics(SM_CYSCREEN) - cs.cy) / 2;
	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
