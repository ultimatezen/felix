/*!
	@file WindowExceptionHandler.h
	@brief interface for the CWindowExceptionHandler class.
	@author Ryan Ginstrom
 */

#pragma once

#include "Exceptions.h"
#include "logging.h"

inline CString get_language_code()
{
	if(resource_string(IDS_LANG_CODE) == L"jp")
	{
		return L"Japanese" ;
	}
	return L"English" ;
}

inline void report_structured_windows_exception(const string &err_msg, except::CSWException &e)
{
	logging::log_error(err_msg) ;
	logging::log_exception(e) ;

	const CString language = get_language_code();
	logging::send_report(language, e.get_filename()) ;
}
/**
	@class CWindowExceptionHandler  
	@brief Handles exceptions thrown in window message-handler routine.
 */
template < class T >
class CWindowExceptionHandler  
{
public:
	virtual BOOL handle_exception( std::exception &e, const CString &failure_message )
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 
		message += _T("\rC Runtime Error \r") ;				 
		message += _T("Description: ") ;
		message += CA2T(e.what()) ;	 
		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg)) ;
		const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_SYSTEMMODAL ;
		::MessageBox( pT->m_hWnd, message, _T("C Runtime Error"), msg_flags ) ;  
		return FALSE ;											 
	}

	virtual BOOL handle_exception( _com_error &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg)) ;
		logging::log_exception(e) ;

		except::CComException com_exception(_T("COM Error"), e) ;		 
		com_exception.notify_user( message, MB_OK, _T("COM Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 
		const wstring werr_msg = (LPCWSTR)message ;
		const string err_msg = string2string(werr_msg) ;

		report_structured_windows_exception(err_msg, e) ;
		return FALSE ;											 
	}

	virtual BOOL handle_exception( except::CComException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message  ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("COM Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}	

	virtual BOOL handle_exception( except::CWinException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message  ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("Windows Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_exception( except::CException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}



	virtual ~CWindowExceptionHandler()
	{
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
		return window_text + TEXT(":\r\n" ) + failure_message;
	}

};

