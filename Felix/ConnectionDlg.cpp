#include "stdafx.h"
#include "ConnectionDlg.h"
#include "DemoException.h"
#include "InputKeyDlg.h"

using namespace except ;

LRESULT CConnectionDlg::OnInitDialog()
{
	SENSE("OnInitDialog") ;

	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	m_edit.Attach( GetDlgItem( IDC_CONN_EDIT ) ) ;
	m_err_box.Attach( GetDlgItem( IDC_MSG_BOX ) ) ;

#ifdef UNIT_TEST
	return TRUE ;
#endif

	// set the focus to the edit box
	m_edit.SetSel(0, 0) ;
	m_edit.SetFocus() ;
	return TRUE;
}

LRESULT CConnectionDlg::OnOK( WORD wID )
{
	SENSE("OnOK") ;

#ifdef UNIT_TEST
	return 0L ;
#endif

	using namespace memory_engine ;

	// get the text in the window
	CString conn_str ;
	m_edit.GetWindowText( conn_str ) ;

	memory_remote *mem = new memory_remote() ;
	memory_pointer pmem(mem) ;
	try
	{
		mem->connect(conn_str) ;
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
			EndDialog(IDCANCEL) ;
			return 0L ;
		}

	}
	catch (CException& e)
	{
		e ;
		MessageBeep(MB_ICONSTOP) ;
		this->FlashWindow(FALSE) ;
		this->SetMsgHandled(FALSE) ;
		// set the focus to the edit box
		m_edit.SetSel(0, -1) ;
		m_edit.SetFocus() ;
		return 1L ;
	}

	m_memory = pmem ;
	MessageBeep(MB_ICONINFORMATION) ;
	EndDialog(wID);
	return 0L ;
}

LRESULT CConnectionDlg::OnCloseCommand( WORD wID )
{
	SENSE("OnClose") ;
	EndDialog(wID);
	return 0L;
}

CConnectionDlg::CConnectionDlg() : m_memory( new memory_engine::memory_remote())
{

}
