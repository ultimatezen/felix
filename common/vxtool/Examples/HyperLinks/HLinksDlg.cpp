// HLinksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HLinks.h"
#include "HLinksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_tt.commands.Add(IDC_REF, _T("<txt res='html:tt_ref'>"));
	m_tt.ShowDelay = 100; // Show tooltips with just 0.1 second delay
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_REF, m_ref);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Making sure that tooltips won't be shows when the window
	// is not active:
	m_tt.Activator = m_hWnd;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// CHLinksDlg dialog

CHLinksDlg::CHLinksDlg(CWnd* pParent):CDialog(CHLinksDlg::IDD, pParent)
{
	// Loading the icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	///////////////////////////////////////////////////////////////
	// Setting up QHTML contents and URL for all QHTML Hyperlinks:
	m_qhtml1[hlNormal] = _T("<txt res='html:idr_home'>");
	m_qhtml1[hlHighlight] = _T("<txt res='html:idr_home_sel'>");
	m_qhtml1.url = _T("http://www.tooltips.net");

	m_qhtml2[hlNormal] = _T("<txt res='html:idr_business'>");
	m_qhtml2[hlHighlight] = _T("<txt res='html:idr_business_sel'>");
	m_qhtml2.url = _T("http://www.tooltips.net/business.html");

	m_qhtml3[hlNormal] = _T("<txt res='html:idr_concept'>");
	m_qhtml3[hlHighlight] = _T("<txt res='html:idr_concept_sel'>");
	m_qhtml3.url = _T("http://www.tooltips.net/concept.html");

	m_qhtml4[hlNormal] = _T("<txt res='html:idr_conference'>");
	m_qhtml4[hlHighlight] = _T("<txt res='html:idr_conference_sel'>");
	m_qhtml4.url = _T("http://www.tooltips.net/cgi-bin/yabb/YaBB.pl");

	m_qhtml5[hlNormal] = _T("<txt res='html:idr_contacts'>");
	m_qhtml5[hlHighlight] = _T("<txt res='html:idr_contacts_sel'>");
	m_qhtml5.url = _T("http://www.tooltips.net/contacts.html");

	m_qhtml6[hlNormal] = _T("<txt res='html:idr_downloads'>");
	m_qhtml6[hlHighlight] = _T("<txt res='html:idr_downloads_sel'>");
	m_qhtml6.url = _T("http://www.tooltips.net/downloads.html");

	m_qhtml7[hlNormal] = _T("<txt res='html:idr_news'>");
	m_qhtml7[hlHighlight] = _T("<txt res='html:idr_news_sel'>");
	m_qhtml7.url = _T("http://www.tooltips.net/news.html");

	m_qhtml8[hlNormal] = _T("<txt res='html:idr_screenshots'>");
	m_qhtml8[hlHighlight] = _T("<txt res='html:idr_screenshots_sel'>");
	m_qhtml8.url = _T("http://www.tooltips.net/screenshots.html");

	m_qhtml9[hlNormal] = _T("<txt res='html:idr_support'>");
	m_qhtml9[hlHighlight] = _T("<txt res='html:idr_support_sel'>");
	m_qhtml9.url = _T("http://www.tooltips.net/support.html");
	////////////////////////////////////////////////////////////////


	/////////////////////////////////////////
	// Setting up Simpsons:
	m_Bart[hlNormal] = _T("<txt res='html:idr_bart'>");
	m_Bart[hlHighlight] = _T("<txt res='html:idr_bart_sel'>");
	m_Bart.style &= ~HLS_TRUNCATE_LINK; // Telling not to truncate this hyperlink

	m_UseHand[hlNormal] = _T("<txt res='html:idr_checked'>");
	m_UseHand[hlHighlight] = _T("<txt res='html:idr_checked_sel'>");
	m_UseHand[hlNormalVisited] = _T("<txt res='html:idr_unchecked'>");
	m_UseHand[hlHighlightVisited] = _T("<txt res='html:idr_unchecked_sel'>");
	/////////////////////////////////////////

	// Settip up the Tip hyperlink
	m_tip[hlNormal] = _T("<txt res='html:idr_tip'>");
	m_tip[hlHighlight] = _T("<txt res='html:idr_tip_sel'>");
	m_tip.url = _T("http://www.tooltips.net");

	// Setting up tooltips contents for all objects;
	// This code can be optimized through a function that will take
	// the control ID and name to be added to "tt_" to form the text
	// and then add the tooltip.
	m_tt.commands.Add(IDC_BART, _T("<txt res='html:tt_bart'>"));
	m_tt.commands.Add(IDC_USEHAND, _T("<txt res='html:tt_homer'>"));
	m_tt.commands.Add(IDC_URL, _T("<txt res='html:tt_url'>"));
	m_tt.commands.Add(IDC_HOME, _T("<txt res='html:tt_home'>"));
	m_tt.commands.Add(IDC_DOWNLOADS, _T("<txt res='html:tt_downloads'>"));
	m_tt.commands.Add(IDC_CONCEPT, _T("<txt res='html:tt_concept'>"));
	m_tt.commands.Add(IDC_SCREENSHOTS, _T("<txt res='html:tt_screenshots'>"));
	m_tt.commands.Add(IDC_NEWS, _T("<txt res='html:tt_news'>"));
	m_tt.commands.Add(IDC_CONFERENCE, _T("<txt res='html:tt_conference'>"));
	m_tt.commands.Add(IDC_BUSINESS, _T("<txt res='html:tt_business'>"));
	m_tt.commands.Add(IDC_SUPPORT, _T("<txt res='html:tt_support'>"));
	m_tt.commands.Add(IDC_CONTACTS, _T("<txt res='html:tt_contacts'>"));
	m_tt.commands.Add(IDC_TIP, _T("<txt res='html:tt_tip'>"));	
	m_tt.commands.Add(IDC_SIMPLE, _T("<txt res='html:tt_simple'>"));
	m_tt.commands.Add(IDC_MULTILINE, _T("<txt res='html:tt_multi'>"));
	m_tt.commands.Add(IDC_WEBREF, _T("<txt res='html:tt_webref'>"));
	m_tt.commands.Add(IDC_MAILTO, _T("<txt res='html:tt_mailto'>"));
	m_tt.commands.Add(IDOK, _T("<txt res='html:tt_exit'>"));
	m_tt.commands.Add(IDC_LEFALIGNED, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_RIGHTALIGNED, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_CENTERALIGNED, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_VLEFT, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_VMIDDLE, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_VRIGHT, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_CENTERMULTI, _T("<txt res='html:tt_align'>"));
	m_tt.commands.Add(IDC_DISABLE, _T("<txt res='html:tt_disable'>"));
	m_tt.commands.Add(IDC_SINK, _T("<txt res='html:tt_sink'>"));

	m_tt.ShowDelay = 100; // Show tooltips with just 0.1 second delay
}

void CHLinksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHLinksDlg)
	DDX_Control(pDX, IDC_BART, m_Bart);
	DDX_Control(pDX, IDC_URL, m_StatusBar);
	DDX_Control(pDX, IDC_USEHAND, m_UseHand);
	DDX_Control(pDX, IDC_TIP, m_tip);
	DDX_Control(pDX, IDC_SUPPORT, m_qhtml9);
	DDX_Control(pDX, IDC_SCREENSHOTS, m_qhtml8);
	DDX_Control(pDX, IDC_NEWS, m_qhtml7);
	DDX_Control(pDX, IDC_DOWNLOADS, m_qhtml6);
	DDX_Control(pDX, IDC_CONTACTS, m_qhtml5);
	DDX_Control(pDX, IDC_CONFERENCE, m_qhtml4);
	DDX_Control(pDX, IDC_CONCEPT, m_qhtml3);
	DDX_Control(pDX, IDC_BUSINESS, m_qhtml2);
	DDX_Control(pDX, IDC_HOME, m_qhtml1);
	DDX_Control(pDX, IDC_WEBREF, m_hlink11);
	DDX_Control(pDX, IDC_VRIGHT, m_hlink10);
	DDX_Control(pDX, IDC_VMIDDLE, m_hlink9);
	DDX_Control(pDX, IDC_VLEFT, m_hlink8);
	DDX_Control(pDX, IDC_SIMPLE, m_hlink7);
	DDX_Control(pDX, IDC_RIGHTALIGNED, m_hlink6);
	DDX_Control(pDX, IDC_MULTILINE, m_hlink5);
	DDX_Control(pDX, IDC_MAILTO, m_hlink4);
	DDX_Control(pDX, IDC_LEFALIGNED, m_hlink3);
	DDX_Control(pDX, IDC_CENTERMULTI, m_hlink2);
	DDX_Control(pDX, IDC_CENTERALIGNED, m_hlink1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHLinksDlg, CDialog)
	//{{AFX_MSG_MAP(CHLinksDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BART, OnBart)
	ON_BN_CLICKED(IDC_DISABLE, OnDisable)
	ON_BN_CLICKED(IDC_SINK, OnSink)
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLinksDlg message handlers

