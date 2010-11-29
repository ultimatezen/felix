// InputPPG.cpp : implementation file
//

#include "stdafx.h"
#include "Parser Test.h"
#include "InputPPG.h"
#include "FileDialogEx.h"
#include "OutputPPG.h"
#include "ParserTestSheet.h"
#include "AfxPriv.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputPPG property page

IMPLEMENT_DYNCREATE(CInputPPG, CPropertyPage)

CInputPPG::CInputPPG() : CPropertyPage(CInputPPG::IDD)
{
// No help button on this tab page:
	m_psp.dwFlags &= ~PSP_HASHELP;

	m_bChanged = false;
	m_bEditChanged = false;

// Retriving full path to the templates directory:
	GetModuleFileName(NULL, m_templates.GetBufferSetLength(MAX_PATH), MAX_PATH);
	m_templates.ReleaseBuffer();
	int index = m_templates.ReverseFind('\\');
	if(index != -1)
	{
		m_templates = m_templates.Left(index);
		m_templates += _T("\\QHTML");
	}
}

void CInputPPG::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPPG)
	DDX_Text(pDX, IDC_INPUT, m_text);
	DDX_Control(pDX, IDC_INPUT, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInputPPG, CPropertyPage)
	//{{AFX_MSG_MAP(CInputPPG)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAbout)
	ON_BN_CLICKED(ID_FONT, OnFont)
	
	ON_BN_CLICKED(ID_FILE_NEW, OnFileNew)
	ON_BN_CLICKED(ID_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(ID_FILE_SAVE, OnFileSave)
	ON_BN_CLICKED(ID_FILE_SAVE_AS, OnFileSaveAs)
	
	ON_BN_CLICKED(ID_EDIT_COPY, OnEditCopy)
	ON_BN_CLICKED(ID_EDIT_PASTE, OnEditPaste)
	ON_BN_CLICKED(ID_EDIT_CUT, OnEditCut)
	ON_BN_CLICKED(ID_EDIT_UNDO, OnEditUndo)
	ON_BN_CLICKED(ID_EDIT_CLEAR, OnEditDelete)
	ON_BN_CLICKED(ID_EDIT_SELECT_ALL, OnSelectAll)

	ON_EN_CHANGE(IDC_INPUT, OnChangeInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPPG message handlers

void CInputPPG::OnChangeInput()
{
	m_bChanged = true;
	if(!m_bEditChanged)
	{
		m_bEditChanged = true;
		UpdateTitle();
	}
}

bool CInputPPG::IsChanged()
{
	return m_bChanged;
}

#ifdef _UNICODE // Making it possible to compile the project even without Unicode
LPCTSTR CInputPPG::GetText()
{
	UpdateData();
	m_bChanged = false;
	return m_text;
}
#else
// In not Unicode we have to use wide chars explicitly:
BSTR CInputPPG::GetText()
{
	m_bChanged = false;
	HWND hWnd = GetDlgItem(IDC_INPUT)->m_hWnd;
	int Length = ::GetWindowTextLengthW(hWnd);
	wchar_t * buffer = new wchar_t[Length + 1];
	::GetWindowTextW(hWnd, buffer, Length + 1);
	_bstr_t result(buffer);
	delete []buffer;
	return result;
}
#endif

BOOL CInputPPG::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	UpdateTitle();

	GetDlgItem(IDC_INPUT)->GetFont()->GetLogFont(&m_LogFont);
	((CParserTestSheet*)GetParent())->m_tm.windows.Add(m_hWnd, _T("<txt res='html:idr_propinput'>"));

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, CBRS_ALIGN_TOP|CBRS_FLYBY) || !m_wndToolBar.LoadToolBar(IDR_INPUTTAB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}	
	
	// Adjusting the toolbar:
	RECT r;
	GetClientRect(&r);
	SIZE size;
	m_wndToolBar.GetToolBarCtrl().GetMaxSize(&size);
	m_wndToolBar.SetWindowPos(NULL, 0, 0, size.cx, size.cy + 2, SWP_NOZORDER|SWP_SHOWWINDOW);

	return TRUE;
}

void CInputPPG::OnFont()
{
// Deactivating tooltips: *****
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;
// ****************************

	CFontDialog font(&m_LogFont, CF_SCREENFONTS);
	if(font.DoModal() == IDOK)
	{
		m_font.DeleteObject();
		m_font.CreateFontIndirect(&m_LogFont);
		GetDlgItem(IDC_INPUT)->SetFont(&m_font);
	}

// Reactivating tooltips:
	pSheet->m_tm.Active = bActive;
}

void CInputPPG::OnFileOpen()
{
// Deactivating tooltips: *****
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;
// ****************************

	CFileDialogEx dlg(TRUE, _T("HTML"), NULL, OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, _T("HTML UTF8 Files (*.htm, *.html)|*.htm;*.html|All Files (*.*)|*.*||"), this);
	if(dlg.DoModal(m_templates) == IDOK)
	{
		m_strFilePath = dlg.GetPathName();
		CFile file(m_strFilePath, CFile::modeRead);
		long L = (long)file.GetLength();
		char * buffer = new char[L + 1];
		file.Read(buffer, L);
		buffer[L] = 0;
#ifdef _UNICODE // Making it possible to compile the project even without Unicode
		int Size = MultiByteToWideChar(CP_UTF8, 0, buffer, L, NULL, 0);
		MultiByteToWideChar(CP_UTF8, 0, buffer, L, m_text.GetBufferSetLength(Size), Size);
		m_text.ReleaseBuffer(Size);
#else
		m_text = buffer;
#endif
		delete []buffer;
		UpdateData(FALSE);
		m_bChanged = true;

		int index = m_strFilePath.ReverseFind('\\');
		if(index != -1)
		{
			m_strFileName = m_strFilePath.Right(m_strFilePath.GetLength() - index - 1);
			m_bEditChanged = false;
			UpdateTitle();
		}
	}
// Reactivating tooltips:
	pSheet->m_tm.Active = bActive;
}

