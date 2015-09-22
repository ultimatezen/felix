/*!
	@file AutomationExceptionHandler.h
	@brief interface for the CAutomationExceptionHandler class.
	@date 2005/06/25
	Time: 15:10:41
	@author Ryan Ginstrom
 */

#pragma once

#include "Exceptions.h"
#include "logging.h"

#define CATCH_C_EXCEPTIONS_VOID( msg ) catch ( std::exception &e ) { CString _msg( msg ) ; handle_exception_void( e, _msg, NULL, 0 ) ; return ; }
#define CATCH_COM_EXCEPTIONS_VOID( msg ) catch ( CComException &e ) { CString _msg( msg ) ; handle_exception_void( e, _msg ) ; return ; }
#define CATCH_WIN_EXCEPTIONS_VOID( msg ) catch ( CWinException &e ) { CString _msg( msg ) ; handle_exception_void( e, _msg ) ; return ; }
#define CATCH_SW_EXCEPTIONS_VOID( msg ) catch ( CSWException &e ) { CString _msg( msg ) ; handle_exception_void( e, _msg, NULL, 0 ) ; return ; }
#define CATCH_EXCEPTIONS_VOID( msg ) catch ( CException &e ) { CString _msg( msg ) ; handle_exception_void( e, _msg ) ; return ; }
#define CATCH_COM_ERRORS_VOID( msg ) catch (_com_error &e ) { CString _msg( msg ) ; handle_exception_void( e, _msg, NULL, 0 ) ; return ; }

#define CATCH_ALL_VOID( msg ) }  CATCH_C_EXCEPTIONS_VOID( msg ) CATCH_COM_EXCEPTIONS_VOID( msg ) CATCH_WIN_EXCEPTIONS_VOID( msg ) CATCH_SW_EXCEPTIONS_VOID( msg ) CATCH_EXCEPTIONS_VOID( msg ) CATCH_COM_ERRORS_VOID( msg )

/**
	@class CAutomationExceptionHandler 
	@brief Handles exceptions thrown by COM objects.
 */
template <class T, const CLSID* pclsid = &CLSID_NULL>
class CAutomationExceptionHandler : public CComCoClass< T, pclsid >
{

protected:
	CString m_error_description ;

public:
	virtual HRESULT handle_exception(std::exception &e, const CString &msg )
	{
		CString message = server_error_line( msg ) ;
		message << TEXT("C Runtime Error\r\n") ;				
		message << TEXT("Description: ") << CA2CT( e.what() ) ;	
		MessageBox( NULL, message.as_cstring(), _T("C Runtime Error"), MB_OK | MB_ICONHAND ) ;

		logging::log_error("Runtime error") ;
		logging::log_error(e.what()) ;
		logging::log_error(CT2A(msg)) ;
		return description_from_exception( message, E_FAIL ) ;
	}
	virtual HRESULT handle_exception(CWinException &e, const CString &msg )
	{
		CString message = server_error_line( msg ) ;
		message << e.what() ;						
		message << _T("\r\n" ) ;								
		message << e.description() ;		
		
		e.notify_user( CString( _T("Windows Error on COM Server") ) ) ;
		logging::log_error("Windows Error") ;
		logging::log_error(CT2A(msg)) ;
		logging::log_exception(e) ;

		return description_from_exception( message, HRESULT_FROM_WIN32( e.code() ) ) ;
	}
	virtual HRESULT handle_exception(CSWException &e, const CString &msg )
	{
		CString message = server_error_line( msg ) ;
		message << e.what() ;						
		message << _T("\r\n" ) ;								
		message << e.description() ;		

		e.notify_user( CString( _T("Structured Windows Exception") ) ) ;
		logging::log_error("Structured Windows Exception") ;
		logging::log_error(CT2A(msg)) ;
		logging::log_exception(e) ;

		return description_from_exception( message, HRESULT_FROM_WIN32( e.get_code() ) ) ;
	}
	virtual HRESULT handle_exception(CComException &com_exception, const CString &msg )
	{
		CString message = server_error_line( msg ) ;
		message << com_exception.what() ;						
		message << _T("\r\n" ) ;								
		message << com_exception.description() ;
		
		com_exception.notify_user( CString( _T("Internal COM Automation Error") ) ) ;
		logging::log_error("Internal COM Automation Error") ;
		logging::log_error(CT2A(msg)) ;
		logging::log_exception(com_exception) ;
		
		return description_from_exception( message, com_exception.code() ) ;
	}
	virtual HRESULT handle_exception(CException &e, const CString &msg )
	{
		CString message = server_error_line( msg ) ;
		message << e.what() ;									
		message << _T("\r\n" ) ;								
		message << e.description() ;	
		e.notify_user( message.as_cstring() ) ;								
		logging::log_error(CT2A(msg)) ;
		logging::log_exception(e) ;

		return description_from_exception( message, E_FAIL ) ;
	}
	virtual HRESULT handle_exception(_com_error &e, const CString &msg )
	{
		CComException com_exception(msg.as_cstring(), e) ;					
		CString message = server_error_line( msg ) ;
		message << com_exception.what() ;						
		message << _T("\r\n" ) ;								
		message << com_exception.description() ;
		
		com_exception.notify_user( CString( _T("External COM Automation Error") )  ) ;
		logging::log_error("External COM Automation Error") ;
		logging::log_exception(com_exception) ;

		return description_from_exception( message, com_exception.code() ) ;
		
	}

