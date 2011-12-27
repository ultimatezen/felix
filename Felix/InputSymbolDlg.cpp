#include "stdafx.h"
#include "InputSymbolDlg.h"

LRESULT CInputSymbolDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE( "OnInitDialog" ) ;

	// set icon
	SetIcon( LoadIcon( _Module.GetModuleInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	CenterWindow(GetParent());

	m_edit.Attach( GetDlgItem( IDC_EDIT ) ) ;
#ifndef UNIT_TEST
	m_edit.SetSelAll( ) ;
#endif
	return TRUE;
}

LRESULT CInputSymbolDlg::OnCloseCommand( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	SENSE( "OnCloseCommand" ) ;

#ifndef UNIT_TEST
	m_edit.GetWindowText( m_text, BUFSIZE ) ;
#endif	
	END_DLG ;
}

wstring CInputSymbolDlg::get_text() const
{
	if ( m_text[0] == 0 )
	{
		return wstring() ;
	}

	return string2wstring((tformat(_T("&%1%;")) % m_text).str()) ;
}
