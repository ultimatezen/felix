#include "stdafx.h"
#include "ImportDialog.h"

void CImportDialog::init_combos()
{
	m_source_combo.Attach( GetDlgItem(IDC_SOURCE_COMBO) ) ;
	m_trans_combo.Attach( GetDlgItem(IDC_TRANS_COMBO) ) ;

	put_langs_into_combo( m_source_combo ) ;
	put_langs_into_combo( m_trans_combo ) ;

	m_source_combo.SetCurSel( 0 ) ;
	m_trans_combo.SetCurSel( 0 ) ;
}

LRESULT CImportDialog::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE("OnInitDialog"); 
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	CenterWindow(GetParent());

	init_combos();

	return TRUE;
}

LRESULT CImportDialog::OnCancel( WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */ )
{
	SENSE("OnCancel"); 
	wID ;
#ifndef UNIT_TEST
	EndDialog(wID);
#endif
	return 0;
}

LRESULT CImportDialog::OnOK( WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */ )
{
	SENSE("OnOK"); 
	wID ;
	set_source( m_source_combo ) ;
	set_trans( m_trans_combo ) ;

	TRUE_ENFORCE( get_source_plain() != get_trans_plain(), IDS_MSG_S_T_MUST_DIFFER ) ;

#ifndef UNIT_TEST
	EndDialog(wID);
#endif
	return 0;
}

CExportDialog::CExportDialog()
{
	m_languages[_T("Japanese")]		= _T("JA") ;
	m_languages[_T("English")]		= _T("EN-US") ;
	m_languages[_T("Afrikaans")]	= _T("AF") ;
	m_languages[_T("Albanian")]		= _T("SQ") ;
	m_languages[_T("Arabic")]		= _T("AR") ;
	m_languages[_T("Basque")]		= _T("EU") ;
	m_languages[_T("Bulgarian")]	= _T("BG") ;
	m_languages[_T("Byelorussian")] = _T("BE") ;
	m_languages[_T("Catalan")]		= _T("CA") ;
	m_languages[_T("Chinese Simplified")]	= _T("ZH-CN") ;
	m_languages[_T("Chinese Traditional")]	= _T("ZH-TW") ;
	m_languages[_T("Croatian")]		= _T("HR") ;
	m_languages[_T("Czech")]		= _T("CS") ;
	m_languages[_T("Danish")]		= _T("DA") ;
	m_languages[_T("Dutch")]		= _T("NL") ;
	m_languages[_T("English")]					= _T("EN") ;
	m_languages[_T("English (United States)")]	= _T("EN-US") ;
	m_languages[_T("English (United Kingdom)")]	= _T("EN-GB") ;
	m_languages[_T("English (Australia)")]		= _T("EN-AU") ;
	m_languages[_T("English (Canada)")]			= _T("EN-CA") ;
	m_languages[_T("English (New Zealand)")]	= _T("EN-NZ") ;
	m_languages[_T("English (Ireland)")]		= _T("EN-IE") ;
	m_languages[_T("Estonian")]		= _T("ET") ;
	m_languages[_T("Faeroese")]		= _T("FO") ;
	m_languages[_T("Finnish")]		= _T("FI") ;
	m_languages[_T("French")]		= _T("FR") ;
	m_languages[_T("Gaelic")]			= _T("GA") ;
	m_languages[_T("Gaelic (Ireland)")]	= _T("GA-IE") ;
	m_languages[_T("German")]		= _T("DE") ;
	m_languages[_T("Greek")]		= _T("EL") ;
	m_languages[_T("Hebrew")]		= _T("IW") ;
	m_languages[_T("Hungarian")]	= _T("HU") ;
	m_languages[_T("Icelandic")]	= _T("IS") ;
	m_languages[_T("Indonesian")]	= _T("IN") ;
	m_languages[_T("Italian")]		= _T("IT") ;
	m_languages[_T("Japanese")]		= _T("JA") ;
	m_languages[_T("Kampuchean")]	= _T("KA") ;
	m_languages[_T("Korean")]		= _T("KO") ;
	m_languages[_T("Laothian")]		= _T("LO") ;
	m_languages[_T("Latvian, Lettish")] = _T("LV") ;
	m_languages[_T("Lithuanian")]	= _T("LT") ;
	m_languages[_T("Macedonian")]	= _T("MK") ;
	m_languages[_T("Maltese")]		= _T("MT") ;
	m_languages[_T("Maori")]		= _T("MI") ;
	m_languages[_T("Norwegian")]	= _T("NO") ;
	m_languages[_T("Polish")]		= _T("PL") ;
	m_languages[_T("Portuguese")]	= _T("PT") ;
	m_languages[_T("Punjabi")]		= _T("PA") ;
	m_languages[_T("Rhaeto-Romance")]	= _T("RM") ;
	m_languages[_T("Romanian")]			= _T("RO") ;
	m_languages[_T("Russian")]			= _T("RU") ;
	m_languages[_T("Serbo-Croatian")]	= _T("SH") ;
	m_languages[_T("Slovak")]		= _T("SK") ;
	m_languages[_T("Slovenian")]	= _T("SL") ;
	m_languages[_T("Serbian")]		= _T("SO") ;
	m_languages[_T("Spanish")]		= _T("ES") ;
	m_languages[_T("Swedish")]		= _T("SV") ;
	m_languages[_T("Thai")]			= _T("TH") ;
	m_languages[_T("Tsonga")]		= _T("TS") ;
	m_languages[_T("Turkish")]		= _T("TR") ;
	m_languages[_T("Ukrainian")]	= _T("UK") ;
	m_languages[_T("Urdu")]			= _T("UR") ;
	m_languages[_T("Vietnamese")]	= _T("VI") ;
}

