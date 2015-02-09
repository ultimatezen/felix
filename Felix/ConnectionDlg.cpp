#include "stdafx.h"
#include <wincred.h>
#include "CredVault.h"
#include <vector>
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
	SetIcon(LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME) ), FALSE) ;

	m_edit.Attach( GetDlgItem( IDC_CONN_EDIT ) ) ;

	m_username.Attach( GetDlgItem( IDC_USERNAME ) ) ;
	m_password.Attach( GetDlgItem( IDC_PASSWORD ) ) ;
	m_remember_cred_checkbox.Attach(GetDlgItem(IDC_REMEMBER_CREDENTIALS));

	if (m_props->m_gen_props.save_credentials())
	{
		CheckDlgButton(IDC_REMEMBER_CREDENTIALS, TRUE);
	}

	if (m_props->m_gen_props.must_log_in())
	{
		CheckDlgButton(IDC_CONN_LOGIN, TRUE) ;
	}
	else
	{
		m_username.EnableWindow(FALSE); 
		m_password.EnableWindow(FALSE); 
		m_remember_cred_checkbox.EnableWindow(FALSE);
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
				MessageBox( resource_string( IDS_REGISTERED_USER ), 
					resource_string( IDS_REGISTERED_USER_TITLE ) ) ;
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
			detach();
			END_DLG;
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

	// If the user wants to save credentials, store them now
	if (IsDlgButtonChecked(IDC_CONN_LOGIN) && IsDlgButtonChecked(IDC_REMEMBER_CREDENTIALS))
	{
		wstring connection_w = (LPCWSTR)conn_str;
		std::vector<wchar_t> connection_v(connection_w.begin(), connection_w.end());
		connection_v.push_back(0);

		wstring username_w = (LPCWSTR)username;
		std::vector<wchar_t> username_v(username_w.begin(), username_w.end());
		username_v.push_back(0);

		m_props->m_gen_props.set_credential(connection_w, username_w);
		m_props->m_gen_props.set_save_credentials(TRUE);
		m_props->save_prefs();

		wstring password_w = (LPCWSTR)password;
		string password_a = string2string(password_w);

		CREDENTIALW credential = { 0 };
		credential.Type = CRED_TYPE_GENERIC;
		credential.TargetName = (LPWSTR)&connection_v[0];
		credential.CredentialBlobSize = (DWORD)(password_a.size() + 1);
		credential.CredentialBlob = (LPBYTE)password_a.c_str();
		credential.Persist = CRED_PERSIST_LOCAL_MACHINE;
		credential.UserName = (LPWSTR)&username_v[0];

		BOOL success = ::CredWriteW(&credential, 0);
		if (! success)
		{
			CWinException e(L"Failed to write credentials");
			logging::log_exception(e);
		}
	}

	if (!IsDlgButtonChecked(IDC_REMEMBER_CREDENTIALS))
	{
		wstring connection_w = (LPCWSTR)conn_str;
		m_props->m_gen_props.remove_credential(connection_w);
		m_props->m_gen_props.set_save_credentials(FALSE);
		m_props->save_prefs();
	}

	MessageBeep(MB_ICONINFORMATION) ;
#endif
	detach();
	END_DLG;
}

LRESULT CConnectionDlg::OnCloseCommand( WORD wID )
{
	SENSE("OnClose") ;
	detach();
	END_DLG;
}

LRESULT CConnectionDlg::OnLoginChecked( WORD, WORD, HWND, BOOL& )
{
	const BOOL enable_login = IsDlgButtonChecked(IDC_CONN_LOGIN) ;
	m_username.EnableWindow(enable_login); 
	m_password.EnableWindow(enable_login); 
	m_remember_cred_checkbox.EnableWindow(enable_login);

	m_props->m_gen_props.set_must_log_in(enable_login) ;
	m_props->save_prefs();

	ATLTRACE("OnLoginChecked\n") ;
	return 0L ;
}

LRESULT CConnectionDlg::OnConnectionStringChanged(WORD, WORD, HWND, BOOL&)
{
	ATLTRACE("OnConnectionStringChanged\n");

	// If the user wants to save credentials, store them now
	if (!IsDlgButtonChecked(IDC_CONN_LOGIN) || !IsDlgButtonChecked(IDC_REMEMBER_CREDENTIALS))
	{
		ATLTRACE("User does not want to save credentials\n");
		return 0L;
	}

	// get the text in the window
	CString conn_str;
	m_edit.GetWindowText(conn_str);

	if (!m_props->m_gen_props.credential_is_saved((LPCWSTR)conn_str))
	{
		ATLTRACE("credential was not saved\n");
		return 0L;
	}

	CString username = m_props->m_gen_props.username_for_connection((LPCWSTR)conn_str).c_str();
	m_username.SetWindowText(username);

	try
	{
		CredentialReader reader((LPCWSTR)conn_str);
		m_password.SetWindowText(reader.get_password().c_str());
	}
	catch (except::CException& e)
	{
		logging::log_error("Failed to retrieve password");
		logging::log_exception(e);
		ATLASSERT(FALSE && "Failed to retrieve password");
	}

	return 0L;
}

void CConnectionDlg::detach()
{
	m_edit.Detach();
	m_username.Detach();
	m_password.Detach();
	m_remember_cred_checkbox.Detach();
}
