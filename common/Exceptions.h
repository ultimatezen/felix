/*!
 * Custom exception classes
 * 
 * We need these classes because stupid STL exceptions are ASCII only!
 */

#pragma once

#pragma warning( disable : 4996 ) 

#include "DebugUtilities.h" 
#include <eh.h>		// _se_translator_function -- CException handling header file
#include "ComUtilities.h"


namespace except
{

	// ========================
	// Exception-aware message maps & automation handlers
	// ========================


#define MSG_TRY try { CLEAR_WINERRORS ;


/*!
* \brief
* Use this in the message maps of windows without status bars.
*/
#define MSG_CATCH( failure_message ) }							\
	catch ( except::CSWException &sw_e ) { ATL::CString failure_label( failure_message ) ; return handle_exception( sw_e, failure_label ) ; } \
	catch ( _com_error &e ) { ATL::CString failure_label( failure_message ) ; return handle_exception( e, failure_label ) ; } \
	catch ( except::CComException &e ) { ATL::CString failure_label( failure_message ) ; return handle_exception( e, failure_label ) ; } \
	catch ( except::CWinException &e ) { ATL::CString failure_label( failure_message ) ; return handle_exception( e, failure_label ) ; } \
	catch ( except::CException &e ) { ATL::CString failure_label( failure_message ) ; return handle_exception( e, failure_label ) ; } \
	catch ( std::exception &e ) { ATL::CString failure_label( failure_message ) ; return handle_exception( e, failure_label ) ; }


	/*!
	* \brief
	* Put an error message in the window status bar.
	*/
inline void sb_text( HWND sb_hwnd, const ATL::CString sb_text )
{
	sb_text ;
	CStatusBarCtrl bar ;
	bar.Attach( sb_hwnd ) ;
#ifndef UNIT_TEST
	bar.SetText( 0, sb_text ) ;
#endif
	bar.Detach() ;
}


// 
/*!
* \brief
* Use this in the message maps of windows with status bars.
*/
#define MSG_CATCH_STATUS_BAR( fail_txt )}							\
	catch ( except::CSWException &sw_e ) { ATL::CString fail_msg( fail_txt ) ; sb_text(m_hWndStatusBar, fail_msg ) ; return handle_sw_exception( sw_e, fail_msg ) ; } \
	catch ( _com_error &e ) { ATL::CString fail_msg( fail_txt ) ; sb_text(m_hWndStatusBar, fail_msg ) ; return handle_exception( e, fail_msg ) ; } \
	catch ( except::CComException &e ) { ATL::CString fail_msg( fail_txt ) ; sb_text(m_hWndStatusBar, fail_msg ) ; return handle_exception( e, fail_msg ) ; } \
	catch ( except::CWinException &e ) { ATL::CString fail_msg( fail_txt ) ; sb_text(m_hWndStatusBar, fail_msg ) ; return handle_exception( e, fail_msg ) ; } \
	catch ( except::CException &e ) { ATL::CString fail_msg( fail_txt ) ; sb_text(m_hWndStatusBar, fail_msg ) ; return handle_exception( e, fail_msg ) ; } \
	catch ( std::exception &e ) { ATL::CString fail_msg( fail_txt ) ; sb_text(m_hWndStatusBar, fail_msg ) ; return handle_exception( e, fail_msg ) ; } 


#define TRUE_ENFORCE( condition, message ) \
{ if( ! (condition) ) throw except::CProgramException( (message), _T(__FILE__), __LINE__ ) ; }

// ========================
// Exception Definitions
// ========================

/**
@class CException
@brief Base exception class.
*/
class CException
{
	ATL::CString		m_error_message ;
	ATL::CString		m_description ;
	ATL::CString		m_bottom_message ;

public:
	// set
	void set_message( const ATL::CString message ) ;
	void set_description( const ATL::CString description ) ;
	void set_bottom_message( const ATL::CString bottom_message ) ;
	void set_bottom_message( UINT bottom_message_id ) ;

	// add message to top of message
	void add_to_message( const ATL::CString message ) ;
	// add description to bottom of description
	void add_to_description( const ATL::CString added_description ) ;

	// feedback
	virtual int notify_user
	( 
		UINT top_message_id, 
		UINT flags = MB_OK, 
		const ATL::CString title = _T("ERROR"),
		HWND parent_hwnd = ::GetActiveWindow() 
	) ;
	virtual int notify_user
	( 
		const ATL::CString top_message, 
		UINT flags = MB_OK, 
		const ATL::CString title = _T("ERROR"),
		HWND parent_hwnd = ::GetActiveWindow() 
	) ;
	ATL::CString format_message_for_message_box() ;

	// ---------
	// get
	// ---------
	// top
	ATL::CString what() const ;
	// middle
	ATL::CString description() const ;
	// bottom
	ATL::CString bottom_message() const ;

