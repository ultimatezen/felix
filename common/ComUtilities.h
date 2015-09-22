/*!
	@file ComUtilities.h
	@brief COM utilities (macros and such)
	@date 2005/06/25
	Time: 15:12:15
	@author Ryan Ginstrom
 */

#pragma once

#include "DebugUtilities.h"

#define CHECK_OUT_PTR( ptr ) \
	if ( ptr == NULL )\
	{\
	ATLTRACE( "%s(%d): Out Pointer [" #ptr "] is NULL\n" __FILE__, __LINE__ ) ;	 \
	m_error_description = _T( "Internal Error: Attempted to pass NULL pointer (" ) _T(#ptr) _T( ")" ) ; \
	return Error( m_error_description, LIBID_Felix, E_POINTER ) ;\
	}

#define CHECK_RES( res ) \
	if ( res != ERROR_SUCCESS ) \
	{ \
	TRACE_WINERR( res ) ;  \
	m_error_description = _T("Registry operation failed" ) ;  \
	return Error( m_error_description, LIBID_Felix, HRESULT_FROM_WIN32( res ) ) ; \
	}



// To use this macro, derive from CAutomationExceptionHandler
#define AUTO_CATCH_VOID( Func )					\
	catch( _com_error &e )						\
	{											\
		handle_exception_void( e, Func ) ;		\
	}											\
	catch( CWinException &e )					\
	{											\
		handle_exception_void( e, Func ) ;		\
	}											\
	catch( CComException &e )					\
	{											\
		handle_exception_void( e, Func ) ;		\
	}											\
	catch( CSWException &e )					\
	{											\
		handle_exception_void( e, Func ) ;		\
	}											\
	catch( CException &e )						\
	{											\
		handle_exception_void( e, Func ) ;		\
	}											\
	catch ( std::exception &e )					\
	{											\
		handle_exception_void( e, Func ) ;		\
	}

// To use this macro, derive from CAutomationExceptionHandler
#define AUTO_CATCH( func_str ) \
	catch( _com_error &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }  \
	catch( except::CWinException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( except::CComException &e )   { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( except::CSWException &e )    { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch( except::CException &e )      { CString Func( func_str ) ; return handle_exception( e, Func ) ; }	\
	catch ( std::exception &e ) { CString Func( func_str ) ; return handle_exception( e, Func ) ; }

#ifdef _DEBUG

	// Throws CComException
	#define AUTO_ENFORCE( hr, message, punk ) \
	{ \
		HRESULT _hr = (hr) ; \
		if( FAILED( _hr ) ) \
		{ \
			TRACE_HRESULT( _hr ) ; \
			CString error_message(message) ; \
			CComException e( error_message, _hr, punk, _T(__FILE__), __LINE__  ) ; \
			if( punk ) punk.Release() ; \
			throw except::CComException( e ) ; \
		} \
	}

	#define COM_ENFORCE( hr, message ) \
	{ \
		HRESULT _hr = (hr) ; \
		if( FAILED( _hr ) ) \
		{ \
			TRACE_HRESULT( _hr ) ; \
			CString error_message(message) ; \
			throw except::CComException( error_message, _hr, NULL, _T(__FILE__), __LINE__  ) ; \
		} \
	}

#else

	// Throws CComException
	#define AUTO_ENFORCE( hr, message, punk ) \
	{ \
		HRESULT _hr = (hr) ; \
		if( FAILED( _hr ) ) \
		{ \
		CString error_message(message) ; \
		CComException e( error_message, _hr, punk, NULL, 0  ) ; \
		if( punk ) punk.Release() ; \
		throw except::CComException( e ) ; \
		} \
	}

	#define COM_ENFORCE( hr, message ) \
	{ \
		HRESULT _hr = (hr) ; \
		if( FAILED( _hr ) ) \
		{ \
		CString error_message(message) ; \
		throw except::CComException( error_message, _hr, NULL, NULL, 0  ) ; \
		} \
	}

#endif // #ifdef _DEBUG


// Safe cast of a pointer to type void**
template< typename I >
inline void ** out_ptr( I **ppi )
{
	ATLASSERT( ppi != NULL ) ;
	return reinterpret_cast< void ** >(ppi) ;
}

