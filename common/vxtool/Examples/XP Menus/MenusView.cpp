// MenusView.cpp : implementation of the CMenusView class
//

#include "stdafx.h"
#include "Menus.h"

#include "MenusDoc.h"
#include "MenusView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenusView

IMPLEMENT_DYNCREATE(CMenusView, CView)

BEGIN_MESSAGE_MAP(CMenusView, CView)
	//{{AFX_MSG_MAP(CMenusView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenusView construction/destruction

CMenusView::CMenusView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMenusView drawing

void CMenusView::OnDraw(CDC* pDC)
{
	CMenusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CVXPQuickHTML & info = ((CMainFrame*)AfxGetMainWnd())->m_info;

	// Drawing the Intro document in the middle of the screen:

	RECT r;
	GetClientRect(&r);
	
	int x = (r.right - info.Width)/2;
	int y = (r.bottom - info.Height)/2;

	// It is perhaps better not to shift x,y to negative values,
	// so we rectify them here:
	if(x < 0)
		x = 0;
	if(y < 0)
		y = 0;

	info.Draw(*pDC, (short)x, (short)y);
}

/////////////////////////////////////////////////////////////////////////////
// CMenusView diagnostics

#ifdef _DEBUG
void CMenusView::AssertValid() const
{
	CView::AssertValid();
}

void CMenusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMenusDoc* CMenusView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMenusDoc)));
	return (CMenusDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenusView message handlers

int CMenusView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Adding a tooltip attached to the view window handle:
	m_tool.windows.Add(m_hWnd, _T("<txt res='html:idr_view'>"));

	return 0;
}
