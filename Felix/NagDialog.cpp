// NagDialog.cpp: implementation of the CNagDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "NagDialog.h"
#include "InputKeyDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Function name	: CNagDialog::OnInitDialog
// Description	    : 
// Return type		: LRESULT 
LRESULT CNagDialog::OnInitDialog( )
{
	SENSE( "OnInitDialog" ) ;

	CenterWindow(::GetDesktopWindow());

    // Init the CDialogResize code
#ifndef UNIT_TEST
    DlgResize_Init( true, true ) ;
#endif
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	// set up static where html view will go
	CStatic msg_box ;
	msg_box.Attach( GetDlgItem( IDC_NAG_MSG_BOX ) ) ;

#ifdef UNIT_TEST
	return TRUE ;
#else

	// get its position
	RECT rc ;
	msg_box.GetWindowRect( &rc ) ;
	ScreenToClient( &rc ) ;

	// create the html view and move it into place
	ATLASSERT( IsWindow() ) ;

	m_view.create( m_hWnd ) ;
	ATLASSERT( m_view.IsWindow() ) ;
	m_view.MoveWindow( &rc, TRUE ) ;

	// load the page
	m_view.load_from_resource( m_nagPage ) ;

	return TRUE ;
#endif

}


// Function name	: CNagDialog::OnClose
// Description	    : 
// Return type		: LRESULT 
// Argument         : WORD wID
LRESULT CNagDialog::OnClose( WORD wID )
{
	SENSE( "OnClose" ) ;
	END_DLG ;
}


// Function name	: CNagDialog::OnSize
// Description	    : 
// Return type		: LRESULT 
// Argument         :  UINT type
// Argument         : CSize size
LRESULT CNagDialog::OnSize( UINT type, CSize size )
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
	CStatic msg_box ;
	msg_box.Attach( GetDlgItem( IDC_NAG_MSG_BOX ) ) ;
	// get its position
	RECT rc ;
	msg_box.GetWindowRect( &rc ) ;
	ScreenToClient( &rc ) ;
	msg_box.ShowWindow( SW_HIDE ) ;

	// move the html view into place
	m_view.SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);

	return 0L ;
}


// Function name	: CNagDialog::CNagDialog
// Description	    : 
// Return type		: 
CNagDialog::CNagDialog( CString nagPage) :
	m_nagPage( nagPage )
{
}


// Function name	: CNagDialog::~CNagDialog
// Description	    : 
// Return type		: 
CNagDialog::~CNagDialog()
{
}
