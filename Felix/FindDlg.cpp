/*!
@brief Implementation file for the record search dialog
@author Ryan Ginstrom
*/

#include "stdafx.h"
#include "FindDlg.h"
#include "reg_msg_filter.h"
#include "record_local.h"

using namespace mem_engine ;

/** CFindDlg
@brief CTOR
*/
CFindDlg::CFindDlg() 
{
}

/** PreTranslateMessage
@brief Give the dialog a crack at the message.
@param *pMsg pointer to the message
@retval BOOL success
*/
BOOL CFindDlg::PreTranslateMessage( MSG *pMsg )
{
	ENSURE_ACTIVE
	// translating accelerators here will just rob messages from our edit boxes.
	// let those guys handle their own accelerators

	return IsDialogMessage( pMsg ) ;

}


/** Get the search parameters.
*/
mem_engine::search_query_params &CFindDlg::get_search_params() 
{ 
	return m_params ; 
}

/** Destroys the dialog window.
*/
LRESULT CFindDlg::OnDestroy( )
{
	logging::log_verbose("Destroying the find dialog window") ;
	// Set to FALSE so that default processing will happen.
	SetMsgHandled( FALSE ) ;

	m_source_edit.Detach() ;
	m_trans_edit.Detach() ;
	m_context_edit.Detach() ;

	m_reliability_edit.Detach() ;

#ifndef UNIT_TEST
	_Module.GetMessageLoop()->RemoveMessageFilter(this);
#endif

	return 0L ;
}

