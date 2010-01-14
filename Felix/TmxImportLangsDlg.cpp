#include "stdafx.h"
#include "TmxImportLangsDlg.h"

void CTMXImportLangsDlg::init_combo()
{
	m_trans_combo.Attach( GetDlgItem(IDC_LANG_TRANS_COMBO) ) ;
	m_source_combo.Attach( GetDlgItem(IDC_LANG_SOURCE_COMBO) ) ;

	put_langs_into_combo(m_source_combo) ;
	put_langs_into_combo( m_trans_combo ) ;

	m_trans_combo.SetCurSel( 0 ) ;
}

LRESULT CTMXImportLangsDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE("OnInitDialog") ;
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	CenterWindow(GetParent());

	init_combo() ;

	return TRUE;
}

LRESULT CTMXImportLangsDlg::OnCancel( WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */ )
{
	SENSE("OnCancel") ;
	EndDialog(wID);
	return 0;
}

LRESULT CTMXImportLangsDlg::OnOK( WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */ )
{
	SENSE("OnOK") ;
	set_trans(m_trans_combo) ;
	set_source(m_source_combo) ;

	EndDialog(wID);
	return 0;
}