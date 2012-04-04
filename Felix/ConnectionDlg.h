
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
	CEdit	m_edit ;
	CStatic m_err_box ;

	CEdit	m_username ;
	CEdit	m_password ;

public:
	static const int IDD = IDD_CONNECTION_DLG ;
	DECLARE_SENSING_VAR ;

	mem_engine::memory_pointer m_memory ;
	app_props::props_ptr m_props ;
	CConnectionDlg(app_props::props_ptr props);

	LRESULT OnInitDialog( );
	LRESULT OnOK(  WORD wID );
	LRESULT OnCloseCommand(WORD wID);

	LRESULT OnLoginChecked(WORD, WORD, HWND, BOOL& );


	BEGIN_MSG_MAP_EX(CConnectionDlg)
	MSG_TRY	
		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )

		COMMAND_HANDLER(IDC_CONN_LOGIN, BN_CLICKED, OnLoginChecked)

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID ( IDCANCEL, OnCloseCommand )
			CMD_HANDLER_EX_ID ( IDOK,     OnOK )
		END_CMD_HANDLER_EX

	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};


