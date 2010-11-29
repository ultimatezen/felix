// CustomTooltips.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CustomTooltips.h"
#include "CustomTooltipsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsApp

BEGIN_MESSAGE_MAP(CCustomTooltipsApp, CWinApp)
	//{{AFX_MSG_MAP(CCustomTooltipsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsApp construction

CCustomTooltipsApp::CCustomTooltipsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCustomTooltipsApp object

CCustomTooltipsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCustomTooltipsApp initialization

BOOL CCustomTooltipsApp::InitInstance()
{
	// Allow the application to use COM:
	AfxOleInit();
	////////////////////////////////////

	// Initializing ATL support in the application:
	_Module.Init(NULL, GetModuleHandle(NULL));
	//////////////////////////////////////////////

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1300				// If the environment is prior to VC++.NET
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	if(!VXPLib::IsLibraryAvailable())	// If the VXPLib is not available on this PC:
	{
		if(AfxMessageBox(IDS_VXPLIB_NOT_AVAILABLE, MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			///////////////////////////////////////////
			// If agreed to download the library:
			// Opening the download URL for VXPLib.dll:
			::ShellExecute(NULL, NULL, _T("http://www.tooltips.net/VXPLib_Binary.zip"), NULL, NULL, 0);

			// Uninitializing ATL support:
			_Module.Term();	
			/////////////////////////////

			return FALSE;
		}
	}

	CCustomTooltipsDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

int CCustomTooltipsApp::ExitInstance() 
{
	// Uninitializing ATL support:
	_Module.Term();	
	/////////////////////////////
	
	return CWinApp::ExitInstance();
}
