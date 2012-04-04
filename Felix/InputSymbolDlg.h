/*!
	@file InputSymbolDlg.h
	@brief interface for the CInputSymbolDlg class.
	@author Ryan Ginstrom
 */

#pragma once

#include "resource.h"
#include "Exceptions.h"		// exception handling
#include "logging.h"

#include "ATLCTRLS.H"		// CEdit

/**
	@class CInputSymbolDlg
	@brief Input an html entity symbol.
 */
class CInputSymbolDlg : 
	public CDialogImpl<CInputSymbolDlg, CWindow>
{
	CEdit	m_edit ;
public:
	static const int BUFSIZE = 512 ;
	TCHAR	m_text[BUFSIZE] ;
	static const int IDD = IDD_INPUT_SYMBOL_DLG ;
	DECLARE_SENSING_VAR ;

	CInputSymbolDlg()
	{
		ZeroMemory(m_text, BUFSIZE*sizeof(TCHAR)) ;
	}
	wstring get_text() const ;


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCloseCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CInputSymbolDlg)
	try
	{
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCommand)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCommand)
	}
	catch( except::CException &e )
	{
		logging::log_error("Error handling message in Input Symbol dialog.") ;
		logging::log_exception(e) ;
		e.notify_user( resource_string(IDS_MSG_ACTION_FAILED) ) ;
	}
	catch (std::exception&e)
	{
		logging::log_error("Error handling message in Input Symbol dialog.") ;
		logging::log_error(e.what()) ;
		CString emsg = CA2CT( e.what() ) ;
		except::CException myEx( emsg ) ;
		myEx.notify_user( resource_string(IDS_MSG_ACTION_FAILED) ) ;
	}
	catch ( _com_error &e ) 
	{
		logging::log_error("Error handling message in Input Symbol dialog.") ;
		logging::log_exception(e) ;
		except::CComException comEx( e ) ;
		comEx.notify_user( resource_string(IDS_MSG_ACTION_FAILED) ) ;
	}
	END_MSG_MAP()


};

