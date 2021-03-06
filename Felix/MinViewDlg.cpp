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
#include "logging.h"
#include "resizeable_dialog.h"

// Serious DRY violation here...
static const wstring red_match			= L"#f80000" ;
static const wstring orange_match		= L"#ff8000" ;
static const wstring yellow_match		= L"#ffff80" ;
static const wstring green_match		= L"#80ff80" ;
static const wstring white_background	= L"#ffffff" ;

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
	
#ifdef UNIT_TEST
	return TRUE ;
#else
	const int padding = 4 ;

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
#endif

}

LRESULT CMinViewDlg::OnRestoreFullView( )
{
	SendMessage( m_parent, UWM_USER_MESSAGE, IDC_MIN_VIEW_END, 0 ) ;

	ShowWindow( SW_HIDE ) ;

	return 0L ;

} // OnButton1

/////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMinViewDlg::OnSize( UINT type, CSize size )
{
	SENSE( "CMinViewDlg::OnSize" ) ;
	
	if (! handle_common_resize_tasks(*this, type, size))
	{
		return 0L ;
	}
	
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

CMinViewDlg::CMinViewDlg() : m_parent( NULL ), 
	m_listener(NULL), 
	m_trans_matches(NULL)
{

}

html::CHtmlDocument CMinViewDlg::get_document()
{
	return m_view.get_document() ;
}

void CMinViewDlg::set_body_text( const wstring text )
{
	m_view.set_body_text(text) ;
}

void CMinViewDlg::set_parent( const HWND parent )
{
	m_parent = parent ;
}

void CMinViewDlg::set_match(mem_engine::felix_query *matches)
{
	html::CHtmlDocument doc = get_document() ;

	if ( matches->empty() )
	{
		set_body_text( wstring() ) ;
		doc.set_bg_color( red_match ) ;
	}
	else
	{
		mem_engine::search_match_ptr match = matches->current() ;

		mem_engine::record_pointer rec = match->get_record() ;

		const double score = match->get_score() ;

		if ( FLOAT_EQ( score, 1.0 ) )
		{
			doc.set_bg_color( green_match ) ;	
		}
		else if ( score >= 0.9 )
		{
			doc.set_bg_color( yellow_match ) ;	
		}
		else 
		{
			doc.set_bg_color( orange_match ) ;	
		}

		wstring content = rec->get_trans_rich() ;
		content << L"<hr />" ;
		content << R2W( IDS_SCORE ) << L": " << double2percent_wstring( score ) ;

		set_body_text( content ) ;	
	}
}

void CMinViewDlg::show_content()
{
	switch(m_listener->get_display_state())
	{
	case WindowListener::MATCH_DISPLAY_STATE:
		set_match(m_trans_matches) ;
		break; 
	case WindowListener::TRANS_REVIEW_STATE:
		set_match(m_trans_matches) ;
		break; 
	case WindowListener::NEW_RECORD_DISPLAY_STATE:
		mem_engine::record_pointer rec = m_listener->get_new_record() ;
		set_body_text( rec->get_trans_rich() ) ;
		html::CHtmlDocument doc = get_document() ;
		doc.set_bg_color( CColorRef(COLOR_WHITE).as_wstring() ) ;
		break; 
	}
}