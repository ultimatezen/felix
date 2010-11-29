// CustomTooltipsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CustomTooltips.h"
#include "CustomTooltipsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRADIENT_START_COLOR	RGB(170, 210, 230)
#define GRADIENT_END_COLOR		RGB(255, 255, 255)

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsDlg dialog

CCustomTooltipsDlg::CCustomTooltipsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomTooltipsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomTooltipsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_hBrush = ::CreateSolidBrush(GRADIENT_START_COLOR);

	m_tt.ShowDelay = 300;	// Show tooltips after 0.3 second
	
	m_tt.MouseClickResponse = mcrHideWhenShown; // Hide the tooltip on click only
												// when the tooltip is shown completely.

	// Attaching tooltip to the exit button:
	m_tt.commands.Add(IDOK, _T("<txt res='html:idr_exit'>"));
	
	// Attaching tooltip to the toggle checkbox:
	m_tt.commands.Add(IDC_ONEHEADER, _T("<txt res='html:idr_toggle'>"));
}

CCustomTooltipsDlg::~CCustomTooltipsDlg()
{
	DeleteObject(m_hBrush);
}

void CCustomTooltipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomTooltipsDlg)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCustomTooltipsDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomTooltipsDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ONEHEADER, OnOneHeaderClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsDlg message handlers

BOOL CCustomTooltipsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// This is to avoid showing tooltips for the dialog box
	// when it is not active:
	m_tt.Activator = m_hWnd;
	
// Attaching tooltip to the dialog box itself:
	m_tt.windows.Add(m_hWnd, _T("<txt res='html:idr_dialog'>"));

// Setting extended style for the list control:
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_List.SetOneHeaderTooltip(false);

// Adding items to the list control:

	// Items:
	LPCTSTR Item0 = _T("First Item");
	LPCTSTR Item1 = _T("Second Item");

	// Subitems:
	LPCTSTR Item01 = _T("Normal length text");
	LPCTSTR Item11 = _T("This text is so long it gets splitted into several lines when shown in the tooltip");

	m_List.InsertColumn(0, "Name", LVCFMT_LEFT, max(m_List.GetStringWidth(Item0), m_List.GetStringWidth(Item1)) + 12);
	m_List.InsertColumn(1, "Details", LVCFMT_LEFT, max(m_List.GetStringWidth(Item01), m_List.GetStringWidth(Item11)) + 12);

	m_List.InsertItem(0, Item0);
	m_List.SetItemText(0, 1, Item01);

	m_List.InsertItem(1, Item1);
	m_List.SetItemText(1, 1, Item11);
	
// Adding items to the tree control:

	HTREEITEM hItem = m_Tree.InsertItem("Root Item");
	m_Tree.InsertItem("First Branch", hItem);
	m_Tree.InsertItem("Second Branch", hItem);
	m_Tree.InsertItem("Third Branch, which is a very long item to be splitted into several lines, and even truncated at the end with three dots.", hItem);

	m_Tree.Expand(hItem, TVE_EXPAND);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCustomTooltipsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Drawing gradient through the whole dialog box:
	RECT r;
	GetClientRect(&r);
	m_tt.GradientFillRect(dc, GRADIENT_START_COLOR, GRADIENT_END_COLOR, 0, 0, (short)r.right, (short)r.bottom, false);

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCustomTooltipsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// We override this event just to provide background and foreground colors
// for our Static Text controls:
HBRUSH CCustomTooltipsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC && m_tt.IsAttached()) // If Static Control, and VXPLib is available
	{
		pDC->SetTextColor(RGB(150, 0, 0)); // Dark Red color for all static controls
		pDC->SetBkMode(TRANSPARENT);	
		return m_hBrush;
	}
	
	return hbr;
}

void CCustomTooltipsDlg::OnOneHeaderClick()
{
	m_List.SetOneHeaderTooltip(GetDlgItem(IDC_ONEHEADER)->SendMessage(BM_GETCHECK, 0, 0) == BST_CHECKED);
	
}