	/** CTOR.
	*/
	CException( UINT msg_id ) ;
	/** CTOR.
	*/
	CException( const ATL::CString message ) ;
	/** CTOR.
	*/
	CException( const CException &e ) ;

protected:
	/** Wraps FormatMessage API call.
	 */
	ATL::CString format_message( DWORD err ) ;
} ;

class CBadRecordException : public CException
{
public:
	CBadRecordException(UINT msgid) : CException(msgid){}
};

/**
	@class CAllocException
	@brief Represents allocation errors.
 */
class CAllocException : public CException
{
public:
	/** CTOR.
	*/
	CAllocException(size_t bytes_requested=0) : CException( _T("Memory allocation exception") )
	{
		if (bytes_requested > 0)
		{
			ATL::CString msg ;
			msg.Format(_T("Failed to allocate requested %d bytes of memory."), (int)bytes_requested) ;
			this->add_to_message(msg) ;
		}
		BANNER( "CAllocException" ) ;
	}
};

/**
	@class CProgramException 
	@brief Represents a bug.
*/
class CProgramException : public CException
{
	ATL::CString m_file_name ;
	int		m_line_number ;
public:
	/** CTOR.
	*/
	CProgramException( UINT msg_id, LPCTSTR name_of_file = NULL, int line_number = 0 ) ;
	/** CTOR.
	*/
	CProgramException( const ATL::CString msg, LPCTSTR name_of_file = NULL, int line_number = 0 ) ;
};

/**
	@class CComException 
	@brief Represents a COM error.
 */
class CComException : public CException
{
	HRESULT		m_hr ;

public:
	HRESULT code( ) ;
	/** CTOR.
	*/
	CComException( const CComException &e ) ;

	/** CTOR.
	*/
	CComException(const ATL::CString message, 
		const HRESULT hresult = E_FAIL, 
		IUnknown *punk = NULL, 
		LPCTSTR file_name = NULL, 
		int line_num = 0 ) ;
	void info_from_unknown( IUnknown *punk ) ;
	/** CTOR.
	*/
	CComException( const _com_error &e ) ;
	/** CTOR.
	*/
	CComException(const ATL::CString message, const _com_error &e, LPCTSTR file_name = NULL, int line_num = 0 ) ;

	void translate_com_error( const _com_error &e ) ;
	virtual ATL::CString get_type() ;

	ATL::CString get_reason() ;
	ATL::CString errnum_as_string() ;
	ATL::CString line_num_as_string( int line_num ) ;

} ;

/**
	@class CWinException 
	@brief Wraps Win32 error.
*/
class CWinException : public CProgramException
{
	DWORD	m_err ;
public:
	// get
	DWORD code() { return m_err ; }

	/** CTOR.
	*/
	CWinException( const ATL::CString message, const DWORD err = GetLastError() )
		: CProgramException( message ), m_err( err )
	{ 
		add_to_description( get_reason() ) ;
	}
	virtual ~CWinException( )
	{
		::SetLastError( (DWORD) 0 ) ; // we handled this error, so clear it from the system
	}
	virtual ATL::CString get_type() { return ATL::CString( _T("Windows Exception") ) ; }
private:
	// private error code lookup
	ATL::CString get_reason()
	{
		return 
			TEXT("Windows error code: ") + errnum_as_string() 
			+ TEXT("\nWindows error message: ") + format_message( m_err ) ;
	}
	ATL::CString errnum_as_string()
	{
		ATL::CString err ;
		err.Format( _T("%d"), (int)m_err ) ;
		return err ;
	}
} ;

/**
	@class CFileException 
	@brief File exception.
*/
class CFileException : public CWinException
{
public:
	/** CTOR.
	*/
	CFileException( const ATL::CString message, const DWORD err = GetLastError() )
	: CWinException( message, err ) 
	{
		BANNER( "CFileException" ) ;
	}
		
	ATL::CString get_type() { return ATL::CString( _T("File Exception") ) ; }
};

/**
	@class CSWException 
	@brief Wraps a structured Windows exception.
*/
class CSWException : public CException
{
	PEXCEPTION_RECORD	m_rec ;
	UINT				m_code ;
	ATL::CString				m_filename ;
public:
	/** CTOR.
	*/
	CSWException( const ATL::CString message, UINT code, PEXCEPTION_POINTERS pep );

	UINT get_code() { return m_code ; }
	ATL::CString get_filename() { return m_filename ; }

	ATL::CString get_reason( );
	ATL::CString get_faulting_module();
	ATL::CString add_exception_information();
	ATL::CString address_as_string( ULONG_PTR num );
} ;

/**
	@class CFloatException 
	@brief Wraps a float exception.
*/
class CFloatException : public CSWException
{
public:
	/** CTOR.
	 */
	CFloatException( LPCTSTR message, UINT code, PEXCEPTION_POINTERS pep );
};

}
inline void THROW_WIN_EXCEPTION( const ATL::CString message ) 
{ 
	throw except::CWinException( message, GetLastError() ) ; 
}
inline void THROW_FILE_EXCEPTION( const ATL::CString message ) 
{ 
	throw except::CFileException( message, GetLastError() ) ; 
}
inline void THROW_COM_EXCEPTION( const HRESULT hr, const ATL::CString message ) 
{ 
	throw except::CComException( message, hr, NULL ) ; 
}
inline void THROW_PROGRAM_EXCEPTION( const ATL::CString message ) 
{ 
	throw except::CProgramException( message ) ; 
}

