// StaticControlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StaticControls.h"
#include "StaticControlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticControlsDlg dialog

CStaticControlsDlg::CStaticControlsDlg(CWnd* pParent /*=NULL*/):CDialog(CStaticControlsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaticControlsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_html = _T("<txt res='html:idr_skin_default'>");

	// If you uncomment this line you also will be able
	// to remove bgcolor=3dface from tag <BODY>
	//	m_html.Transparent = true;
}

void CStaticControlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaticControlsDlg)
	DDX_Control(pDX, IDC_SKIN3, m_skin3);
	DDX_Control(pDX, IDC_SKIN2, m_skin2);
	DDX_Control(pDX, IDC_SKIN1, m_skin1);
	DDX_Control(pDX, IDC_EDITBOX, m_editbox);
	DDX_Control(pDX, IDC_CHECKBOX, m_checkbox);
	DDX_Control(pDX, IDC_HYPERLINK, m_hlink1);
	DDX_Control(pDX, IDC_THIRDBUTTON, m_button3);
	DDX_Control(pDX, IDC_SECONFBUTTON, m_button2);
	DDX_Control(pDX, IDC_FIRSTBUTTON, m_button1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStaticControlsDlg, CDialog)
	//{{AFX_MSG_MAP(CStaticControlsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECKBOX, OnCheckbox)
	ON_BN_CLICKED(IDC_SKIN1, OnSkinDefault)
	ON_BN_CLICKED(IDC_SKIN2, OnSkinSky)
	ON_BN_CLICKED(IDC_SKIN3, OnSkinFire)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticControlsDlg message handlers

BOOL CStaticControlsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_skin1.SetCheck(1);

	InitControls();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStaticControlsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
		CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStaticControlsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	if (IsIconic())
	{
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
	
	// Calculating x,y of the document to be placed in the middle
	// of the dialog box:
	short offset_x = (short)max((rect.right - m_html.Width)/2, 0);
	short offset_y = (short)max((rect.bottom - m_html.Height)/2, 0);

	// Drawing our QHTML document in the middle of the dialog box;
	// By using the checkbox we show that QHTML even can be used
	// just to format user interface:
	if(m_checkbox.GetCheck() != 1)
		m_html.Draw(dc, offset_x, offset_y);

	short x, y, w, h;
	// List of all controls that we try to embed in our QHTML document:
	CWnd * controls[] = {&m_button1, &m_button2, &m_button3, &m_hlink1, &m_editbox, &m_checkbox, &m_skin1, &m_skin2, &m_skin3};

	// For all controls involved in the coordinate adjustment:
	for(int i=0;i < sizeof(controls)/sizeof(CWnd*);i++)
		if(m_html.GetControlPos(i + 1, x, y, w, h)) // Note: we have all our ID-s for controls from 1 to 9
			controls[i]->SetWindowPos(NULL, offset_x + x, offset_y + y, w, h, SWP_NOZORDER);

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStaticControlsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStaticControlsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();
}

void CStaticControlsDlg::OnCheckbox()
{
	Invalidate();
}

void CStaticControlsDlg::OnSkinDefault()
{
	m_html = _T("<txt res='html:idr_skin_default'>"); // Loading a new QHTML document
	InitControls();	// Reinitializing parameters of all controls from custom attributes
	Invalidate();	// Updating the screen
}

void CStaticControlsDlg::OnSkinSky()
{
	m_html = _T("<txt res='html:idr_skin_sky'>");	// Loading a new QHTML document
	InitControls();	// Reinitializing parameters of all controls from custom attributes
	Invalidate();	// Updating the screen
}

void CStaticControlsDlg::OnSkinFire()
{
	m_html = _T("<txt res='html:idr_skin_fire'>");	// Loading a new QHTML document
	InitControls();	// Reinitializing parameters of all controls from custom attributes
	Invalidate();	// Updating the screen
}

void CStaticControlsDlg::InitControls()
{
// **************************************************************
// Here we initialize some controls with values taken from custom
// attributes of their tags:

	// For the first button:
	VARIANT v = m_html.GetControlAttr(1, _T("caption"));
	if(v.vt != VT_EMPTY) // If a valid text string was found
		m_button1.SetWindowText(_bstr_t(v));

	// For the second button:
	v = m_html.GetControlAttr(2, _T("caption"));
	if(v.vt != VT_EMPTY) // If a valid text string was found
		m_button2.SetWindowText(_bstr_t(v));

	// For the third button:
	v = m_html.GetControlAttr(3, _T("caption"));
	if(v.vt != VT_EMPTY) // If a valid text string was found
		m_button3.SetWindowText(_bstr_t(v));

	// For the editbox control:
	v = m_html.GetControlAttr(5, _T("text"));
	if(v.vt != VT_EMPTY) // If a valid text string was found
		m_editbox.SetWindowText(_bstr_t(v));

	// For the hyperlink:
	v = m_html.GetControlAttr(4, _T("url"));
	if(v.vt != VT_EMPTY) // If a valid text string was found
		m_hlink1.url = (LPCTSTR)_bstr_t(v);

	// Use custom attribute COLOR for normal and highlighted state:
	v = m_html.GetControlAttr(4, _T("color"), taColor);
	if(v.vt != VT_EMPTY) // If a valid color attribute was found
	{
		m_hlink1.SetHLColor(hlNormal, v.lVal, false);
		m_hlink1.SetHLColor(hlHighlight, v.lVal, false);
	}
// **************************************************************
}
