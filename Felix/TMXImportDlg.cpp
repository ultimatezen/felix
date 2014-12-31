#include "stdafx.h"
#include "TmxImportDlg.h"

void CTMXImportDlg::init_combo()
{
#ifndef UNIT_TEST
	m_trans_combo.Attach( GetDlgItem(IDC_LANG_COMBO_TRANS) ) ;
#endif

	put_langs_into_combo( m_trans_combo ) ;

#ifndef UNIT_TEST
	m_trans_combo.SetCurSel( 0 ) ;
#endif
}

LRESULT CTMXImportDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE("OnInitDialog") ;
#ifndef UNIT_TEST
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	CenterWindow(GetParent());
#endif

	init_combo() ;

	return TRUE;
}

LRESULT CTMXImportDlg::OnCancel( WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */ )
{
	SENSE("OnCancel") ;
	END_DLG ;
}

LRESULT CTMXImportDlg::OnOK( WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */ )
{
	SENSE("OnOK") ;
	set_trans( m_trans_combo ) ;

	END_DLG ;
}