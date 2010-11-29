// StaticTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StaticText.h"
#include "StaticTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticTextDlg dialog

CStaticTextDlg::CStaticTextDlg(CWnd* pParent /*=NULL*/)	: CDialog(CStaticTextDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_Static = _T("<txt res='html:idr_static'>"); // Loading contents of the static control
}

void CStaticTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaticTextDlg)
	DDX_Control(pDX, IDC_SUNKEN, m_Sunken);
	DDX_Control(pDX, IDC_DISABLED, m_Disabled);
	DDX_Control(pDX, IDC_COMPLEX, m_Static);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStaticTextDlg, CDialog)
	//{{AFX_MSG_MAP(CStaticTextDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DISABLED, OnDisabled)
	ON_BN_CLICKED(IDC_SUNKEN, OnSunken)
	ON_MESSAGE(WM_SETTINGCHANGE, OnSettingsChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticTextDlg message handlers

BOOL CStaticTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	if(m_Static.GetStyle() & WS_DISABLED)
	{
		m_Disabled.SetCheck(1);
		m_Sunken.EnableWindow(FALSE);
	}

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CStaticTextDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		CRect rect;
		GetClientRect(&rect);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
		CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStaticTextDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStaticTextDlg::OnDisabled()
{
	bool bChecked = (m_Disabled.GetCheck() != 0);
	
	m_Static.EnableWindow(!bChecked);
	
	m_Static.UpdateControl();	// This is more effective than calling Invalidate()

	m_Sunken.EnableWindow(!bChecked);
}

void CStaticTextDlg::OnSunken()
{
	// Setting Sunken style for the control
	m_Static.SetSunken(m_Sunken.GetCheck() == 1);
}

// Handling WM_SETTINGCHANGE allows us to update appearance of all
// controls dynamically when visual parameters change in the system:
LRESULT CStaticTextDlg::OnSettingsChanged(WPARAM, LPARAM)
{
	m_Static.RecalculateLayout();	// Update the control's layout
	return Default();
}
