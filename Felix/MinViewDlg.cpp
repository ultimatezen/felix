/*!
 * implementation of CMinViewDlg class.
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */


#include "StdAfx.h"
#include "resource.h"
#include "MinViewDlg.h"
#include "resource_string.h"
#include "xpmenu/Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = TEXT(__FILE__) ;
#endif


LRESULT CMinViewDlg::OnInitDialog( )
{
	SENSE("OnInitDialog");
	
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	SetWindowText( resource_string( IDD_MIN_VIEW_DLG ) ) ;

	// Attach member controls to their resources.
    m_idcButton1 = GetDlgItem(IDC_MIN_BUTTON);
    m_idcEditBox = GetDlgItem(IDC_EDIT_BOX);

	// Initialize resizing abilities.
#ifndef UNIT_TEST
	DlgResize_Init();
#endif
	// get its position

	const CWindowRect window_rc(*this) ;
	
	const int padding = 4 ;
#ifdef UNIT_TEST
	return TRUE ;
#endif

	CWindowRect rc(m_idcEditBox) ;
	ScreenToClient( &rc ) ;
	
	rc.top = 0 ;
	rc.bottom = window_rc.Height() ;
	rc.right -= padding ;
	
	// create the html view and move it into place
	m_view.create( m_hWnd ) ;
	ATLASSERT( m_view.IsWindow() ) ;
	m_view.MoveWindow( &rc, TRUE ) ;

	CWindowRect button_rect(m_idcButton1) ;
	ScreenToClient( &button_rect ) ;
	button_rect.left -= padding ;
	button_rect.top -= padding ;
	button_rect.bottom += padding ;
	m_idcButton1.SetWindowPos(NULL, &button_rect, SWP_NOZORDER | SWP_NOACTIVATE);

	m_tooltips.Create( *this, NULL, NULL, TTS_NOPREFIX | TTS_BALLOON );
	m_tooltips.Activate( TRUE ) ;
	
	CToolInfo ToolInfo( TTF_IDISHWND | TTF_SUBCLASS, *this , 0, 0, MAKEINTRESOURCE(IDD) );
	m_tooltips.AddTool( &ToolInfo );
	CToolInfo button_info( TTF_IDISHWND | TTF_SUBCLASS, m_idcButton1, 0, 0, MAKEINTRESOURCE( IDC_MIN_BUTTON ) ) ;
	m_tooltips.AddTool( &button_info ) ;

	return 0;
} // OnInitDialog

LRESULT CMinViewDlg::OnRestoreFullView( )
{
	SendMessage( m_parent, UWM_USER_MESSAGE, IDC_MIN_VIEW_END, 0 ) ;

	ShowWindow( SW_HIDE ) ;

	return 0L ;

} // OnButton1

/////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMinViewDlg::OnSize( UINT type, CSize size )
{
	BANNER( "CAboutDlg::OnSize" ) ;
	
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
	
	// get its position
	RECT window_rc ;
	GetWindowRect( &window_rc ) ;

	int height = window_rc.bottom - window_rc.top ;

	RECT rc ;
	m_idcEditBox.GetWindowRect( &rc ) ;
	ScreenToClient( &rc ) ;

	rc.top = 0 ;
	rc.bottom = height ;
	rc.right -= 4 ;
	
	// move the html view into place
	m_view.SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_NOACTIVATE);

	return 0L ;
}

LRESULT CMinViewDlg::OnLButtonDown(UINT /*id*/, CPoint pt)
{
	CRect rc ;
	m_view.GetWindowRect( &rc ) ;
	if ( rc.PtInRect( pt ) )
	{
		SetMsgHandled( FALSE ) ;
		return 1L ;
	}

	return SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, NULL) ;
}

LRESULT CMinViewDlg::OnMouse(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/ )
{
	SetMsgHandled( FALSE ) ;
	if(m_tooltips.IsWindow())
		m_tooltips.RelayEvent((LPMSG)this->GetCurrentMessage());
	return 0;
}

BOOL CMinViewDlg::PreTranslateMessage( LPMSG pMsg )
{
	ENSURE_FOCUS

		if ( IsDialogMessage( pMsg ) )
			return TRUE ;

	return m_view.PreTranslateMessage( pMsg ) ;
}

CMinViewDlg::CMinViewDlg() : m_parent( NULL )
{

}