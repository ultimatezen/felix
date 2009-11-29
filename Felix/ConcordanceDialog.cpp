#include "stdafx.h"
#include "ConcordanceDialog.h"

tstring CConcordanceDialog::get_text()
{
	return m_text ;
}

LRESULT CConcordanceDialog::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE( "OnInitDialog" ) ;
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;
	m_edit.Attach( GetDlgItem( IDC_CONCORDANCE_EDIT ) ) ;

	return 0 ;
}

LRESULT CConcordanceDialog::OnCloseCommand( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	SENSE( "OnCloseCommand" ) ;

	if ( wID == IDCANCEL ) 
	{
		EndDialog( wID ) ;
		return 0L ;
	}

	CString text ;
	m_edit.GetWindowText( text ) ;
	m_text = static_cast< LPCTSTR >( text ) ;
	EndDialog(wID);
	return 0L ;
}