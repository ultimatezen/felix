#include "stdafx.h"
#include "InputKeyDlg.h"
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
	string key = get_key_text(text);

	END_DLG ;
}

string CInputKeyDlg::get_key_text( CString text )
{
	string key = CT2A(text) ;

	boost::replace_all( key, "\t", "" ) ;
	boost::replace_all( key, "\n", "" ) ;
	boost::replace_all( key, "\r", "" ) ;
	boost::replace_all( key, " ", "" ) ;
	boost::replace_all( key, ">", "" ) ;
	return key ;
}

LRESULT CInputKeyDlg::OnCloseCommand( WORD wID )
{
	SENSE("OnCloseCommand") ;
	END_DLG ;
}
