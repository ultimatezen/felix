#pragma once

#include "resource.h"
#include "StringEx.h"
#include "ui.h"			// windows_ui
#include "MessageMapDefines.h"

#include "WindowExceptionHandler.h"

/**
	@class CMultitermConversionDlg 
	@brief Dialog to get user input for import of MultiTerm glossary.
 */
class CMultitermConversionDlg : 
	public CDialogImpl< CMultitermConversionDlg, TWindow >
	, public CWinDataExchange< CMultitermConversionDlg >
	, public CWindowExceptionHandler< CMultitermConversionDlg >
{
VISIBLE_TO_TESTS
	int	m_multiterm_version ;
public:
	static const int IDD = IDD_MULTITERM_CONVERSION_DLG ;
	DECLARE_SENSING_VAR ;

	int get_multiterm_version() ;
	void set_multiterm_version( int setting ) ;

	CMultitermConversionDlg();
	virtual ~CMultitermConversionDlg();

	LRESULT OnInitDialog( ) ;
	LRESULT OnOK( ) ;
	LRESULT OnCancel( ) ;
	LRESULT OnMulti55( ) ;
	LRESULT OnMulti66( ) ;

	BEGIN_MAP_EX( CMultitermConversionDlg )
	
		MSG_HANDLER_0 ( WM_INITDIALOG,  OnInitDialog )
		
		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0( IDOK,          OnOK   )
			CMD_HANDLER_EX_0( IDCANCEL,      OnCancel   )
			CMD_HANDLER_EX(IDC_MULTI55, BN_CLICKED, OnMulti55)
			CMD_HANDLER_EX(IDC_MULTI6, BN_CLICKED, OnMulti66)
		END_CMD_HANDLER_EX
	
	END_MAP_EX( R2T( IDS_MSG_ACTION_FAILED ) )

};


