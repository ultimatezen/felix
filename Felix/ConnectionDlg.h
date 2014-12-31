
#pragma once


#include "resource.h"
#include "MessageMapDefines.h"
#include "WindowExceptionHandler.h"
#include "TranslationMemory.h"

/**
@class CConnectionDlg 
@brief Connect to a remote memory
*/
class CConnectionDlg : 
	public CDialogImpl<CConnectionDlg, CWindow>, 
	public CWindowExceptionHandler< CConnectionDlg >
{
	CCommandEdit	m_edit ;

	CCommandEdit	m_username ;
	CCommandEdit	m_password ;

	CWindow			m_remember_cred_checkbox;
public:
	static const int IDD = IDD_CONNECTION_DLG ;
	DECLARE_SENSING_VAR ;

	mem_engine::memory_pointer m_memory ;
	app_props::props_ptr m_props ;
	CConnectionDlg(app_props::props_ptr props);

	LRESULT OnInitDialog( );
	LRESULT OnOK(WORD wID);
	LRESULT OnCloseCommand(WORD wID);

	void detach();

	LRESULT OnLoginChecked(WORD, WORD, HWND, BOOL&);
	LRESULT OnConnectionStringChanged(WORD, WORD, HWND, BOOL&);

	BEGIN_MSG_MAP_EX(CConnectionDlg)
	MSG_TRY	
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)

		COMMAND_HANDLER(IDC_CONN_LOGIN, BN_CLICKED, OnLoginChecked)
		COMMAND_HANDLER(IDC_CONN_EDIT, EN_CHANGE, OnConnectionStringChanged)

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID(IDCANCEL, OnCloseCommand)
			CMD_HANDLER_EX_ID(IDOK,     OnOK)
		END_CMD_HANDLER_EX

	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};


