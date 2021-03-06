#pragma once

#include "resource.h"       // main symbols
#include "WindowExceptionHandler.h"

#include "LanguageHolder.h"

/**
@class CImportDialog 
@brief Select source/target languages when importing memories.
*/
class CImportDialog : 
	public CDialogImpl<CImportDialog, CWindow>
	, public CWindowExceptionHandler< CImportDialog >
	, public CLanguageHolder
{

	CComboBox		m_source_combo ;
	CComboBox		m_trans_combo ;

public:
	static const int IDD = IDD_IMPORT_DIALOG ;
	DECLARE_SENSING_VAR ;

	CImportDialog() {}
	~CImportDialog() {}

	// message handlers 
	void init_combos();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */);
	LRESULT OnOK(WORD /* wNotifyCode */, WORD wID, HWND /* hWndCtl */, BOOL& /* bHandled */);

	BEGIN_MSG_MAP(CImportDialog)
		MSG_TRY
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};


/**
@class CExportDialog  
@brief Dialog for selecting languages during export.
*/
class CExportDialog : 
	public CDialogImpl<CExportDialog, CWindow>
	, public CWindowExceptionHandler< CImportDialog >
{
	CComboBoxT<CWindow>		m_source_combo ;
	CComboBoxT<CWindow>		m_trans_combo ;

	typedef std::map< tstring, tstring > string_map ;

	string_map	m_languages  ;
	tstring		m_source ;
	tstring		m_trans ;
	tstring		m_source_full ;
	tstring		m_trans_full ;

public:
	static const int IDD = IDD_IMPORT_DIALOG ;
	DECLARE_SENSING_VAR ;

	CExportDialog();

	// message handlers 
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	const tstring get_combo_sel(CComboBoxT<CWindow> &combo) const;
	const tstring get_source_plain() const;
	const tstring get_trans_plain() const;
	const tstring get_source_full() const;
	const tstring get_trans_full() const;

	BEGIN_MSG_MAP(CExportDialog)
		MSG_TRY
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};

