#pragma once

#include "resource.h"
#include "StringEx.h"				// extended strings/char stream support
#include "StringConversions.h"		// convert to/from wstrings/strings/tstrings/nums
#include "MessageMapDefines.h"
#include "Exceptions.h"
#include "WindowExceptionHandler.h"

class CZoomInterface
{
public:
	virtual void set_zoom_level(int zoom_level) = 0 ;
};


/**
@class CZoomDlg 
@brief Sets the zoom ratio for the window.
*/
class CZoomDlg : 
	public CDialogImpl< CZoomDlg, TWindow >, 
	public CWindowExceptionHandler< CZoomDlg >
{
    CTrackBarCtrl m_ZoomCtrl; // ZOOM
	
public:
	enum { IDD = IDD_ZOOM_DLG };
	DECLARE_SENSING_VAR ;
	CZoomInterface *m_interface ;
	int m_zoom_level ;
	LRESULT OnInitDialog( );
	LRESULT OnOK(WORD wID);
	LRESULT OnCancel(WORD wID);
	void OnZoomCtrl( int /*iType*/, short /*iTrackPos*/, HWND /*hWndTrackBar*/ );
	BEGIN_MSG_MAP_EX(CZoomDlg)
		MSG_TRY	
		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )
		MSG_WM_HSCROLL(OnZoomCtrl) // ZOOM
		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID ( IDCANCEL, OnCancel )
			CMD_HANDLER_EX_ID ( IDOK,     OnOK )
		END_CMD_HANDLER_EX

		MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()
};


