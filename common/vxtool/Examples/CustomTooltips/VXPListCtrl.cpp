// VXPListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CustomTooltips.h"
#include "VXPListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVXPListCtrl

CVXPListCtrl::CVXPListCtrl()
{
	ShowDelay = 300; // Display tooltips after 0.3 second over an item
}

CVXPListCtrl::~CVXPListCtrl()
{
}

BEGIN_MESSAGE_MAP(CVXPListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CVXPListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVXPListCtrl message handlers

#define MAX_COLUMN_NAME_LENGTH	255

// This event is fired whenever it is time to show a tooltip
// for a window that doesn't have any tooltip directly attached to it,
// or if it does, but such tooltip is not active:
BSTR CVXPListCtrl::OnActivateCustomTooltip(HWND hWnd, LPCTSTR ClassName, short x, short y)
{
	POINT p = {x, y};
	CHeaderCtrl * pHeader = GetHeaderCtrl();

	if(hWnd == pHeader->m_hWnd) // If the mouse cursor is over the list header
	{
		HDHITTESTINFO info;
		info.pt = p;
		info.flags = HHT_ONHEADER; // Test for the header only

		// Finding out on what column header the mouse cursor is:
		if(pHeader->SendMessage(HDM_HITTEST, 0, (long)&info) == -1)
			return NULL;

		// *** Getting title of the column header: *******************
		HDITEM ItemInfo;
		ItemInfo.mask = HDI_TEXT;									// Getting text only
		ItemInfo.cchTextMax = MAX_COLUMN_NAME_LENGTH;				// Maximum length of the header title
		ItemInfo.pszText = new TCHAR[MAX_COLUMN_NAME_LENGTH + 1];	// Allocating text buffer
		pHeader->GetItem(info.iItem, &ItemInfo);					// Getting the text
		m_strHeaderTitle = ItemInfo.pszText;						// Storing the text
		delete []ItemInfo.pszText;									// Deallocating the buffer
		// ***********************************************************

		// Returning QHTML template to show information about the
		// active column header in a tooltip:
		_bstr_t t(_T("<txt res='html:idr_headercolumn'>"));
		return t.copy();
	}

	if(hWnd != m_hWnd)	// If the mouse cursor is not over the list control
		return NULL;

	// Checking where the mouse cursor is within
	// the list control:
	LVHITTESTINFO lvhti;
	lvhti.pt = p;
	SubItemHitTest(&lvhti);

	if (lvhti.flags & LVHT_ONITEMLABEL) // If found an item underneath the mouse cursor
	{
		m_strItemText = GetItemText(lvhti.iItem, lvhti.iSubItem); // Get the item text

		// Getting title of the column (just one more way of doing it):
		LVCOLUMN col;
		col.mask = LVCF_TEXT;									// Text only
		col.pszText = new TCHAR[MAX_COLUMN_NAME_LENGTH + 1];	// Allocating buffer
		col.cchTextMax = MAX_COLUMN_NAME_LENGTH;
		GetColumn(lvhti.iSubItem, &col);						// Getting the column text
		m_strHeaderTitle = col.pszText;
		delete []col.pszText;									// Deallocating the buffer
	}
	else
	{
		// Otherwise, we just return a tooltip for
		// the entire list control:
		_bstr_t txt = _T("<txt res='html:idr_listcontrol'>");
		return txt.copy();
	}

	if(m_strItemText.length() <= 1) // If no text was copied
		return NULL;
	else
		// Making sure the text doesn't have HTML-reserved symbols, and splitting the text into
		// several lines, if it is too long (longer than 30 symbols):
		m_strItemText = TextToHTML((BSTR)m_strItemText, 30);
	
	// Here we put even further automation by allowing means of QHTML itself
	// to do the formatting through tags <TXT> in document "IDR_LISTITEM".
	// Read VXPLib documentation for details on tag <TXT>
	CString strTool = _T("<txt res='html:idr_listitem'>");

	return strTool.AllocSysString();
}

// This event is provoked by using tag <TXT> in our QHTML documents,
// Read VXPLib documentation for details.
BSTR CVXPListCtrl::OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID)
{
	// ID-s here correspond to those we specified
	// in HTML documents "IDR_LISTITEM" and "IDR_HEADERCOLUMN":
	switch(ID)	
	{
	case 1:
		{
			return m_strHeaderTitle.copy();
		}
	case 2:
		{
			return m_strItemText.copy();
		}
	default:
		break;
	}
	return NULL;
}

void CVXPListCtrl::PreSubclassWindow()
{
	// This is to avoid showing tooltips for this control
	// if its parent window is not active:
	Activator = GetParent()->m_hWnd;

	// Adding the general list header tooltip:
	windows.Add(GetHeaderCtrl()->m_hWnd, _T("<txt res='html:idr_listheader'>"));
	
	CListCtrl::PreSubclassWindow();
}

void CVXPListCtrl::SetOneHeaderTooltip(bool bOneHeaderTooltip)
{
	// If bOneHeaderTooltip is FALSE we are deactivating the header's general
	// tooltip to make CVXPTooltipManager fire event OnActivateCustomTooltip
	// to get the tooltip, or, if bOneHeaderTooltip is TRUE we are activating
	// the general tooltip to show it instead of showing custom tooltips.
	windows.SetActive(GetHeaderCtrl()->m_hWnd, bOneHeaderTooltip);
}
