// DynamicControlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicControls.h"
#include "DynamicControlsDlg.h"

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
// CDynamicControlsDlg dialog

CDynamicControlsDlg::CDynamicControlsDlg(CWnd* pParent /*=NULL*/):CDialog(CDynamicControlsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDynamicControlsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Setting up contents of the two hyperlinks:

	m_hlink1[hlNormal] = _T("<txt res='html:idr_hlink1'>");
	m_hlink1[hlHighlight] = _T("<txt res='html:idr_hlink1_sel'>");

	m_hlink2[hlNormal] = _T("<txt res='html:idr_hlink2'>");
	m_hlink2[hlHighlight] = _T("<txt res='html:idr_hlink2_sel'>");

	SetSourceText(_T("<txt res='html:idr_maindoc'>"));	// The main document

	RecalculateLayout();	// Because we have dynamic controls we have
							// to recalculate the document and all controls here
}

void CDynamicControlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynamicControlsDlg)
	DDX_Control(pDX, IDC_HYPERLINK1, m_hlink1);
	DDX_Control(pDX, IDC_HYPERLINK2, m_hlink2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDynamicControlsDlg, CDialog)
	//{{AFX_MSG_MAP(CDynamicControlsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_HYPERLINK1, OnHyperLink1)
	ON_BN_CLICKED(IDC_HYPERLINK2, OnHyperLink2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicControlsDlg message handlers

BOOL CDynamicControlsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CDynamicControlsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
		CDialog::OnSysCommand(nID, lParam);
}

// This is the key event called by the parser to get sizes
// for each tag CONTROL:
void CDynamicControlsDlg::OnControlGetSize(long ID, LPCTSTR Name, short & Width, short & Height, LPCTSTR Param)
{
// For both controls we return size of the hyperlink
// in the normal state assuming that in all other states
// size of hyperlinks won't exceed that in the normal state.
	switch(ID)
	{
	case 1:
		{
			Width = m_hlink1[hlNormal].Width;
			Height = m_hlink1[hlNormal].Height;
			break;
		}
	case 2:
		{
			Width = m_hlink2[hlNormal].Width;
			Height = m_hlink2[hlNormal].Height;
			break;
		}
	default:
		break;
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CDynamicControlsDlg::OnPaint() 
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
	short offset_x = (short)max((rect.right - Width)/2, 0);
	short offset_y = (short)max((rect.bottom - Height)/2, 0);

	// Drawing our QHTML document in the middle of the dialog box:
	Draw(dc, offset_x, offset_y);

	short x, y, w, h;

	// For the first hyperlink:
	if(GetControlPos(1, x, y, w, h))
		m_hlink1.SetWindowPos(NULL, offset_x + x, offset_y + y, w, h, SWP_NOZORDER);

	// For the second hyperlink:
	if(GetControlPos(2, x, y, w, h))
		m_hlink2.SetWindowPos(NULL, offset_x + x, offset_y + y, w, h, SWP_NOZORDER);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDynamicControlsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDynamicControlsDlg::OnHyperLink1()
{
	// We display here a message with text taken from
	// custom attribute "msg" of the QHTML document:
	VARIANT msg = GetControlAttr(1, _T("msg"));
	if(msg.vt != VT_EMPTY)	// If found the attribute
		AfxMessageBox(_bstr_t(msg), MB_OK|MB_ICONINFORMATION);
}

void CDynamicControlsDlg::OnHyperLink2()
{
	// We display here a message with text taken from
	// custom attribute "msg" of the QHTML document:
	VARIANT msg = GetControlAttr(2, _T("msg"));
	if(msg.vt != VT_EMPTY)	// If found the attribute
		AfxMessageBox(_bstr_t(msg), MB_OK|MB_ICONINFORMATION);
}

void CDynamicControlsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Invalidate();	// Necessary to reposition drawing in the middle
					// of the dialog box
}