BOOL CHLinksDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

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
	
	// Making sure that tooltips won't be shows when the window
	// is not active:
	m_tt.Activator = m_hWnd;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHLinksDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CHLinksDlg::OnPaint() 
{
	if(IsIconic())
	{
		CPaintDC dc(this); // device context for painting
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
		CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHLinksDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHLinksDlg::OnHighligh(CHyperLink * pLink, bool bHighlighted)
{
	if(bHighlighted)							// If highlighted a hyperlink
		m_StatusBar.SetWindowText(pLink->url);	// Set the URL text to be displayed
	else	// If none, or unhighlighted
		m_StatusBar.SetWindowText(NULL);		// Remove any text
}

template<class T>
void TCursorOverride<T>::OnHighlight(bool bHighlighted)
{
	// Allowing to update contents of the IDC_URL static control:
	((CHLinksDlg*)AfxGetMainWnd())->OnHighligh(this, bHighlighted);
}

void CHLinksDlg::OnBart()
{
	// Make sure the hyperlink is released (unhighlighted):
	m_Bart.ReleaseLink();
	
	// Showing the About Box
	CAboutDlg dlg;
	dlg.DoModal();
}

void CHLinksDlg::OnDisable()
{
	void * links[] = {&m_hlink1, &m_hlink2, &m_hlink3, &m_hlink4, &m_hlink5, &m_hlink6, &m_hlink7, &m_hlink8, &m_hlink9, &m_hlink10, &m_hlink11,
						&m_qhtml1, &m_qhtml2, &m_qhtml3, &m_qhtml4, &m_qhtml5, &m_qhtml6, &m_qhtml7, &m_qhtml8, &m_qhtml9,
						&m_Bart, &m_UseHand, &m_tip};

	bool bEnable = !((CButton*)GetDlgItem(IDC_DISABLE))->GetCheck();
	for(int i=0;i<sizeof(links)/sizeof(void*);i ++)
	{
		CHyperLink * link = (CHyperLink*)links[i];
		link->EnableWindow(bEnable);
		link->UpdateLink();
	}
	GetDlgItem(IDC_SINK)->EnableWindow(bEnable);
}

void CHLinksDlg::OnSink() 
{
	void * links[] = {&m_hlink1, &m_hlink2, &m_hlink3, &m_hlink4, &m_hlink5, &m_hlink6, &m_hlink7, &m_hlink8, &m_hlink9, &m_hlink10, &m_hlink11,
						&m_qhtml1, &m_qhtml2, &m_qhtml3, &m_qhtml4, &m_qhtml5, &m_qhtml6, &m_qhtml7, &m_qhtml8, &m_qhtml9,
						&m_Bart, &m_UseHand, &m_tip};

	bool bSink = ((CButton*)GetDlgItem(IDC_SINK))->GetCheck() != 0;
	for(int i=0;i<sizeof(links)/sizeof(void*);i ++)
		((CHyperLink*)links[i])->bSunken = bSink;
}

// Handles event WM_SETTINGCHANGE:
void CHLinksDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);

	void * links[] = {&m_qhtml1, &m_qhtml2, &m_qhtml3, &m_qhtml4, &m_qhtml5, &m_qhtml6, &m_qhtml7, &m_qhtml8, &m_qhtml9,
					  &m_Bart, &m_UseHand, &m_tip};

	for(int i=0;i<sizeof(links)/sizeof(void*);i ++)
		((CVXPHyperLink*)links[i])->RecalculateLayout();

	// Updating border color for tooltips:
	m_tt.BorderColor = GetSysColor(COLOR_WINDOWFRAME);
}
