#include "stdafx.h"
#include "InputKeyDlg.h"
#include "ArmadilloStuff.h"
#include "logging.h"

// CInputKeyDlg

LRESULT CInputKeyDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;
#ifndef UNIT_TEST
	CenterWindow( ::GetDesktopWindow() ) ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	m_edit.Attach( GetDlgItem( IDC_KEY_EDIT ) ) ;
#endif

	return TRUE;
}

LRESULT CInputKeyDlg::OnOK( WORD wID )
{
	SENSE("OnOK") ;
	wID ;
	logging::log_debug("Registering Key") ;

	CString text ;
#ifndef UNIT_TEST
	// get the text in the window
	m_edit.GetWindowText(text) ;
#else
	text = _T("debug") ;
#endif
	// load the name and key strings
	const string name = "REGISTERED" ;
	string key = CT2A(text) ;

	// strip whitespace from key
	boost::replace_all( key, "\t", "" ) ;
	boost::replace_all( key, "\n", "" ) ;
	boost::replace_all( key, "\r", "" ) ;
	boost::replace_all( key, " ", "" ) ;

#ifndef UNIT_TEST
#ifdef _DEBUG
	if ( key != "debug" )
#else
	if ( ! armadillo::install_key( name, key ) )
#endif
	{
		::MessageBeep( MB_ICONSTOP ) ;
		MessageBox( resource_string( IDS_KEY_INCORRECT ), resource_string( IDS_KEY_INCORRECT_TITLE ) ) ;
		m_edit.SetFocus() ;
		m_edit.SetSelAll( ) ;
		SetMsgHandled( TRUE ) ;
		return 0L ;
	}

	if ( ! armadillo::update_environment() )
	{
		MessageBox( CA2T( armadillo::get_arm_error_string().c_str() ), _T("FAILED TO UPDATE ARMADILLO ENVIRONMENT") ) ;
		SetMsgHandled( TRUE ) ;
		return 0L ;
	}

	if ( ! armadillo::get_environment_var( "EXPIRED" ).empty() )
	{
		::MessageBeep( MB_ICONSTOP ) ;
		MessageBox( resource_string( IDS_KEY_INCORRECT ), resource_string( IDS_KEY_INCORRECT_TITLE ) ) ;
		m_edit.SetFocus() ;
		m_edit.SetSelAll( ) ;
		SetMsgHandled( TRUE ) ;
		return 0L ;
	}

	EndDialog(wID);
#endif

	return 0L;
}

LRESULT CInputKeyDlg::OnCloseCommand( WORD wID )
{
	SENSE("OnCloseCommand") ;
	wID ;
#ifndef UNIT_TEST
	EndDialog(wID);
#endif
	return 0L;
}
