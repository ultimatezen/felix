/*!
	@file MinViewDlg.h
	@brief interface for the CMinViewDlg class.
 */

#pragma once

#if WIN32_IE	<	0x0500 
#error WIN32_IE must be 0x0500 or higher
#endif

#pragma warning( disable:4995 ) // #pragma deprecated
#include <atlframe.h> // for CDialogResize<>
#pragma warning( default:4995 ) // #pragma deprecated

#include <atlctrls.h> // for member controls
#include "Exceptions.h"			// CException
#include "HtmlView.h"			// CHtmlView
#include "atlcrack.h"			// message crackers
#include "MessageMapDefines.h"	// more crackers
#include "Exceptions.h"			// CException

#include "WindowExceptionHandler.h"
#include "query.h"
#include "window_listener.h"

/**
	@class CMinViewDlg 
	@brief Minimal view to save screen real estate.
 */
class CMinViewDlg: public CDialogImpl<CMinViewDlg, CWindow >
                 , public CDialogResize<CMinViewDlg> 
				 , public CWindowExceptionHandler< CMinViewDlg >
{
	VISIBLE_TO_TESTS

	CToolTipCtrl	m_tooltips ;

	CButtonT<CWindow>     m_idcButton1;
	CStaticT<CWindow>     m_idcEditBox;
	HWND m_parent ;
	// view
	html::CHtmlView m_view ;
	WindowListener *m_listener ;
	mem_engine::felix_query *m_trans_matches ;
public:
		
	static const int IDD = IDD_MIN_VIEW_DLG ;
	DECLARE_SENSING_VAR ;

	CMinViewDlg();

	html::CHtmlDocument get_document();
	void set_body_text(wstring text);
	void set_parent(HWND parent);
	void set_listener(WindowListener *listener)
	{
		m_listener = listener ;
	}
	void set_matches(mem_engine::felix_query *matches)
	{
		m_trans_matches = matches ;
	}

	void show_content();
	void set_match(mem_engine::felix_query *matches);

	LRESULT OnLButtonDown( UINT id, CPoint pt );
	LRESULT OnMouse(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/ ) ;

	LRESULT OnInitDialog( );

	LRESULT OnSize( UINT type, CSize size ) ;
	
	LRESULT OnRestoreFullView( ) ;

	BOOL PreTranslateMessage( LPMSG pMsg );

	BEGIN_MAP_EX(CMinViewDlg)
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		MSG_WM_SIZE   ( OnSize )
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,WM_MOUSELAST, OnMouse)
		
		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX(IDC_MIN_BUTTON, 0, OnRestoreFullView)
		END_CMD_HANDLER_EX

		MSG_WM_LBUTTONDOWN( OnLButtonDown )

		CHAIN_MSG_MAP(CDialogResize<CMinViewDlg>)
	END_MAP_EX( _T("Action Failed") )

	// Resize map.

	BEGIN_DLGRESIZE_MAP(CMinViewDlg) // Flags: DLSZ_SIZE_X or DLSZ_MOVE_X, DLSZ_SIZE_Y  or DLSZ_MOVE_X, DLSZ_REPAINT
			BEGIN_DLGRESIZE_GROUP()
				DLGRESIZE_CONTROL(IDC_EDIT_BOX,            DLSZ_SIZE_X | DLSZ_SIZE_Y)
			END_DLGRESIZE_GROUP()
	END_DLGRESIZE_MAP()


}; // class CMinViewDlg