	// =================


	void handle_exception_void(std::exception &e, const CString &msg, LPCTSTR file_name, int line_num )
	{
		CString message = server_error_line( msg ) ;
		message << TEXT("C Runtime Error\r\n") ;	
		message << TEXT("Description: ") << e.what() ;	
		message << TEXT("\r\nFile: ") << file_name ;
		message << TEXT("\r\nLine Num: ") << int2wstring( line_num ) ;

		logging::log_error("Runtime error") ;
		logging::log_error(e.what()) ;
		logging::log_error(CT2A(msg)) ;
		logging::log_error(CT2A(file_name)) ;
		logging::log_error(lexical_cast<string>(line_num)) ;

		MessageBox( NULL, message.as_cstring(), _T("C Runtime Error"), MB_OK | MB_ICONHAND | MB_SETFOREGROUND ) ;
		m_error_description = message ;		
		
	}
	void handle_exception_void(CComException &com_exception, const CString &msg, LPCTSTR file_name, int line_num )
	{
		com_exception.notify_user(server_error_line( msg )) ;
		logging::log_error("Internal COM Automation Error") ;
		logging::log_error(CT2A(server_error_line( msg ))) ;
		logging::log_exception(com_exception) ;
		logging::log_error(CT2A(file_name)) ;
		logging::log_error(lexical_cast<string>(line_num)) ;

		message << com_exception.what() ;						
		message << _T("\r\n" ) ;								
		message << com_exception.description() ;				
		
		m_error_description = message ;		
	}
	void handle_exception_void(CWinException &e, const CString &msg, LPCTSTR file_name, int line_num )
	{
		e.notify_user(server_error_line( msg )) ;
		logging::log_error("Windows Error") ;
		logging::log_error(CT2A(server_error_line( msg ))) ;
		logging::log_exception(e) ;
		logging::log_error(CT2A(file_name)) ;
		logging::log_error(lexical_cast<string>(line_num)) ;

		message << e.what() ;						
		message << _T("\r\n" ) ;								
		message << e.description() ;				
		
		m_error_description = message ;		
		
	}
	void handle_exception_void(CSWException &e, const CString &msg, LPCTSTR file_name, int line_num )
	{
		e.notify_user( server_error_line( msg ) ) ;								
		logging::log_error(CT2A(msg)) ;
		logging::log_error(CT2A(server_error_line( msg ))) ;
		logging::log_exception(e) ;
		logging::log_error(CT2A(file_name)) ;
		logging::log_error(lexical_cast<string>(line_num)) ;
		
		message << e.what() ;						
		message << _T("\r\n" ) ;								
		message << e.description() ;				

		message << TEXT("\r\nFile: ") << file_name ;
		message << TEXT("\r\nLine Num: ") << int2wstring( line_num ) ;

		m_error_description = message ;		
	}
	void handle_exception_void(CException &e, const CString &msg )
	{
		e.notify_user(server_error_line( msg )) ;								

		message << e.what() ;									
		message << _T("\r\n" ) ;								
		message << e.description() ;							
		logging::log_error(CT2A(msg)) ;
		logging::log_error(CT2A(server_error_line( msg ))) ;
		logging::log_exception(e) ;

		m_error_description = message ;		
	}
	void handle_exception_void(_com_error &e, const CString &msg, LPCTSTR file_name, int line_num )
	{
		CComException com_exception(msg, e) ;					
		CString message = server_error_line( msg ) ;
		message << com_exception.what() ;						
		message << _T("\r\n" ) ;								
		message << com_exception.description() ;				
		message << TEXT("\r\nFile: ") << file_name ;
		message << TEXT("\r\nLine Num: ") << int2wstring( line_num ) ;

		com_exception.notify_user( CString( _T("External COM Automation Error") )  ) ;
		logging::log_error("External COM Automation Error") ;
		logging::log_error(CT2A(server_error_line( msg ))) ;
		logging::log_exception(com_exception) ;
		logging::log_error(CT2A(file_name)) ;
		logging::log_error(lexical_cast<string>(line_num)) ;

		m_error_description = message ;		
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
			
		return code ;
	}
private:
	CString server_error_line( const CString &msg )
	{
		CString error_line( _T("Call to ") ) ;
		error_line << msg ;										
		error_line << _T(" on automation server failed.\r\n") ;	
		return error_line ;
	}

};

