/*!
	@file automationexceptionhandler.h
	@brief interface for the CAutomationExceptionHandler class.
	@date 2005/06/23
	Time: 23:03:14
	@author Ryan Ginstrom
	@dir c:\programming\programming projects\Felix 1.5
 */

#pragma once

#include "Exceptions.h"
#include "ComUtilities.h"
#include "logging.h"


template <class T, const CLSID* pclsid = &CLSID_NULL>
class CAutomationExceptionHandler : public CComCoClass< T, pclsid >
{
protected:
	CString m_error_description ;

public:
	HRESULT handle_exception(std::exception &e, const CString &msg )
	{
		CString message = call_to_error_line( msg ) ;						
		message += TEXT("C Runtime Error\r\n") ;				
		message += TEXT("Description: ") ;
		message += CA2CT( e.what() ) ;	

		logging::log_error((LPCSTR)CStringA(message)) ;

		return description_from_exception( message, E_FAIL ) ;
	}
	HRESULT handle_exception(CComException &com_exception, const CString &msg )
	{
		logging::log_error("COM exception on Automation object") ;
		logging::log_exception(com_exception) ;
		CString message = call_to_error_line( msg ) ;						
		message += com_exception.what() ;						
		message += _T("\r\n" ) ;								
		message += com_exception.description() ;

		logging::log_error((LPCSTR)CStringA(message)) ;

		return description_from_exception( message, com_exception.code() ) ;
	}
	HRESULT handle_exception(CWinException &e, const CString &msg )
	{
		logging::log_error("COM exception on Automation object") ;
		CString message = call_to_error_line( msg ) ;						
		message += e.what() ;						
		message += _T("\r\n" ) ;								
		message += e.description() ;		

		logging::log_error((LPCSTR)CStringA(message)) ;

		return description_from_exception( message, HRESULT_FROM_WIN32( e.code() ) ) ;
	}
	HRESULT handle_exception(CSWException &e, const CString &msg )
	{
		logging::log_error("COM exception on Automation object") ;
		CString message = call_to_error_line( msg ) ;						
		message += e.what() ;						
		message += _T("\r\n" ) ;								
		message += e.description() ;	

		logging::log_error((LPCSTR)CStringA(message)) ;

		return description_from_exception( message, E_FAIL ) ;
	}
	HRESULT handle_exception(CException &e, const CString &msg )
	{
		logging::log_error("COM exception on Automation object") ;
		::MessageBeep( MB_ICONHAND ) ;							
		CString message = call_to_error_line( msg ) ;						
		message += e.what() ;									
		message += _T("\r\n" ) ;								
		message += e.description() ;	

		logging::log_error((LPCSTR)CStringA(message)) ;

		return description_from_exception( message, E_FAIL ) ;
	}
	HRESULT handle_exception(_com_error &e, const CString &msg )
	{
		CComException com_exception(msg, e) ;					
		CString message = call_to_error_line( msg ) ;						
		message += com_exception.what() ;						
		message += _T("\r\n" ) ;								
		message += com_exception.description() ;

		logging::log_error((LPCSTR)CStringA(message)) ;

		return description_from_exception( message, com_exception.code() ) ;
		
	}

	// =================


	void handle_exception_void(std::exception &e, const CString &msg )
	{
		CString message = call_to_error_line( msg ) ;						
		message += TEXT("C Runtime Error\r\n") ;				
		message += TEXT("Description: ") ;
		message += CA2CT( e.what() ) ;	

		logging::log_error((LPCSTR)CStringA(message)) ;

		MessageBox( NULL, message, _T("C Runtime Error"), MB_OK | MB_ICONHAND ) ;
		m_error_description = message ;		
		
	}
	void handle_exception_void(CComException &com_exception, const CString &msg )
	{
		CString message = call_to_error_line( msg ) ;						
		message += com_exception.what() ;						
		message += _T("\r\n" ) ;								
		message += com_exception.description() ;				
		m_error_description = message ;		

		logging::log_error((LPCSTR)CStringA(message)) ;


		com_exception.notify_user( _T("COM Error" ) ) ;
	}
	void handle_exception_void(CWinException &e, const CString &msg )
	{
		CString message = call_to_error_line( msg ) ;						
		message += e.what() ;						
		message += _T("\r\n" ) ;								
		message += e.description() ;				
		m_error_description = message ;		

		logging::log_error((LPCSTR)CStringA(message)) ;

		e.notify_user( _T("Automation Error" ) ) ;
	}
	void handle_exception_void(CSWException &e, const CString &msg )
	{
		CString message = call_to_error_line( msg ) ;						
		message += e.what() ;						
		message += _T("\r\n" ) ;								
		message += e.description() ;				
		m_error_description = message ;		

		logging::log_error((LPCSTR)CStringA(message)) ;

		e.notify_user( _T("Automation Error" ) ) ;
	}
	void handle_exception_void(CException &e, const CString &msg )
	{
		::MessageBeep( MB_ICONHAND ) ;							
		CString message = call_to_error_line( msg ) ;						
		e.notify_user( message ) ;								
		message += e.what() ;									
		message += _T("\r\n" ) ;								
		message += e.description() ;							
		m_error_description = message ;		

		logging::log_error((LPCSTR)CStringA(message)) ;

		e.notify_user( _T("Automation Error" ) ) ;
	}
	void handle_exception_void(_com_error &e, const CString &msg )
	{
		CComException com_exception(msg, e) ;					
		CString message = call_to_error_line( msg ) ;						
		message += com_exception.what() ;						
		message += _T("\r\n" ) ;								
		message += com_exception.description() ;				
		m_error_description = message ;		

		logging::log_error((LPCSTR)CStringA(message)) ;

		com_exception.notify_user( _T("Automation Error" ) ) ;
	}
	// Function name	: CApplication::description_from_exception
	// Description	    : 
	// Return type		: HRESULT 
	// Argument         : CException &e
	// Argument         : HRESULT code
	HRESULT description_from_exception( const CString &message, HRESULT code = E_FAIL )
	{
		BANNER( "CApplication::description_from_exception" ) ;
		
		m_error_description = message ;
		return Error( m_error_description, LIBID_Felix, code ) ; \
	}

private:
	CString call_to_error_line( const CString &func_name )
	{
		CString error_line( _T("Call to " ) ) ;
		error_line += func_name ;										
		error_line += _T(" on automation server failed.\r\n") ;	
		return error_line ;
	}
};
