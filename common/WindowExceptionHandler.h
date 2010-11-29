/*!
	@file WindowExceptionHandler.h
	@brief interface for the CWindowExceptionHandler class.
	@date 2005/06/25
	Time: 15:35:08
	@author Ryan Ginstrom
	@dir c:\Programming\MyLibrary
 */

#pragma once

#include "Exceptions.h"
#include "logging.h"

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
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		const UINT msg_flags = MB_OK | MB_ICONSTOP | MB_SETFOREGROUND | MB_SYSTEMMODAL ;
		::MessageBox( pT->m_hWnd, message, _T("C Runtime Error"), msg_flags ) ;  
		return FALSE ;											 
	}

	virtual BOOL handle_exception( _com_error &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		except::CComException com_exception(_T("COM Error"), e) ;		 
		com_exception.notify_user( message, MB_OK, _T("COM Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_sw_exception( except::CSWException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		CString language = L"English" ;
		const CString lang_code = resource_string(IDS_LANG_CODE);
		if(lang_code == L"jp")
		{
			language = L"Japanese" ;
		}
		logging::send_report(language, e.get_filename()) ;
		return FALSE ;											 
	}

	virtual BOOL handle_exception( except::CComException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message  ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("COM Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}	

	virtual BOOL handle_exception( except::CWinException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message  ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
		logging::log_exception(e) ;

		e.notify_user( message, MB_OK, _T("Windows Exception"), pT->m_hWnd ) ;		 
		return FALSE ;											 
	}

	virtual BOOL handle_exception( except::CException &e, const CString &failure_message ) 
	{
		T* pT = static_cast< T* >(this) ;
		CString message( build_window_err_msg( pT->m_hWnd, failure_message ) ) ;	 

		const wstring err_msg = (LPCWSTR)message ;
		logging::log_error(string2string(err_msg, CP_UTF8)) ;
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
		TWindow window( hwnd ) ;

		CString window_text( _T("Exception") ) ;

		if ( window.IsWindow() ) 
		{
			window.FlashWindow( FALSE ) ;
			window.GetWindowText( window_text ) ;
		}
		return window_text + TEXT(":\r\n" ) + failure_message;
	}

};

