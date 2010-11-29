// VXPTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CustomTooltips.h"
#include "VXPTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVXPTreeCtrl

CVXPTreeCtrl::CVXPTreeCtrl()
{
	m_hItem = NULL;		// No item selected
	Theme = ttSlick;	// Selecting tooltip theme ttSlick for the tree
}

CVXPTreeCtrl::~CVXPTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CVXPTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CVXPTreeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVXPTreeCtrl message handlers

// This event is fired whenever it is time to show a tooltip
// for a window that doesn't have any tooltip directly attached to it,
// or if it does, but such tooltip is not active:
BSTR CVXPTreeCtrl::OnActivateCustomTooltip(HWND hWnd, LPCTSTR ClassName, short x, short y)
{
	if(hWnd != m_hWnd)	// If the mouse cursor is not above the tree control
		return NULL;	// Return NULL

	UINT uFlags;
	m_hItem = HitTest(CPoint(x, y), &uFlags);	// Retrieve tree item under the mouse cursor
	if(m_hItem && (uFlags & TVHT_ONITEM))		// If the mouse is on a tree item
	{
		m_strItemText = GetItemText(m_hItem);	// Get the item's text

		// Making sure the text doesn't have HTML-reserved symbols, and splitting it into
		// several lines, if it is too long (longer than 30 symbols);
		// If we get more than 3 lines of text such text is to be truncated:
		long nRows = 3;
		m_strItemText = TextToHTMLEx((LPCTSTR)m_strItemText, 30, &nRows);

		// A few possible implementations:

		// Implementation 1:
		// We can return a simple tooltip like this:
		// return m_strItemText.copy();

		// Implementation 2:
		// If you want extra HTML formatting you could use the following
		// three lines:
		// CString txt;
		// txt.Format(_T("<b>Found Item: </b><font color=blue>%s</font>"), (LPCTSTR)m_strItemText);
		// return txt.AllocSysString();

		// Implementation 3:
		// The same as implementation 2, except the formatting text string would
		// be loaded from a string or HTML resourse.

		// Implementation 4:
		// We use here by default the most interesting and complicated solution-
		// returning a QHTML document that refers to an HTML resource:
		_bstr_t txt(_T("<txt res='html:idr_treeitem'>")); // This will load HTML resource "IDR_TREEITEM"
		// The complication comes from the fact that  document "IDR_TREEITEM" uses tags <TXT> with ID, which
		// means we have to override method OnGetText to return the dynamic contents.
		return txt.copy();

	}

	// If the mouse cursor is not over any of the tree items
	// we'll just show a tooltip for the entire tree control:
	_bstr_t entire_tree(_T("<txt res='html:idr_treecontrol'>"));
	return entire_tree.copy();
}

// This event is provoked by using tag <TXT> in our QHTML document,
// Read VXPLib documentation for details.
// If we didn't use tag <TXT> in resource "IDR_TREEITEM" there would be
// no need to handle this event.
BSTR CVXPTreeCtrl::OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID)
{
	// ID-s here correspond to those we specified
	// in HTML document "IDR_TREEITEM":
	switch(ID)
	{
	case 1:
		{
			return m_strItemText.copy();
		}
	case 2:
		{
			TCHAR buffer[20];
			itoa((long)m_hItem, buffer, 16);	// Converting the item's handle into hex
			_tcsupr(buffer);					// Upping the letters
			return _bstr_t(buffer).copy();
		}
	default:
		break;
	}

	return NULL;
}

void CVXPTreeCtrl::PreSubclassWindow()
{
	// This is to avoid showing tooltips for this control
	// if its parent window is not active:
	Activator = GetParent()->m_hWnd;

	CTreeCtrl::PreSubclassWindow();
}