/** Init the dialog.

We set up the data in the find dialog, because we may
switch the UI languages, and destroy/create the 
find dialog to get the new UI. This will destroy the
info in the edit boxes and such, but not the params.
*/
LRESULT CFindDlg::OnInitDialog( )
{
	logging::log_verbose("Initializing the find dialog") ;
#ifndef UNIT_TEST
	// set icon
	SetIcon(LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	// set our message filter...
	register_message_filter(this) ;
#endif

	// ===============
	// checkbox setup
	// ===============

#ifndef UNIT_TEST
	CheckDlgButton( IDC_ONLY_VALIDATED_CHECK, ( m_params.m_only_validated ? BST_CHECKED : BST_UNCHECKED ) ) ;
	CheckDlgButton( IDC_REGEX_CHECK, ( m_params.m_use_regex ? BST_CHECKED : BST_UNCHECKED ) ) ;
	CheckDlgButton( IDC_IGNORE_CASE_CHECK, ( m_params.m_ignore_case ? BST_CHECKED : BST_UNCHECKED ) ) ;
	CheckDlgButton( IDC_FIND_IGNORE_WIDTH_CHECK, ( m_params.m_ignore_width ? BST_CHECKED : BST_UNCHECKED ) ) ;
	CheckDlgButton( IDC_FIND_IGNORE_HIR_KAT_CHECK, ( m_params.m_ignore_hira_kata? BST_CHECKED : BST_UNCHECKED ) ) ;
#endif

	// ===============
	// source setup
	// ===============

	// create the source window 
	
#ifndef UNIT_TEST
	m_source_edit.Attach( GetDlgItem( IDC_SOURCE_BOX ) ) ;
#endif

	// ===============
	// trans setup
	// ===============

	// create the trans window
#ifndef UNIT_TEST
	m_trans_edit.Attach( GetDlgItem( IDC_TRANS_BOX ) ) ;
#endif

	// ===============
	// context setup
	// ===============

	// create the context window 
#ifndef UNIT_TEST
	m_context_edit.Attach( GetDlgItem( IDC_CONTEXT_BOX ) )  ;
#endif

#ifdef UNIT_TEST
	return 0L ;
#else
	// ===========================
	// set the text of the edit boxes
	// ===========================
	m_source_edit.SetText( m_params.get_source_rich() ) ;
	if ( m_params.get_source_rich().empty() ) 
	{
		m_source_edit.SetSel(0, 0) ;
	}
	else
	{
		m_source_edit.SetSel( 0, -1 ) ;
	}
	m_trans_edit.SetText( m_params.m_rich_trans ) ;
	m_context_edit.SetText( m_params.m_rich_context ) ;

	// ===========================
	// set up the reliability box
	// ===========================
	m_reliability_edit.Attach( GetDlgItem( IDC_RELIABILITY_EDIT ) ) ;

	CUpDownCtrl reliability_spin = GetDlgItem( IDC_RELIABILITY_SPIN ) ;
	reliability_spin.SetRange( 0, 9 ) ;
	ATLASSERT(m_params.m_min_reliability <= 9) ;
	reliability_spin.SetPos( (int) m_params.m_min_reliability ) ;

	m_source_edit.SetFocus() ;

	return 0 ;
#endif
}

/** We hide ourselves when we get a close command.
*/
LRESULT CFindDlg::OnClose( )
{
	SENSE("OnClose") ;

	ShowWindow( SW_HIDE ) ;
	return 0L ;
}

/** Handle user's search request.
*/
LRESULT CFindDlg::OnSearch( )
{
	SENSE("OnSearch") ;

	// source
	m_params.set_source(m_source_edit.GetText()) ;

	// trans
	m_params.set_trans(m_trans_edit.GetText()) ;

	// context
	m_params.set_context(m_context_edit.GetText()) ;

	// various search flags
	// !! changes BOOL to bool
	m_params.m_ignore_case		= !! IsDlgButtonChecked( IDC_IGNORE_CASE_CHECK ) ;
	m_params.m_use_regex		= !! IsDlgButtonChecked( IDC_REGEX_CHECK ) ;
	m_params.m_only_validated	= !! IsDlgButtonChecked( IDC_ONLY_VALIDATED_CHECK ) ;

	m_params.m_ignore_width = !! IsDlgButtonChecked( IDC_FIND_IGNORE_WIDTH_CHECK ) ;
	m_params.m_ignore_hira_kata = !! IsDlgButtonChecked( IDC_FIND_IGNORE_HIR_KAT_CHECK ) ;

	// minimum reliability
	CString text ;
#ifdef UNIT_TEST
	text = _T("5") ;
#else
	m_reliability_edit.GetWindowText(text) ;
#endif	
	tstring reliability = static_cast<LPCTSTR>(text) ;
	m_params.m_min_reliability = string2long(reliability) ;

	// send message to parent to come and get the record for search
	::SendMessage( GetParent(), UWM_USER_MESSAGE, ID_USER_SEARCH, 0 ) ;

	return 0L ;
}

/** Redo command from user.
*/
LRESULT CFindDlg::OnRedo( )
{
	SENSE("OnRedo") ;
	::SendMessage(::GetFocus(), EM_REDO, 0, 0L);
	return 0L ;
}

bool CFindDlg::check_focus( LRESULT &lResult, WPARAM wparam, LPARAM lparam )
{
	SENSE("check_focus") ;
	HWND focus_hwnd = ::GetFocus() ;

	if ( has_focus(focus_hwnd, m_source_edit) )
	{
		lResult = m_source_edit.SendMessage( WM_COMMAND, wparam, lparam ) ;
		return true ;
	}
	else if ( has_focus(focus_hwnd, m_trans_edit)	)
	{
		lResult = m_trans_edit.SendMessage( WM_COMMAND, wparam, lparam ) ;
		return true ;
	}
	else if ( has_focus(focus_hwnd, m_context_edit)	)
	{
		lResult = m_context_edit.SendMessage( WM_COMMAND, wparam, lparam ) ;
		return true ;
	}
	else if ( has_focus(focus_hwnd, m_reliability_edit)	)
	{
		lResult = m_reliability_edit.SendMessage( WM_COMMAND, wparam, lparam ) ;
		return true ;
	}

	return false ;
}

bool CFindDlg::has_focus( HWND focus_hwnd, CWindow window )
{
	return window.IsWindow() && ( focus_hwnd == window.m_hWnd || window.IsChild( focus_hwnd ) );
}