/*!
	@brief Interface file for CTMXImportDlg class.
	@author Ryan Ginstrom
 */

#pragma once

#include "tmx_dialogs.h"

// CTMXImportDlg
class CTMXImportDlg : 
	public CDialogImpl< CTMXImportDlg, CWindow >
	, public CWindowExceptionHandler< CTMXImportDlg >
	, public CLanguageHolder
{

	CComboBox		m_trans_combo ;


public:
	static const int IDD = IDD_SELECT_LANG_DLG ;
	DECLARE_SENSING_VAR ;

	CTMXImportDlg() {}
	~CTMXImportDlg() {}

	// message handlers 
	void init_combo();

	// message handlers 
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */);
	LRESULT OnOK(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */);

	BEGIN_MSG_MAP(CTMXImportDlg)
		MSG_TRY
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnOK)
			COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};




