/*!
	@brief Interface file for CTMXImportLangsDlg class.
	@author Ryan Ginstrom
 */

#pragma once

#include "resource.h"       // main symbols
#include <vector>
#include <set>
#include <map>
#include "Exceptions.h"		// exception support
#include "WindowExceptionHandler.h"

#include "LanguageHolder.h"

typedef std::vector< tstring >		string_list ;
typedef std::map< tstring, tstring >	string_map ;

// CTMXImportLangsDlg
class CTMXImportLangsDlg : 
	public CDialogImpl< CTMXImportLangsDlg, CWindow >
	, public CWindowExceptionHandler< CTMXImportLangsDlg >
	, public CLanguageHolder
{

	CComboBoxT<CWindow>		m_source_combo ;
	CComboBoxT<CWindow>		m_trans_combo ;

public:
	static const int IDD = IDD_SELECT_LANGS_DLG ;
	DECLARE_SENSING_VAR ;

	CTMXImportLangsDlg() {}
	~CTMXImportLangsDlg() {}

	// message handlers 
	void init_combo();

	// message handlers 
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */);
	LRESULT OnOK(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */);

	BEGIN_MSG_MAP(CTMXImportLangsDlg)
		MSG_TRY
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnOK)
			COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};




