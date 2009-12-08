
#pragma once


#include "resource.h"
#include "MessageMapDefines.h"
#include "WindowExceptionHandler.h"
#include "memory_remote.h"

/**
@class CConnectionDlg 
@brief Connect to a remote memory
*/
class CConnectionDlg : 
	public CDialogImpl<CConnectionDlg, TWindow>, 
	public CWindowExceptionHandler< CConnectionDlg >
{
	CEdit	m_edit ;
	CStatic m_err_box ;


public:
	static const int IDD = IDD_CONNECTION_DLG ;
	DECLARE_SENSING_VAR ;

	memory_engine::memory_pointer m_memory ;
	CConnectionDlg();

	LRESULT OnInitDialog( );
	LRESULT OnOK(  WORD wID );
	LRESULT OnCloseCommand(WORD wID);

	BEGIN_MSG_MAP_EX(CConnectionDlg)
	MSG_TRY	
		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID ( IDCANCEL, OnCloseCommand )
			CMD_HANDLER_EX_ID ( IDOK,     OnOK )
		END_CMD_HANDLER_EX

	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};


