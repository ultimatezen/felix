#pragma once

#include "resource.h"
#include "HtmlView.h"			// CHtmlView
//#include <atlapp.h>				// must be included before atlmisc.h
//#include "atlmisc.h"			// CSize

#pragma warning( disable:4995 ) // #pragma deprecated
#include "atlframe.h"			// CDialogResize
#pragma warning( default:4995 ) // #pragma deprecated

#include "MessageMapDefines.h"	// atlcrack.h + custom message map defines

#include "WindowExceptionHandler.h"

#include "resource_string.h"

/**
	@class CNagDialog  
	@brief Nag dialog for demo version.
 */
class CNagDialog  
	: 
	public CDialogImpl< CNagDialog, TWindow >, 
	public CDialogResize< CNagDialog >, 
	public CWindowExceptionHandler< CNagDialog >
{
	// view
	html::CHtmlView	m_view ;
	CString		m_nagPage ;

public:
	enum { IDD = IDD_NAG_BOX };
	DECLARE_SENSING_VAR ;

	LRESULT OnInitDialog( );
	LRESULT OnClose( WORD wID  );

	CNagDialog( CString nagPage = _T("NAGPAGE.HTML") );
	virtual ~CNagDialog();

	LRESULT OnSize( UINT type, CSize size ) ;

    BEGIN_DLGRESIZE_MAP(CNagDialog)
        // buttons
        DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X|DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X|DLSZ_MOVE_Y)

        // WebBrowser control
        DLGRESIZE_CONTROL(IDC_NAG_MSG_BOX, DLSZ_SIZE_X|DLSZ_SIZE_Y)

    END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP_EX(CNagDialog)
	MSG_TRY	

		MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )
		MSG_WM_SIZE   ( OnSize )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID ( IDCANCEL, OnClose ) 
			CMD_HANDLER_EX_ID ( IDOK,     OnClose )
			PASS_CMDS_TO_VIEW_EX( m_view )
		END_CMD_HANDLER_EX

        CHAIN_MSG_MAP( CDialogResize<CNagDialog> )
	MSG_CATCH( R2T( IDS_MSG_ACTION_FAILED ) )
	END_MSG_MAP()

};
