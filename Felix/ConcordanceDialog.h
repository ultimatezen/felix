/*!
	@file ConcordanceDialog.h
	@brief interface for CConcordanceDialog class.
	@author Ryan Ginstrom
 */

#pragma once

#include "resource.h"
#include "Exceptions.h"
#include "StringConversions.h"	// convert to/from wstrings/strings/tstrings
#include "WindowExceptionHandler.h"


/**
	@class CConcordanceDialog  
	@brief Dialog to get query for concordance.
 */
class CConcordanceDialog  : 
		public CDialogImpl< CConcordanceDialog, CWindow >, 
		public CWindowExceptionHandler< CConcordanceDialog >
{
	CEdit		m_edit ;
	tstring		m_text ;

public:
	static const int IDD = IDD_CONCORDANCE_DIALOG ;
	DECLARE_SENSING_VAR ;

	tstring get_text();

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CConcordanceDialog)
	MSG_TRY
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCommand)
		COMMAND_ID_HANDLER(IDCLOSE, OnCloseCommand)
		COMMAND_ID_HANDLER(IDOK, OnCloseCommand)
	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};
