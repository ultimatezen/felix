/*!
	@file InputKeyDlg.h
	@brief interface for the CInputKeyDlg class.
	@author Ryan Ginstrom
 */

#pragma once

#include "resource.h"
#include "MessageMapDefines.h"
#include "Exceptions.h"
#include "logging.h"

/**
	@class CInputKeyDlg 
	@brief Input the key to register the software.
 */

class CInputKeyDlg : 
	public CDialogImpl< CInputKeyDlg, CWindow >
{
	CCommandEdit	m_edit ;

public:
	DECLARE_SENSING_VAR ;
	const static int IDD = IDD_INPUT_KEY_DLG ;

	LRESULT OnInitDialog( );
	LRESULT OnOK(  WORD wID );

	// strip whitespace from key.
	// Also strip quote marks (>), in case email is forwarded, etc.
	string get_key_text( CString text );

	LRESULT OnCloseCommand(  WORD wID );

	BEGIN_MSG_MAP_EX(CInputKeyDlg)
		try
		{
			MSG_HANDLER_0 ( WM_INITDIALOG, OnInitDialog )

			BEGIN_CMD_HANDLER_EX
				CMD_HANDLER_EX_ID ( IDCANCEL, OnCloseCommand )
				CMD_HANDLER_EX_ID ( IDOK,     OnOK )
			END_CMD_HANDLER_EX
		}
		catch (except::CException& e)
		{
			logging::log_error("Program exception") ;
			logging::log_exception(e) ;
			
		}
		catch (_com_error& e)
		{
			logging::log_error("COM exception") ;
			logging::log_exception(e) ;
		}
		catch (std::exception& e)
		{
			logging::log_error("C runtime exception") ;
			logging::log_error(e.what()) ;
		}
	END_MSG_MAP()


};