// DynamicControls.cpp : Defines the class behaviors for the application.
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
// CDynamicControlsApp

BEGIN_MESSAGE_MAP(CDynamicControlsApp, CWinApp)
	//{{AFX_MSG_MAP(CDynamicControlsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicControlsApp construction

CDynamicControlsApp::CDynamicControlsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDynamicControlsApp object

CDynamicControlsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDynamicControlsApp initialization

BOOL CDynamicControlsApp::InitInstance()
{
	AfxOleInit();
	_Module.Init(NULL, GetModuleHandle(NULL));
	AfxEnableControlContainer();

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

	m_hCursor = LoadCursor(IDC_HANDCURSOR);

	CDynamicControlsDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	DestroyCursor(m_hCursor); // Releasing the Hand cursor
	_Module.Term();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

HCURSOR CDynamicControlsApp::GetCursor()
{
	return m_hCursor;
}