LRESULT CExportDialog::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE("OnInitDialog"); 
	// set icon
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINFRAME) ), FALSE ) ;

	CenterWindow(GetParent());
	m_source_combo.Attach( GetDlgItem( IDC_SOURCE_COMBO ) ) ;
	m_trans_combo.Attach( GetDlgItem( IDC_TRANS_COMBO ) ) ;

	string_map::iterator pos = m_languages.begin() ;
	while ( pos != m_languages.end() )
	{
		m_source_combo.AddString( string2tstring( pos->first ).c_str() ) ;
		m_trans_combo.AddString( string2tstring( pos->first ).c_str() ) ;
		++pos ;
	}

	m_source_combo.SetCurSel( 0 ) ;
	m_trans_combo.SetCurSel( 0 ) ;
	return TRUE;
}

LRESULT CExportDialog::OnCancel( WORD, WORD wID, HWND, BOOL& )
{
	SENSE("OnCancel"); 
	wID ;
#ifndef UNIT_TEST
	EndDialog(wID);
#endif
	return 0;
}

LRESULT CExportDialog::OnOK(WORD, WORD wID, HWND, BOOL&)
{
	SENSE("OnOK"); 
	wID ;

	m_source_full = get_combo_sel(m_source_combo) ;
	m_source = m_languages[ m_source_full ]  ;

	m_trans_full = get_combo_sel(m_trans_combo) ;
	m_trans = m_languages[ m_trans_full ]  ;

#ifndef UNIT_TEST
	EndDialog(wID);
#endif
	return 0;
}

const tstring CExportDialog::get_source_plain() const
{
	return m_source ;
}

const tstring CExportDialog::get_source_full() const
{
	return m_source_full ;
}

const tstring CExportDialog::get_trans_full() const
{
	return m_trans_full ;
}

const tstring CExportDialog::get_trans_plain() const
{
	return m_trans ;
}

// Get the text for the selected item in the combo box
const tstring CExportDialog::get_combo_sel( CComboBoxT<TWindow> &combo ) const
{
	CString text ;
	combo.GetLBText(combo.GetCurSel(), text) ;
	return tstring(static_cast<LPCTSTR>(text)) ;
}