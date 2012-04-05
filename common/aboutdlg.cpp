// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"

#include "aboutdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif



LRESULT CAboutDialog::OnSize( UINT type, CSize size )
{
	SENSE( "OnSize" ) ;

	SetMsgHandled( FALSE ) ;

	if(m_bGripper)
	{
		CWindow wndGripper = GetDlgItem(ATL_IDW_STATUS_BAR);
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
	CWindow msg_box = GetDlgItem( IDC_ABOUT_MSG_BOX ) ;
	// get its position
	RECT rc ;
	msg_box.GetWindowRect( &rc ) ;
	ScreenToClient( &rc ) ;
	msg_box.ShowWindow( SW_HIDE ) ;

	// move the html view into place
	m_view.SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);


	return 0L ;
}

LRESULT CAboutDialog::OnInitDialog()
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
	CWindow msg_box = GetDlgItem( IDC_ABOUT_MSG_BOX ) ;
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

LRESULT CAboutDialog::OnCloseCmd( WORD wID )
{
	SENSE( "OnCloseCmd" ) ;
	END_DLG ;
}