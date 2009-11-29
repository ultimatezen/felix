/*!
	@file FindDlg.h
	@brief Interface file for the memory search dialog
	@date 2005/06/25
	Time: 9:26:20
	@author Ryan Ginstrom
	@dir c:\Programming\Programming Projects\Felix 1.5
 */

#pragma once

#include <atlbase.h>			// must be included before atlapp.h
#include "atlapp.h"				// CMessageFilter
								// must be included before atlmisc.h
#include "resource.h"
#include "record.h"				// for record_pointer
#include "query.h"				// for search_params_ptr
#include "WideRichEdit.h"		// for CWideRichEdit
#include "Exceptions.h"			// for exception support
#include "Accelerator.h"		// CAccelerator

#include "MessageMapDefines.h"	// more crackers

#include "WindowExceptionHandler.h"

/**
	@class CFindDlg  
	@brief For searching entries.
 */
class CFindDlg  : 
		public CMessageFilter, 
		public CDialogImpl<CFindDlg, TWindow>,
		public CWindowExceptionHandler< CFindDlg >
{
	memory_engine::search_query_params	m_params ;
	memory_engine::record_pointer		m_record ;

	CWideRichEdit		m_source_edit ;
	CWideRichEdit		m_trans_edit ;
	CWideRichEdit		m_context_edit ;

	CCommandEdit		m_reliability_edit ;

public:
	enum { IDD = IDD_FIND_DLG } ;
	DECLARE_SENSING_VAR ;

	CFindDlg()  ;
	BOOL PreTranslateMessage( MSG *pMsg ) ;

	memory_engine::record_pointer get_record() ;
	memory_engine::search_query_params &get_search_params() ;

	LRESULT OnDestroy( ) ;
	LRESULT OnInitDialog( ) ;
	LRESULT OnActivate( WPARAM wParam )  ;
	LRESULT OnClose( ) ;
	LRESULT OnSearch( ) ;

	LRESULT OnRedo( ) ;

	bool check_focus( LRESULT &lResult, WPARAM wparam, LPARAM lparam ) ;

	BEGIN_MAP_EX( CFindDlg )
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		MSG_HANDLER_0(WM_DESTROY, OnDestroy)
		MSG_HANDLER_1 (WM_ACTIVATE,   OnActivate,    wParam)

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
