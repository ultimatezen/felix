/*!
	@brief Interface file for the CUserStringDlg class.
	@author Ryan Ginstrom
 */

#pragma once
#include <atlapp.h>			// must be included before atlctrls.h
							// must be included before atlmisc.h
#include <atlctrls.h>		// for member controls
#include "messagemapdefines.h"
#include "WideRichEdit.h"		// for CWideRichEdit
#include "Exceptions.h"			// for exception support
#include "WindowExceptionHandler.h"

/**
	@class CUserStringDlg 
	@brief Dialog to allow the user to enter a custom key-val pair.
 */
class CUserStringDlg : 
	public CDialogImpl<CUserStringDlg, TWindow>, 
	public CWindowExceptionHandler< CUserStringDlg >
{

	CWideRichEdit		m_key_edit ;
	CWideRichEdit		m_value_edit ;

public:
	enum {IDD = IDD_USER_STRING_DLG};
	DECLARE_SENSING_VAR ;

	wstring				m_key ;
	wstring				m_value ;
	
	BEGIN_MAP_EX(CUserStringDlg)
		MSG_HANDLER_0(WM_INITDIALOG, OnInitDialog)
		BEGIN_CMD_HANDLER_EX
			CMD_HANDLER_EX_0(IDCLOSE, OnClose)
			CMD_HANDLER_EX_0(IDCANCEL, OnClose)
			CMD_HANDLER_EX_ID(IDOK, OnOK)
		END_CMD_HANDLER_EX
	END_MAP_EX( _T("Action Failed") )

		LRESULT OnInitDialog( );
		LRESULT OnClose( ) ;
		LRESULT OnOK(WORD wID) ;

		const std::wstring& GetKey();
		const std::wstring& GetValue() ;
};

