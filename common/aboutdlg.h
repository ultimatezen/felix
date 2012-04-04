#pragma once

#include "HtmlView.h"			// CHtmlView
#include "atlcrack.h"			// message crackers
#include "MessageMapDefines.h"	// more crackers
#include "wingdi.h"
#include "resource.h"

/**
	@class CAboutDialog
	@brief Generic about dialog with HTML content.
 */
class CAboutDialog : 
		public CDialogImpl<CAboutDialog, CWindow > 
		, public CDialogResize< CAboutDialog  >
		, public CWindowExceptionHandler< CAboutDialog  >
{

	// view
	html::CHtmlView	m_view ;

public:
	static const int IDD = IDD_ABOUTBOX ;
	CString m_aboutfile_name ;

	DECLARE_SENSING_VAR ;

	CAboutDialog(CString aboutfile_name=_T("")) : m_aboutfile_name(aboutfile_name)
	{
	}
	// Function name	: CAboutDialog::OnSize
	// Description	    : 
	// Return type		: LRESULT 
	// Argument         :  UINT type
	// Argument         : CSize size
	LRESULT OnSize( UINT type, CSize size );
	
	
	// Function name	: CAboutDialog::OnInitDialog
	// Description	    : 
	// Return type		: LRESULT 
	LRESULT OnInitDialog( );
	
	
	// Function name	: CAboutDialog::OnCloseCmd
	// Description	    : 
	// Return type		: LRESULT 
	// Argument         : WORD wID
	LRESULT OnCloseCmd( WORD wID );

    BEGIN_DLGRESIZE_MAP(CAboutDialog< int id_type >)
        // buttons
        DLGRESIZE_CONTROL(IDCLOSE, DLSZ_MOVE_X|DLSZ_MOVE_Y)

        // WebBrowser control
        DLGRESIZE_CONTROL(IDC_ABOUT_MSG_BOX, DLSZ_SIZE_X|DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP_EX( CAboutDialog< int id_type > )
		MSG_HANDLER_0 ( WM_INITDIALOG,  OnInitDialog )
 		MSG_WM_SIZE   ( OnSize )

		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_ID( IDOK,          OnCloseCmd   )
			CMD_HANDLER_EX_ID( IDCANCEL,      OnCloseCmd   )
			CMD_HANDLER_EX_ID( IDCLOSE,       OnCloseCmd   )
		// allow copy and other operations
			PASS_CMDS_TO_VIEW_EX( m_view ) 
		END_CMD_HANDLER_EX

      CHAIN_MSG_MAP( CDialogResize<CAboutDialog> )

	END_MSG_MAP( )

};

