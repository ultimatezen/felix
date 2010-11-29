// Parser Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Parser Test.h"
#include "ParserTestSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParserTestApp

BEGIN_MESSAGE_MAP(CParserTestApp, CWinApp)
	//{{AFX_MSG_MAP(CParserTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParserTestApp construction

CParserTestApp::CParserTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CParserTestApp object

CParserTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CParserTestApp initialization

BOOL CParserTestApp::InitInstance()
{
// Initializing STA here (Single Thread Apartment) to be able to use
// COM. Calling ::CoInitialize(NULL); would do the same job.
	AfxOleInit();

// Initializing ATL:
	_Module.Init(NULL, GetModuleHandle(NULL));

// This is to support Windows XP style of the user interface:
	InitCommonControls();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1300 // If it is prior to VC++.NET
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	CParserTestSheet dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CParserTestApp::ExitInstance()
{
// Closing ATL session here. Without doing it the program still will be working
// correctly, but there will be some memory loss left after initialization
// of ATL:
	_Module.Term();

	return CWinApp::ExitInstance();
}
