#include "StdAfx.h"
#include "QueryMergeDlg.h"

CQueryMergeDlg::CQueryMergeDlg(int title_res_id, int text_res_id, const CString mem_name)
: m_bDontAsk(FALSE), 
	m_iMerge(IDC_MERGE)
{
	if(! m_title.LoadString(title_res_id))
	{
		logging::log_warn("Failed to load title resource ID " + int2string(title_res_id)) ;
	}
	m_merge_message.Format(resource_string(text_res_id), mem_name) ;
}


LRESULT CQueryMergeDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SENSE("OnInitDialog");
	SetIcon( LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_ICON ) ), TRUE ) ;
	this->SetWindowText(m_title) ;
	m_merge_static.Attach(this->GetDlgItem(IDC_DUP_MSG)) ;

#ifndef UNIT_TEST
	m_merge_static.SetWindowText(m_merge_message) ;
#endif	
	
	DoDataExchange(FALSE) ;
	SendDlgItemMessage( m_iMerge, BM_SETCHECK, TRUE, 0 ) ;
	return TRUE;
}

LRESULT CQueryMergeDlg::OnCloseCmd( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	SENSE("OnCloseCmd");
#ifndef UNIT_TEST
	if (wID == IDOK)
	{
		DoDataExchange(TRUE) ;
	}
#endif

	END_DLG ;
}

LRESULT CQueryMergeDlg::OnRadioMerge( WORD, WORD, HWND, BOOL& )
{
	m_iMerge = IDC_MERGE ;
	return 0;
}

LRESULT CQueryMergeDlg::OnRadioSeparate( WORD, WORD, HWND, BOOL& )
{
	m_iMerge = IDC_SEPARATE ;
	return 0;
}