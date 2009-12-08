/*!
	@file InputKeyDlg.h
	@brief interface for the CInputKeyDlg class.
	@author Ryan Ginstrom
 */

#pragma once


#include "resource.h"
#include "StringEx.h"				// extended strings/char stream support
#include "StringConversions.h"		// convert to/from wstrings/strings/tstrings/nums
#include "ArmadilloStuff.h"
#include "MessageMapDefines.h"
#include "Exceptions.h"
#include "WindowExceptionHandler.h"


/**
	@class CInputKeyDlg 
	@brief Input the key to register the software.
 */
class CInputKeyDlg : 
	public CDialogImpl< CInputKeyDlg, TWindow >, 
	public CWindowExceptionHandler< CInputKeyDlg >
{
	CEdit	m_edit ;

public:
	static const int IDD = IDD_INPUT_KEY_DLG ;
	DECLARE_SENSING_VAR ;

	LRESULT OnInitDialog( );
	LRESULT OnOK(  WORD wID );
	LRESULT OnCloseCommand(  WORD wID );

	BEGIN_MSG_MAP_EX(CInputKeyDlg)
	MSG_TRY	
		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID ( IDCANCEL, OnCloseCommand )
			CMD_HANDLER_EX_ID ( IDOK,     OnOK )
		END_CMD_HANDLER_EX

	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};


