#include "stdafx.h"
#include "InputKeyDlg.h"

LRESULT CInputKeyDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;
	CenterWindow( ::GetDesktopWindow() ) ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	m_edit.Attach( GetDlgItem( IDC_KEY_EDIT ) ) ;

	return TRUE;
}

LRESULT CInputKeyDlg::OnOK( WORD wID )
{
	SENSE("OnOK") ;

	str::tbuffer tbuff ;
#ifndef UNIT_TEST
	// get the text in the window
	int len = m_edit.GetWindowTextLength() + 1 ;
	m_edit.GetWindowText( tbuff.buffer( len ), len ) ;
#else
	_tcscpy(tbuff.buffer(20), _T("debug")) ;
#endif
	// load the name and key strings
	std::string name, key ;

	name = "REGISTERED" ;
	key = CT2A( tbuff.buffer() ) ;

	// strip whitespace from key
	str::replace_all( key, "\t", "" ) ;
	str::replace_all( key, "\n", "" ) ;
	str::replace_all( key, "\r", "" ) ;
	str::replace_all( key, " ", "" ) ;

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
		return 0 ;
	}

	if ( ! armadillo::update_environment() )
	{
		MessageBox( CA2T( armadillo::get_arm_error_string().c_str() ), _T("FAILED TO UPDATE ARMADILLO ENVIRONMENT") ) ;
		SetMsgHandled( TRUE ) ;
		return 0 ;
	}

	if ( ! armadillo::get_environment_var( "EXPIRED" ).empty() )
	{
		::MessageBeep( MB_ICONSTOP ) ;
		MessageBox( resource_string( IDS_KEY_INCORRECT ), resource_string( IDS_KEY_INCORRECT_TITLE ) ) ;
		m_edit.SetFocus() ;
		m_edit.SetSelAll( ) ;
		SetMsgHandled( TRUE ) ;
		return 0 ;
	}

	EndDialog(wID);

	return 0;
}

LRESULT CInputKeyDlg::OnCloseCommand( WORD wID )
{
	SENSE("OnCloseCommand") ;
	EndDialog(wID);
	return 0;
}