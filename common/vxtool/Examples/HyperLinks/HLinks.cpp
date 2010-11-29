// HLinks.cpp : Defines the class behaviors for the application.
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
// CHLinksApp

BEGIN_MESSAGE_MAP(CHLinksApp, CWinApp)
	//{{AFX_MSG_MAP(CHLinksApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLinksApp construction

CHLinksApp::CHLinksApp()
{
	m_bUseHandCursor = true;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHLinksApp object

CHLinksApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHLinksApp initialization

BOOL CHLinksApp::InitInstance()
{
	AfxOleInit(); // This is necessary to make COM work in the application

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

	if(!VXPLib::IsLibraryAvailable())	// If the library is not available on this PC:
	{
		CString path;
		GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH), MAX_PATH);	// Retrieving full path
																				// to the application

		// Retrieving the path-only part from the full path:
		int index = path.ReverseFind('\\');
		if(index != -1)
			path = path.Left(index);

		if(!VXPLib::RegisterLibrary(path + _T("\\VXPLib.dll"))) // If failed to register VXPLib.dll from
																// the same directory where the application resides.
		{
			// Try to register VXPLib.dll from the parent directory, if there is any:
			index = path.ReverseFind('\\');
			if(index != -1)
				path = path.Left(index);
			if(!VXPLib::RegisterLibrary(path + _T("\\VXPLib.dll"))) // If failed to register again
			{
				if(AfxMessageBox(IDS_VXPLIB_NOTFOUND, MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					///////////////////////////////////////////
					// If agreed to upload the library:

					// Opening the upload URL for VXPLib.dll:
					::ShellExecute(NULL, NULL, _T("http://www.tooltips.net/VXPLib_Binary.zip"), NULL, NULL, 0);

					return FALSE;
				}
			}
		}
	}

	CHLinksDlg dlg;
	m_pMainWnd = &dlg;

	m_hHandCursor = LoadCursor(IDC_HANDCURSOR);	// Loading the Hand cursor

	dlg.DoModal();

	DestroyCursor(m_hHandCursor);				// Releasing the Hand cursor

	return FALSE;
}

HCURSOR CHLinksApp::GetCursor()
{
	if(m_bUseHandCursor)
		return m_hHandCursor;
	return NULL;
}
