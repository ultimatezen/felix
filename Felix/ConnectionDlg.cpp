#include "stdafx.h"
#include "ConnectionDlg.h"
#include "DemoException.h"
#include "InputKeyDlg.h"
#include "memory_remote.h"

using namespace except ;

CConnectionDlg::CConnectionDlg(app_props::props_ptr props) : 
	m_props(props)
{

}
LRESULT CConnectionDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	m_edit.Attach( GetDlgItem( IDC_CONN_EDIT ) ) ;
	m_err_box.Attach( GetDlgItem( IDC_MSG_BOX ) ) ;

	m_username.Attach( GetDlgItem( IDC_USERNAME ) ) ;
	m_password.Attach( GetDlgItem( IDC_PASSWORD ) ) ;

	if (m_props->m_gen_props.must_log_in())
	{
		CheckDlgButton(IDC_CONN_LOGIN, TRUE) ;
	}
	else
	{
		m_username.EnableWindow(FALSE); 
		m_password.EnableWindow(FALSE); 
	}


#ifdef UNIT_TEST
	return TRUE ;
#else

	// set the focus to the edit box
	m_edit.SetSel(0, 0) ;
	m_edit.SetFocus() ;
	return TRUE;
#endif
}

LRESULT CConnectionDlg::OnOK( WORD wID )
{
	SENSE("OnOK") ;

#ifndef UNIT_TEST

	using namespace mem_engine ;

	// get the text in the window
	CString conn_str ;
	m_edit.GetWindowText( conn_str ) ;

	memory_remote *mem = new memory_remote(m_props) ;
	memory_pointer pmem(mem) ;

	CString username ;
	CString password ;
	
	// If we also need to log in...
	if (IsDlgButtonChecked(IDC_CONN_LOGIN))
	{
		m_username.GetWindowText( username ) ;
		m_password.GetWindowText( password ) ;
	}

	try
	{
		ATLTRACE("Connecting to TM\n") ;
		TRACE(conn_str) ;
		TRACE(username) ;
		TRACE(password) ;
		// will also log in if username and password are not empty.
		mem->connect(conn_str, username, password) ;

	}
	catch( CDemoException &e)
	{
		if (e.notify_user(_T("This is the Demo version of Felix")) == IDOK)
		{
			// prompt him!
			CInputKeyDlg input_key_dlg ;

			if ( IDOK == input_key_dlg.DoModal( ) )
			{
				MessageBox( resource_string( IDS_REGISTERED_USER ), resource_string( IDS_REGISTERED_USER_TITLE ) ) ;
			}
			else
			{
				MessageBeep(MB_ICONSTOP) ;
				this->FlashWindow(FALSE) ;
				this->SetMsgHandled(FALSE) ;
				// set the focus to the edit box
				m_edit.SetSel(0, -1) ;
				m_edit.SetFocus() ;
				return 1L ;
			}
		}
		else
		{
			END_DLG ;
		}

	}
	catch (CException& e)
	{
		if (! username.IsEmpty())
		{
			e.add_to_message(R2T(IDS_CHECK_USER_AND_PASS)) ;
		}
		else
		{
			e.add_to_message(R2T(IDS_CHECK_LOGIN)) ;
		}
		e.notify_user(_T("Failed to connect to Memory Serves")) ;
		this->FlashWindow(FALSE) ;
		this->SetMsgHandled(FALSE) ;
		// set the focus to the edit box
		m_edit.SetSel(0, -1) ;
		m_edit.SetFocus() ;
		return 1L ;
	}


	m_memory = pmem ;
	MessageBeep(MB_ICONINFORMATION) ;
#endif

	END_DLG ;
}

LRESULT CConnectionDlg::OnCloseCommand( WORD wID )
{
	SENSE("OnClose") ;
	END_DLG ;
}

LRESULT CConnectionDlg::OnLoginChecked( WORD, WORD, HWND, BOOL& )
{
	const BOOL enable_login = IsDlgButtonChecked(IDC_CONN_LOGIN) ;
	m_username.EnableWindow(enable_login); 
	m_password.EnableWindow(enable_login); 

	m_props->m_gen_props.set_must_log_in(enable_login) ;
	m_props->save_prefs();

	ATLTRACE("OnLoginChecked\n") ;
	return 0L ;
}
