/*!
	@file CrashDlg.h
	@brief Headers for crash window
	@date 2004/12/16
	Time: 12:29:54
	@author Ryan Ginstrom
 */

#pragma once

#include "HtmlView.h"			// CHtmlView
#include "MessageMapDefines.h"	// more crackers
#include "Exceptions.h"			// CException

#include "atlstr.h"
#include "atlmisc.h"			// CSize

#include "DebugUtilities.h"

#include "Path.h"

#include "atlwin.h"
#include "atlctrls.h"
#include "atlframe.h"	// CDialogResize
#include "WindowExceptionHandler.h"

#define VIEW_REPORT_ID 165

#define XCRASHREPORT_SEND_TO_NAME		"Felix Support"
#define XCRASHREPORT_SEND_TO_ADDRESS	"support@felix-cat.com"

BOOL SendEmail(HWND    hWnd,			// parent window, must not be NULL
					LPCSTR lpszTo,			// must NOT be NULL or empty
					LPCSTR lpszToName,		// may be NULL
					LPCSTR lpszSubject,		// may be NULL
					LPCSTR lpszMessage,		// may be NULL
					LPCSTR lpszAttachment) ;	// may be NULL


/**
	@class CCrashDlg : 
	@brief Shows error information for structured Windows exception handling.
 */
template< int id_type, int box_id, int crash_page_id >
class CCrashDlg : 
	public CDialogImpl<CCrashDlg<id_type, box_id, crash_page_id > >, 
	public CDialogResize< CCrashDlg<id_type, box_id, crash_page_id >  >
{
	CString			m_file_name ;
	string			m_message ;

	// view
	html::CHtmlView	m_view ;

public:
	enum { IDD = id_type };


	LRESULT OnSize( UINT type, CSize size )
	{
		BANNER( "CCrashDlg::OnSize" ) ;

		SetMsgHandled( FALSE ) ;

		if(m_bGripper)
		{
			ATL::CWindow wndGripper = GetDlgItem(ATL_IDW_STATUS_BAR);
			if(type == SIZE_MAXIMIZED)
				wndGripper.ShowWindow(SW_HIDE);
			else if(type == SIZE_RESTORED)
				wndGripper.ShowWindow(SW_SHOW);
		}
		if(type != SIZE_MINIMIZED)
		{
			DlgResize_UpdateLayout(size.cx , size.cy );
		}

		if ( type == SIZE_MINIMIZED )
			return 0L ;

		// set up static where html view will go
		CWindow msg_box = GetDlgItem( box_id ) ;
		// get its position
		RECT rc ;
		msg_box.GetWindowRect( &rc ) ;
		ScreenToClient( &rc ) ;
		msg_box.ShowWindow( SW_HIDE ) ;

		// move the html view into place
		m_view.SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);


		return 0L ;
	}

	LRESULT OnInitDialog()
	{
		BANNER( "CCrashDlg::OnInitDialog" ) ;

		// Init the CDialogResize code
		DlgResize_Init( true, true ) ;

		// set icon
		SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

		//	SetTextBackGround(0xFFFFFF);   
		//	SetTextColor(RGB(0x00, 0x00, 0x00));  

		// set up static where html view will go
		CWindow msg_box = GetDlgItem( box_id ) ;
		// get its position
		RECT rc ;
		msg_box.GetWindowRect( &rc ) ;
		ScreenToClient( &rc ) ;

		// create the html view and move it into place
		m_view.create( m_hWnd ) ;
		ATLASSERT( m_view.IsWindow() ) ;
		m_view.MoveWindow( &rc, TRUE ) ;

		// load the page
		CString crash_page_res ;
		crash_page_res.LoadString( crash_page_id ) ;
		m_view.load_from_resource( crash_page_res ) ;

		return TRUE ;
	}

	LRESULT OnViewReport()
	{
		BANNER( "CCrashDlg::OnViewReport" ) ;

		MessageBox(m_file_name, _T("Error Report")) ;
		file::directory report_directory( m_file_name, true ) ;

		CString report_path = report_directory.get_path()  ;
		int result = (int)::ShellExecute(
			m_hWnd,					//	HWND hwnd, 
			_T("open"),				// LPCTSTR lpOperation,
			m_file_name,			// LPCTSTR lpFile, 
			NULL,					// LPCTSTR lpParameters, 
			report_path,			// LPCTSTR lpDirectory,
			SW_SHOW					// INT nShowCmd
			);	

		if ( result <= 32 )
		{
			throw CWinException( _T("Failed to show report"), result ) ;
		}

		return 0 ;
	}

	LRESULT OnSend()
	{
		string subject = "Felix Error report for " + m_message ;
		file::view log_view ;

		string body = "User Comment: \r\n\r\n\r\n============ ERROR LOG =====================\r\n"	;
		body <<  (const char *)log_view.create_view( m_file_name ) ;
		SendEmail(m_hWnd,
			XCRASHREPORT_SEND_TO_ADDRESS, 
			XCRASHREPORT_SEND_TO_ADDRESS, 
			subject.c_str(), 
			body.c_str(), 
			NULL);

		EndDialog(IDOK);
		return 0;
	}

	LRESULT OnDontSend()
	{
		EndDialog(IDCANCEL);
		return 0;
	}

	BEGIN_DLGRESIZE_MAP(CCrashDlg< int id_type >)
			// buttons
			DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X|DLSZ_MOVE_Y)

			// WebBrowser control
			DLGRESIZE_CONTROL(IDC_ABOUT_MSG_BOX, DLSZ_SIZE_X|DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP_EX( CCrashDlg< int id_type > )
		try
		{
			// ----
			MSG_HANDLER_0 ( WM_INITDIALOG,  OnInitDialog )
			MSG_WM_SIZE   ( OnSize )

			// ----
			BEGIN_USER_HANDLER_EX
				USER_HANDLER_EX_0( VIEW_REPORT_ID, OnViewReport )
			END_USER_HANDLER_EX

			// ----
			BEGIN_CMD_HANDLER_EX
				CMD_HANDLER_EX_0( IDOK,          OnSend   )
				CMD_HANDLER_EX_0( IDCANCEL,      OnDontSend   )
				// allow copy and other operations
				PASS_CMDS_TO_VIEW_EX( m_view ) 
			END_CMD_HANDLER_EX

			// ----
			CHAIN_MSG_MAP( CDialogResize<CCrashDlg> )
		}
		catch( ... )
		{
			BANNER( "Exception caught" ) ;
			MessageBeep( MB_ICONSTOP ) ;
			return FALSE ;
		}
		END_MSG_MAP()

	CCrashDlg( )
		: m_message( "Structured Windows Exception" )
	{
		file::CPath module_path ;
		HINSTANCE module_instance = _Module.GetModuleInstance() ;
		module_path.GetModulePath( module_instance ) ;
		module_path.Append( XCRASHREPORT_ERROR_LOG_FILE ) ;
		m_file_name = module_path.Path() ;
	}

};

