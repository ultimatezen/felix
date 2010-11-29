// OutputPPG.cpp : implementation file
//

#include "stdafx.h"
#include "Parser Test.h"
#include "OutputPPG.h"
#include "ParserTestSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const LPCTSTR strTrue = _T("True");
const LPCTSTR strFalse = _T("False");

/////////////////////////////////////////////////////////////////////////////
// COutputPPG property page

IMPLEMENT_DYNCREATE(COutputPPG, CPropertyPage)

COutputPPG::COutputPPG() : CPropertyPage(COutputPPG::IDD)
{
	// No help button on this tab page:
	m_psp.dwFlags &= ~PSP_HASHELP;

	m_Bitmap = NULL;
}

COutputPPG::~COutputPPG()
{
	if(m_Bitmap)
		DeleteObject(m_Bitmap);
}

void COutputPPG::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputPPG)
	DDX_Control(pDX, IDC_EVENTS, m_events);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COutputPPG, CPropertyPage)
	//{{AFX_MSG_MAP(COutputPPG)
	ON_BN_CLICKED(ID_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputPPG message handlers

BOOL COutputPPG::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	// Creating the QHTML document output area:
	CRect brect;
	GetDlgItem(IDC_PICTURE)->GetWindowRect(brect);
	ScreenToClient(brect);
	m_bmpCtrl.Create(WS_CHILD|WS_VISIBLE, brect, this, IDC_PICTURE);

	// Attaching a tooltip to this tab page:
	((CParserTestSheet*)GetParent())->m_tm.windows.Add(m_hWnd, _T("<txt res='html:idr_propoutput'>"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#ifdef _UNICODE // Making it possible to compile the project even without Unicode
void COutputPPG::UpdateOutput(LPCTSTR text)
#else
void COutputPPG::UpdateOutput(BSTR text)
#endif
{
	m_events.ResetContent();

	if(text)
		SetSourceText(text);
//************************
	SYSTEMTIME s1;
	GetSystemTime(&s1);

	RecalculateLayout();

	SYSTEMTIME s2;
	GetSystemTime(&s2);
	int Delay = s2.wMilliseconds - s1.wMilliseconds;
	if(Delay < 0)
		Delay = 1000 - s1.wMilliseconds + s2.wMilliseconds;
//************************


//************************
	GetSystemTime(&s1);
//************************

	CPaintDC dc(this);
	HDC memDC = ::CreateCompatibleDC(dc);
	if(m_Bitmap)
		DeleteObject(m_Bitmap);
	m_Bitmap = ::CreateCompatibleBitmap(dc, Width, Height);
	HGDIOBJ old = ::SelectObject(memDC, m_Bitmap);
	Draw(memDC, 0, 0);
	::SelectObject(memDC, old);
	::DeleteDC(memDC);
	m_bmpCtrl.SetBitmap(m_Bitmap);


//************************
	GetSystemTime(&s2);
	int Display = s2.wMilliseconds - s1.wMilliseconds;
	if(Display < 0)
		Display = 1000 - s1.wMilliseconds + s2.wMilliseconds;
//************************

	CString width;
	width.Format(_T("%d"), Width);
	GetDlgItem(IDC_WIDTH)->SetWindowText(width);
	CString height;
	height.Format(_T("%d"), Height);
	GetDlgItem(IDC_HEIGHT)->SetWindowText(height);
	CString delay;
	if(!Delay)
		delay = _T("< 10");
	else
		delay.Format(_T("%d"), Delay);
	GetDlgItem(IDC_DELAY)->SetWindowText(delay);
	CString display;
	if(!Display)
		display = _T("< 10");
	else
		display.Format(_T("%d"), Display);
	GetDlgItem(IDC_DISPLAY)->SetWindowText(display);
}

BOOL COutputPPG::OnSetActive()
{
	CInputPPG & Input = ((CParserTestSheet*)GetParent())->m_Input;
	CSettingsPPG & Settings = ((CParserTestSheet*)GetParent())->m_Settings;
	if(Input.IsChanged())
	{
		TxtCodePage = Settings.m_cp;
		UpdateOutput(Input.GetText());
	}
	return CPropertyPage::OnSetActive();
}

void COutputPPG::OnUpdate() 
{
	UpdateOutput();
}

void COutputPPG::OnRecalculate(bool Begin, bool SourceChanged)
{
	CString text;
	text.Format(_T("OnRecalculate (%s, %s)"), Begin?strTrue:strFalse, SourceChanged?strTrue:strFalse);
	AddEvent(text);
}

void COutputPPG::OnDocumentGetSize(LPCTSTR DocumentPath, LPCTSTR Param, short & Width, short & Height)
{
	CString text;
	text.Format(_T("OnDocumentGetSize (\"%s\", \"%s\", %d, %d)"), DocumentPath?DocumentPath:_T(""), Param?Param:_T(""), Width, Height);
	AddEvent(text);
}

void COutputPPG::OnControlGetSize(long ID, LPCTSTR Name, short & Width, short & Height, LPCTSTR Param)
{
	CString text;
	text.Format(_T("OnControlGetSize (%d, \"%s\", %d, %d, \"%s\")"), ID, Name?Name:_T(""), Width, Height, Param?Param:_T(""));
	AddEvent(text);
}

void COutputPPG::OnDocumentDraw(LPCTSTR DocumentPath, LPCTSTR Param, HDC DC, short x, short y, short Width, short Height)
{
	CString text;
	text.Format(_T("OnDocumentDraw (\"%s\", \"%s\", %X, %d, %d, %d, %d)"), DocumentPath?DocumentPath:_T(""), Param?Param:_T(""), DC, x, y, Width, Height);
	AddEvent(text);
}

BSTR COutputPPG::OnGetText(LPCTSTR FilePath, LPCTSTR Res, long Size, long ID)
{
	CString text;
	text.Format(_T("OnGetText (\"%s\", \"%s\", %d, %d)"), FilePath?FilePath:_T(""), Res?Res:_T(""), Size, ID);
	AddEvent(text);

	return NULL;
}

void COutputPPG::OnSetDefaults()
{
	AddEvent(_T("OnSetDefaults ()"));
}

void COutputPPG::OnSourceChanged(bool IsEmpty)
{
	CString text;
	text.Format(_T("OnSourceChanged (%s)"), IsEmpty?strTrue:strFalse);
	AddEvent(text);
}

// Each CVXPxxx class is derived from an error handler that
// has this virtual method. We override this method here
// to display information about exceptions fired by the VXP Library:
void COutputPPG::OnError(HRESULT ErrorCode, LPCTSTR ErrorMsg)
{
	CParserTestSheet * pSheet = (CParserTestSheet*)GetParent();
	// Do not show any tooltip while a message box is displayed:
	bool bActive = pSheet->m_tm.Active;
	pSheet->m_tm.Active = false;

	CString error;
	error.Format(IDS_ERROR, ErrorCode, ErrorMsg);
	AfxMessageBox(error, MB_OK|MB_ICONERROR);

	// Now can display tooltips:
	pSheet->m_tm.Active = bActive;
}

void COutputPPG::AddEvent(LPCTSTR txt)
{
	m_events.AddString(txt);
	
	// Find the longest string in the list box.
	CString str;
	CSize sz;
	int dx=0;
	CDC * pDC = m_events.GetDC();
	CFont * pFont = m_events.GetFont();
	CGdiObject * pObject = pDC->SelectObject(pFont);
	for(int i = 0;i < m_events.GetCount();i ++)
	{
		m_events.GetText(i, str);
		sz = pDC->GetTextExtent(str);
		if (sz.cx > dx)
			dx = sz.cx;
	}
	pDC->SelectObject(pObject);
	m_events.ReleaseDC(pDC);
	m_events.SetHorizontalExtent(dx + 15);
}
