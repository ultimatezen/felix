/*!
	@file FindDlg.h
	@brief Interface file for the memory search dialog
	@date 2005/06/25
	Time: 9:26:20
	@author Ryan Ginstrom
 */

#pragma once

#include "record.h"				// for record_pointer
#include "query.h"				// for search_params_ptr
#include "Exceptions.h"			// for exception support

#include "MessageMapDefines.h"	// more crackers

#include "WindowExceptionHandler.h"
#include "active_window.h"

/**
	@class CFindDlg  
	@brief For searching entries.
 */
class CFindDlg  : 
		public CMessageFilter, 
		public CDialogImpl<CFindDlg, CWindow>,
		public CWindowExceptionHandler< CFindDlg >
		, public ActiveWindow
{
	mem_engine::search_query_params	m_params ;
	mem_engine::record_pointer		m_record ;

	CCommandEdit		m_source_edit ;
	CCommandEdit		m_trans_edit ;
	CCommandEdit		m_context_edit ;

	CCommandEdit		m_reliability_edit ;

public:
	static const int IDD = IDD_FIND_DLG ;
	DECLARE_SENSING_VAR ;

	CFindDlg()  ;
	BOOL PreTranslateMessage( MSG *pMsg ) ;

	mem_engine::record_pointer get_record() ;
	mem_engine::search_query_params &get_search_params() ;

	LRESULT OnDestroy( ) ;
	LRESULT OnInitDialog( ) ;
	LRESULT OnClose( ) ;
	LRESULT OnSearch( ) ;

	LRESULT OnRedo( ) ;

	bool check_focus( LRESULT &lResult, WPARAM wparam, LPARAM lparam ) ;

	bool has_focus( HWND focus_hwnd, CWindow window );
	BEGIN_MAP_EX( CFindDlg )
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)
		MSG_HANDLER_WIN_MSG(WM_ACTIVATE, on_activate)

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0(IDCANCEL, OnClose)
			CMD_HANDLER_EX_0(IDCLOSE, OnClose)
			CMD_HANDLER_EX_0(IDC_SEARCH, OnSearch )
			CMD_HANDLER_EX_0(ID_EDIT_REDO, OnRedo)

			else if ( check_focus(lResult, wParam, lParam) )
			{
				return TRUE ;
			}
		END_CMD_HANDLER_EX

	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )

};
