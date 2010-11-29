// MenusDoc.cpp : implementation of the CMenusDoc class
//

#include "stdafx.h"
#include "Menus.h"

#include "MenusDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenusDoc

IMPLEMENT_DYNCREATE(CMenusDoc, CDocument)

BEGIN_MESSAGE_MAP(CMenusDoc, CDocument)
	//{{AFX_MSG_MAP(CMenusDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenusDoc construction/destruction

CMenusDoc::CMenusDoc()
{
	// TODO: add one-time construction code here

}

CMenusDoc::~CMenusDoc()
{
}

BOOL CMenusDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMenusDoc serialization

void CMenusDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMenusDoc diagnostics

#ifdef _DEBUG
void CMenusDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMenusDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenusDoc commands
