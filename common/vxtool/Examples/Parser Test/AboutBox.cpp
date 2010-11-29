// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "Parser Test.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog


CAboutBox::CAboutBox(CWnd* pParent) : CDialog(CAboutBox::IDD, pParent)
{
	// Draw QHTML using transparency:
	m_background.Transparent = true;

	// Loading the document:
	m_background = _T("<txt res='html:idr_aboutbox'>");
}


void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAboutBox)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBox message handlers

void CAboutBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// *** Filling the entire dialog box with gradient: *************************************
	RECT r;
	GetClientRect(&r);
	m_background.GradientFillRect(dc, RGB(255,255,150), RGB(255,150,150), 0, 0, (short)r.right, (short)r.bottom, false);
	// **************************************************************************************

	// Drawing the QHTML document on the dialog box:
	m_background.Draw(dc, 0, 0);
}
