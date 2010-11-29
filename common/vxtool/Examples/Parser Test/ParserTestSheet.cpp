// ParserTestSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Parser Test.h"
#include "ParserTestSheet.h"
#include "AfxPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParserTestSheet

IMPLEMENT_DYNAMIC(CParserTestSheet, CPropertySheet)

CParserTestSheet::CParserTestSheet():CPropertySheet(_T("Parser Test"), 0, 0)
{
	m_bCanClose = true;

// Setting up the property sheet interface:
	m_psh.dwFlags |= PSH_NOAPPLYNOW|PSH_USEHICON;
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_psh.hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

// Loading accelerators:
	m_hAccel = LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

// Attaching all three tab pages:
	AddPage(&m_Input);
	AddPage(&m_Output);
	AddPage(&m_Settings);

	m_footer = _T("<txt res='html:idr_footer'>");	// Property Sheet footer

	m_tm.ShowDelay = 1000;	// Display tooltips after 1 second
	m_tm.HideDelay = 8000;	// Hide tooltips automatically after 8 seconds

// Adding tooltips here:
	m_tm.commands.Add(ID_FONT, _T("<txt res='html:idr_font'>"));
	m_tm.commands.Add(ID_APP_ABOUT, _T("<txt res='html:idr_about'>"));
	m_tm.commands.Add(ID_FILE_OPEN, _T("<txt res='html:idr_open'>"));
	m_tm.commands.Add(ID_FILE_SAVE, _T("<txt res='html:idr_save'>"));
	m_tm.commands.Add(ID_FILE_SAVE_AS, _T("<txt res='html:idr_saveas'>"));
	m_tm.commands.Add(ID_FILE_NEW, _T("<txt res='html:idr_new'>"));
	m_tm.commands.Add(IDC_INPUT, _T("<txt res='html:idr_input'>"));
	m_tm.commands.Add(IDC_CODEPAGE, _T("<txt res='html:idr_codepage'>"));
	m_tm.commands.Add(IDCANCEL, _T("<txt res='html:idr_cancel'>"));
	m_tm.commands.Add(IDC_WIDTH, _T("<txt res='html:idr_width'>"));
	m_tm.commands.Add(IDC_HEIGHT, _T("<txt res='html:idr_height'>"));
	m_tm.commands.Add(IDC_DELAY, _T("<txt res='html:idr_delay'>"));
	m_tm.commands.Add(IDC_DISPLAY, _T("<txt res='html:idr_display'>"));
	m_tm.commands.Add(ID_UPDATE, _T("<txt res='html:idr_update'>"));
	m_tm.commands.Add(IDC_EVENTS, _T("<txt res='html:idr_events'>"));
	m_tm.commands.Add(IDC_RESOURCE, _T("<txt res='html:idr_resource'>"));
	m_tm.commands.Add(ID_BROWSE, _T("<txt res='html:idr_browse'>"));
	m_tm.commands.Add(IDC_PATHS, _T("<txt res='html:idr_paths'>"));
	m_tm.commands.Add(ID_ADDPATH, _T("<txt res='html:idr_addpath'>"));
	m_tm.commands.Add(IDC_PICTURE, _T("<txt res='html:idr_picture'>"));
	m_tm.commands.Add(ID_HIDETOOLTIPS, _T("<txt res='html:idr_hidetooltips'>"));
	m_tm.commands.Add(ID_EDIT_COPY, _T("<txt res='html:idr_copy'>"));
	m_tm.commands.Add(ID_EDIT_PASTE, _T("<txt res='html:idr_paste'>"));
	m_tm.commands.Add(ID_EDIT_CUT, _T("<txt res='html:idr_cut'>"));
	m_tm.commands.Add(ID_EDIT_CLEAR, _T("<txt res='html:idr_delete'>"));
	m_tm.commands.Add(ID_EDIT_UNDO, _T("<txt res='html:idr_undo'>"));
	m_tm.commands.Add(ID_EDIT_SELECT_ALL, _T("<txt res='html:idr_select_all'>"));
}

BEGIN_MESSAGE_MAP(CParserTestSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CParserTestSheet)
	ON_WM_PAINT()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdatePaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateSelectAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// This method is to provide simulation of automatic interface
// update by calling update methods OnUpdateXXX
// (To make macro ON_UPDATE_COMMAND_UI work).
LRESULT CParserTestSheet::OnKickIdle(WPARAM, LPARAM)
{
	CToolBar & bar = m_Input.m_wndToolBar;
	CToolCmdUI state;
	state.m_pOther = &bar;
	state.m_nIndexMax = bar.GetToolBarCtrl().GetButtonCount();
	for(state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;state.m_nIndex ++)
	{
		state.m_nID = bar.GetItemID(state.m_nIndex);
		if (!(bar.GetButtonStyle(state.m_nIndex) & TBSTYLE_SEP))
			state.DoUpdate(this, FALSE);
	}
	return FALSE;
}

void CParserTestSheet::OnUpdatePaste(CCmdUI* pCmdUI)
{
	OpenClipboard();
	HGLOBAL hg = GetClipboardData(CF_TEXT);
	CloseClipboard();
	pCmdUI->Enable(hg?TRUE:FALSE);
}

void CParserTestSheet::OnUpdateUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Input.m_edit.CanUndo());
}

void CParserTestSheet::OnUpdateCut(CCmdUI* pCmdUI)
{
	int nStartChar, nEndChar;
	m_Input.m_edit.GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nEndChar - nStartChar > 0);
}

void CParserTestSheet::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(::GetWindowTextLengthW(m_Input.m_edit.m_hWnd) > 0);
}

void CParserTestSheet::OnUpdateCopy(CCmdUI* pCmdUI)
{
	int nStartChar, nEndChar;
	m_Input.m_edit.GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nEndChar - nStartChar > 0);
}

void CParserTestSheet::OnUpdateDelete(CCmdUI* pCmdUI)
{
	int nStartChar, nEndChar;
	m_Input.m_edit.GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nEndChar - nStartChar > 0);
}

void CParserTestSheet::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Input.CanSave());
}

/////////////////////////////////////////////////////////////////////////////
// CParserTestSheet message handlers

BOOL CParserTestSheet::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();

// Do a trick with the property sheet buttons:
	GetDlgItem(IDOK)->ModifyStyle(WS_VISIBLE, 0);
	GetDlgItem(IDCANCEL)->SetWindowText(_T("E&xit"));

// Attaching a tooltip to the property sheet itself:
	m_tm.windows.Add(m_hWnd, _T("<txt res='html:idr_propsheet'>"));

// Attaching a tooltip to the tab control:
	m_tm.commands.Add(GetTabControl()->GetDlgCtrlID(), _T("<txt res='html:idr_tabs'>"));

// Adding Minimize button to the dialog:
	::SetWindowLong(m_hWnd, GWL_STYLE, GetStyle() | WS_MINIMIZEBOX);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CParserTestSheet::ContinueModal()
{
// Do not just close, if there's an unsaved document:
	if(!m_bCanClose)
		return TRUE;
	
	return CPropertySheet::ContinueModal();
}

void CParserTestSheet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

// Painting the footer:
	RECT r;
	GetClientRect(&r);
	m_footer.Draw(dc, 0, (short)(r.bottom - m_footer.Height + 1));
}

BOOL CParserTestSheet::PreTranslateMessage(MSG* pMsg) 
{
// Providing accelerator functionality:
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
			return TRUE;
	return CPropertySheet::PreTranslateMessage(pMsg);
}
