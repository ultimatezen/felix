// InputDlg.h: interface for the InputBoxDlg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Exceptions.h"			// exception handling
#include "StringEx.h"			// string helper functions/classes
#include "StringConversions.h"	// convert to/from std strings
#include "ATLCTRLS.H"			// CEdit
#include "resource.h"			// resource file
#include "InputEdit.h"
#include "logging.h"

// class CInputBoxDlg : public CDialogImpl<CInputBoxDlg>
class CInputDlg : public CDialogImpl<CInputDlg>
{
	CInputEdit	m_edit ;
	long		m_num ;

public:
	enum { IDD = IDD_INPUT_BOX };
	
	CInputDlg() : m_num(0) {}
	
	long get_num () { return m_num ; }
	
	BEGIN_MSG_MAP(CInputDlg)
	MSG_TRY
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	MSG_CATCH( _T("Action failed") )
	END_MSG_MAP()
		
		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_edit.Attach( GetDlgItem( IDC_INPUT_EDIT ) ) ;
		m_edit.SetSelAll( ) ;
		m_edit.SetWindowText( _T("0") ) ;
		
		return TRUE;
	}
	
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString window_text ;

		m_edit.GetWindowText(window_text) ;
		
		m_num = _tcstol( window_text, NULL, 10 ) ;
		
		EndDialog(wID);
		return 0;
	}
	virtual BOOL handle_exception( std::exception &e, const CString &failure_message )
	{
		CString message( build_window_err_msg( m_hWnd, failure_message ) ) ;	 
		message += _T("\rC Runtime Error \r") ;				 
		message += _T("Description: ") ;
		message += CA2T(e.what()) ;	 
		wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_SYSTEMMODAL ;
		MessageBox(message, _T("C Runtime Error"), msg_flags ) ;  
		return FALSE ;											 
	}

	virtual BOOL handle_exception( _com_error &e, const CString &failure_message ) 
	{
		CString message( build_window_err_msg( m_hWnd, failure_message ) ) ;	 

		wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		except::CComException com_exception(_T("COM Error"), e) ;		 
		com_exception.notify_user( message, MB_OK, _T("COM Exception"), m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message ) 
	{
		CString message( build_window_err_msg( m_hWnd, failure_message ) ) ;	 

		wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("Structured Windows Exception"), m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_exception( except::CComException &e, const CString &failure_message ) 
	{
		CString message( build_window_err_msg( m_hWnd, failure_message ) ) ;	 

		wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("COM Exception"), m_hWnd ) ;		 
		return FALSE ;											 
	}	

	virtual BOOL handle_exception( except::CWinException &e, const CString &failure_message ) 
	{
		CString message( build_window_err_msg( m_hWnd, failure_message ) ) ;	 

		wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("Windows Exception"), m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_exception( except::CException &e, const CString &failure_message ) 
	{
		CString message( build_window_err_msg( m_hWnd, failure_message ) ) ;	 

		wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("Exception"), m_hWnd ) ;		 
		return FALSE ;											 
	}

private:
	CString build_window_err_msg( HWND hwnd, const CString &failure_message )
	{
		CWindow window( hwnd ) ;

		CString window_text( _T("Exception") ) ;

		if ( window.IsWindow() ) 
		{
			window.FlashWindow( FALSE ) ;
			window.GetWindowText( window_text ) ;
		}
		CString message( window_text ) ; 
		message += TEXT(":\r\n" ) ;
		message += failure_message ;
		return message ;
	}};