void CInputPPG::OnFileSave()
{
	DoSave(false);
}

void CInputPPG::OnFileSaveAs()
{
	DoSave(true);
}

void CInputPPG::SetModified()
{
	m_bChanged = true;
}

void CInputPPG::OnCancel()
{
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	pSheet->m_bCanClose = true;
	if(m_bEditChanged)
	{
		UINT result = AfxMessageBox(IDS_SAVE, MB_YESNOCANCEL|MB_ICONQUESTION);
		if(result == IDCANCEL || (result == IDYES && !DoSave(false)))
		{
			pSheet->m_bCanClose = false;
			return;
		}
	}
	CPropertyPage::OnCancel();
}

bool CInputPPG::DoSave(bool bSaveAs)
{
// Deactivating tooltips: *****
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;
// ****************************

	if(bSaveAs || m_strFileName.IsEmpty())
	{
		CFileDialogEx dlg(FALSE, _T("HTML"), NULL, OFN_ENABLESIZING|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST, _T("HTML UTF8 Files (*.htm, *.html)|*.htm;*.html|All Files (*.*)|*.*||"), this);
		if(dlg.DoModal(m_templates) == IDOK)
		{
			m_strFilePath = dlg.GetPathName();
			CFile file(m_strFilePath, CFile::modeCreate|CFile::modeWrite);
			UpdateData(TRUE);
#ifdef _UNICODE // Making it possible to compile the project even without Unicode
			int L = m_text.GetLength();
			char * buffer = new char[L * sizeof(TCHAR)];
			L = WideCharToMultiByte(CP_UTF8, 0, m_text, L, buffer, L * sizeof(TCHAR), NULL, NULL);
			file.Write(buffer, L);
			delete []buffer;
#else
			file.Write(m_text, m_text.GetLength());
#endif

			int index = m_strFilePath.ReverseFind('\\');
			if(index != -1)
			{
				m_strFileName = m_strFilePath.Right(m_strFilePath.GetLength() - index - 1);
				m_bEditChanged = false;
				UpdateTitle();
			}
			
			// Reactivating tooltips:
			pSheet->m_tm.Active = bActive;

			return true;
		}
	}
	else
	{
		CFile file(m_strFilePath, CFile::modeCreate|CFile::modeWrite);
		UpdateData(TRUE);
#ifdef _UNICODE // Making it possible to compile the project even without Unicode
		int L = m_text.GetLength();
		char * buffer = new char[L * sizeof(TCHAR)];
		L = WideCharToMultiByte(CP_UTF8, 0, m_text, L, buffer, L * sizeof(TCHAR), NULL, NULL);
		file.Write(buffer, L);
		delete []buffer;
#else
		file.Write(m_text, m_text.GetLength());
#endif
		m_bEditChanged = false;
		UpdateTitle();

		// Reactivating tooltips:
		pSheet->m_tm.Active = bActive;
		return true;
	}
// Reactivating tooltips:
	pSheet->m_tm.Active = bActive;
	return false;
}

void CInputPPG::OnFileNew()
{
	m_text.Empty();
	UpdateData(FALSE);
	m_bEditChanged = false;
	m_strFileName.Empty();
	UpdateTitle();
	GetDlgItem(IDC_INPUT)->SetFocus();
	m_bChanged = true;
}

void CInputPPG::UpdateTitle()
{
	CString title;
	if(m_strFileName.IsEmpty())
		title.Format(_T("Parser Test - [NONAME%s]"), m_bEditChanged?_T(" *"):_T(""));
	else
		title.Format(_T("Parser Test - [%s%s]"), m_strFileName, m_bEditChanged?_T(" *"):_T(""));

	GetParent()->SetWindowText(title);
}

void CInputPPG::OnEditCopy()
{
	GetDlgItem(IDC_INPUT)->SendMessage(WM_COPY, 0, 0);
}

void CInputPPG::OnEditDelete()
{
	GetDlgItem(IDC_INPUT)->SendMessage(WM_CLEAR, 0, 0);
}

void CInputPPG::OnEditPaste()
{
	GetDlgItem(IDC_INPUT)->SendMessage(WM_PASTE, 0, 0);
}

void CInputPPG::OnEditCut()
{
	GetDlgItem(IDC_INPUT)->SendMessage(WM_CUT, 0, 0);
}

void CInputPPG::OnEditUndo()
{
	GetDlgItem(IDC_INPUT)->SendMessage(WM_UNDO, 0, 0);
}

void CInputPPG::OnSelectAll()
{
	m_edit.SetSel(0, -1, TRUE);
}

void CInputPPG::OnAbout()
{
// Deactivating tooltips: *****
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;
// ****************************

	CAboutBox dlg;
	dlg.DoModal();

// Reactivating tooltips:
	pSheet->m_tm.Active = bActive;
}

BOOL CInputPPG::PreTranslateMessage(MSG* pMsg)
{
// Updating all dialog controls:
	UpdateDialogControls(this, TRUE);

	return CPropertyPage::PreTranslateMessage(pMsg);
}

bool CInputPPG::CanSave()
{
	return m_bEditChanged;
}

