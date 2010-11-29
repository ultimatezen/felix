/*!
	@file aboutdlg.h
	@brief interface of the CAboutDialog class.
 */

#pragma once

#include "HtmlView.h"			// CHtmlView
#include "atlcrack.h"			// message crackers
#include "MessageMapDefines.h"	// more crackers
#include <atlbase.h>			// must be included before atlapp.h
#include <atlapp.h>				// must be included before altmisc.h
#include "wingdi.h"
#include "atlmisc.h"			// CSize
#include "Exceptions.h"			// CException
#include "WindowExceptionHandler.h"
#include "resource.h"

/**
	@class CAboutDialog
	@brief Generic about dialog with HTML content.
 */
class CAboutDialog : 
		public CDialogImpl<CAboutDialog, TWindow > 
		, public CDialogResize< CAboutDialog  >
		, public CWindowExceptionHandler< CAboutDialog  >
{

	// view
	html::CHtmlView	m_view ;

public:
	static const int IDD = IDD_ABOUTBOX ;
	CString m_aboutfile_name ;

	DECLARE_SENSING_VAR ;

	CAboutDialog(CString aboutfile_name=_T("")) : m_aboutfile_name(aboutfile_name)
	{
	}
	// Function name	: CAboutDialog::OnSize
	// Description	    : 
	// Return type		: LRESULT 
	// Argument         :  UINT type
	// Argument         : CSize size
	LRESULT OnSize( UINT type, CSize size )
	{
		SENSE( "OnSize" ) ;
		
		SetMsgHandled( FALSE ) ;
		
		if(m_bGripper)
		{
			TWindow wndGripper = GetDlgItem(ATL_IDW_STATUS_BAR);
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
		TWindow msg_box = GetDlgItem( IDC_ABOUT_MSG_BOX ) ;
		// get its position
		RECT rc ;
		msg_box.GetWindowRect( &rc ) ;
		ScreenToClient( &rc ) ;
		msg_box.ShowWindow( SW_HIDE ) ;
		
		// move the html view into place
		m_view.SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);
		
		
		return 0L ;
	}
	
	
	// Function name	: CAboutDialog::OnInitDialog
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnInitDialog( )
	{
		SENSE( "OnInitDialog" ) ;
		
		// Init the CDialogResize code
#ifndef UNIT_TEST
		DlgResize_Init( true, true ) ;
#endif		
		// set icon
		SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
		
		//	SetTextBackGround(0xFFFFFF);   
		//	SetTextColor(RGB(0x00, 0x00, 0x00));  
		
		// set up static where html view will go
		TWindow msg_box = GetDlgItem( IDC_ABOUT_MSG_BOX ) ;
		// get its position
		RECT rc ;
		msg_box.GetWindowRect( &rc ) ;
		ScreenToClient( &rc ) ;
		
		// create the html view and move it into place
		m_view.create( m_hWnd ) ;
		ATLASSERT( m_view.IsWindow() ) ;
		m_view.MoveWindow( &rc, TRUE ) ;
		
		// load the page
		if (! m_aboutfile_name.IsEmpty())
		{
			m_view.navigate(_bstr_t(m_aboutfile_name)) ;
		}
		else
		{
			m_view.load_from_resource( _T("ABOUTPAGE.HTML") ) ;
		}
		
		
		return TRUE ;
		
	}
	
	
	// Function name	: CAboutDialog::OnCloseCmd
	// Description	    : 
	// Return type		: LRESULT 
	// Argument         : WORD wID
	LRESULT OnCloseCmd( WORD wID )
	{
		SENSE( "OnCloseCmd" ) ;
		if ( wID == IDCANCEL ) 
		{
#ifndef UNIT_TEST
	EndDialog(wID);
#endif
		}
		else
		{
#ifndef UNIT_TEST
	EndDialog(IDOK);
#endif
		}
		return 0;
	}

    BEGIN_DLGRESIZE_MAP(CAboutDialog< int id_type >)
        // buttons
        DLGRESIZE_CONTROL(IDCLOSE, DLSZ_MOVE_X|DLSZ_MOVE_Y)

        // WebBrowser control
        DLGRESIZE_CONTROL(IDC_ABOUT_MSG_BOX, DLSZ_SIZE_X|DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP_EX( CAboutDialog< int id_type > )
		MSG_HANDLER_0 ( WM_INITDIALOG,  OnInitDialog )
 		MSG_WM_SIZE   ( OnSize )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID( IDOK,          OnCloseCmd   )
			CMD_HANDLER_EX_ID( IDCANCEL,      OnCloseCmd   )
			CMD_HANDLER_EX_ID( IDCLOSE,       OnCloseCmd   )
		// allow copy and other operations
			PASS_CMDS_TO_VIEW_EX( m_view ) 
		END_CMD_HANDLER_EX

      CHAIN_MSG_MAP( CDialogResize<CAboutDialog> )

	END_MSG_MAP( )

};

