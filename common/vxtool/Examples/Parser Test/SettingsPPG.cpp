// SettingsPPG.cpp : implementation file
//

#include "stdafx.h"
#include "Parser Test.h"
#include "SettingsPPG.h"
#include "ParserTestSheet.h"
#include "FileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is only to be accessible from BrowseCallbackProc(...)
CSettingsPPG * pSettings = NULL;

/////////////////////////////////////////////////////////////////////////////
// CSettingsPPG property page

IMPLEMENT_DYNCREATE(CSettingsPPG, CPropertyPage)

CSettingsPPG::CSettingsPPG() : CPropertyPage(CSettingsPPG::IDD)
{
	// Reset the code page
	m_cp = 0;

	// No help button on this page
	m_psp.dwFlags &= ~PSP_HASHELP;

	pSettings = this;

	m_bHideTooltips = FALSE;
}

void CSettingsPPG::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsPPG)
	DDX_Control(pDX, IDC_PATHS, m_paths);
	DDX_Text(pDX, IDC_RESOURCE, m_resource);
	DDX_Text(pDX, IDC_CODEPAGE, m_cp);
	DDX_Check(pDX, ID_HIDETOOLTIPS, m_bHideTooltips);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSettingsPPG, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingsPPG)
	ON_BN_CLICKED(ID_BROWSE, OnBrowse)
	ON_BN_CLICKED(ID_ADDPATH, OnAddpath)
	ON_BN_CLICKED(ID_HIDETOOLTIPS, OnHideTooltips)
	ON_EN_CHANGE(IDC_CODEPAGE, OnChangeCodepage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsPPG message handlers

// Browsing for a binary resource file to be used as the default one:
void CSettingsPPG::OnBrowse()
{
// Deactivating tooltips: *****
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;
// ****************************

	CFileDialogEx dlg(TRUE, NULL, NULL, OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, _T("Binary Files (*.exe;*.dll;*.ocx)|*.exe;*.dll;*.ocx|All Files (*.*)|*.*||"), this);
	CString dir;
	int index = m_resource.ReverseFind('\\');
	if(index != -1)
		dir = m_resource.Left(index);
	if(dlg.DoModal(dir) == IDOK)
	{
		CString resource = dlg.GetPathName();
		if(pSheet->m_Output.SetDefaultRes(resource))
		{
			m_resource = resource;
			UpdateData(FALSE);
			pSheet->m_Input.SetModified();
		}
		else
			AfxMessageBox(IDS_FAILEDSETRESOURCE, MB_OK|MB_ICONERROR);
	}

// Reactivating tooltips:
	pSheet->m_tm.Active = bActive;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM /*pData*/)
{
    TCHAR szDir[MAX_PATH];
    switch(uMsg) 
    {
    case BFFM_INITIALIZED: 
		{
			_tcscpy(szDir, pSettings->m_dir);
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
			break;
        }        
    case BFFM_SELCHANGED:
		{
			if(SHGetPathFromIDList((LPITEMIDLIST)lp ,szDir))
				SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
			break;
		}
    default:
        break;
    }
    return 0;
}

void CSettingsPPG::OnAddpath()
{
// Deactivating tooltips: *****
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;
// ****************************

    LPMALLOC pMalloc;
    if(SUCCEEDED(SHGetMalloc(&pMalloc))) 
    {
        TCHAR szTitle[] = _T("Select a directory, and press OK...");
        BROWSEINFO bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.hwndOwner = m_hWnd;
        bi.pszDisplayName = NULL;
        bi.lpszTitle = szTitle;
        bi.pidlRoot = NULL;
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        bi.lpfn = BrowseCallbackProc;
        LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
        if(pidl) 
        {
            TCHAR szDir[MAX_PATH];
            if(SHGetPathFromIDList(pidl, szDir ))
			{
				m_dir = szDir;
				m_DirList.push_back(m_dir);
				m_paths.AddString(m_dir);
				CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
				pSheet->m_Output.AddFilePath(m_dir);
				pSheet->m_Input.SetModified();
			}
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
	}

// Reactivating tooltips:
	pSheet->m_tm.Active = bActive;
}

BOOL CSettingsPPG::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH), MAX_PATH);
	path.ReleaseBuffer();
	m_resource = path;
	UpdateData(FALSE);
	int index = path.ReverseFind('\\');
	if(index != -1)
	{
		path = path.Left(index);
		m_DirList.push_back(path);
		m_paths.AddString(path);
		SetCurrentDirectory(_T("c:\\temp\\"));
	}
	
	// Attaching a tooltip to this property page:
	((CParserTestSheet*)GetParent())->m_tm.windows.Add(m_hWnd, _T("<txt res='html:idr_propsettings'>"));

	return TRUE;
}

void CSettingsPPG::OnHideTooltips()
{
	UpdateData();
	((CParserTestSheet*)GetParent())->m_tm.Active = m_bHideTooltips?false:true;
}

void CSettingsPPG::OnChangeCodepage()
{
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	pSheet->m_Input.SetModified();
}

