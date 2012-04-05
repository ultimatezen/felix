/*!
* Implementation file for CException classes
*/

#include "stdafx.h"
#include "Exceptions.h"				// the header file
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include "resource_string.h"
#include "stringconversions.h"

#include <boost/format.hpp>
using boost::format;
using boost::wformat ;
typedef boost::basic_format< TCHAR > tformat;


CString GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
{
	WCHAR* szAppName = L"Felix";
	wstring version = string2wstring(VERSION) ;
	const wchar_t* szVersion = version.c_str();
	const DWORD dwBufferSize = MAX_PATH;
	WCHAR szPath[dwBufferSize] ;
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	GetLocalTime( &stLocalTime );
	GetTempPath( dwBufferSize, szPath );

	wformat filename = wformat(L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp") 
			% szPath 
			% szAppName
			% szVersion
			% stLocalTime.wYear 
			% stLocalTime.wMonth
			% stLocalTime.wDay
			% stLocalTime.wHour
			% stLocalTime.wMinute
			% stLocalTime.wSecond
			% GetCurrentProcessId()
			% GetCurrentThreadId() ;
	hDumpFile = CreateFile(filename.str().c_str(), GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
		hDumpFile, MiniDumpNormal, &ExpParam, NULL, NULL);

	return CString(filename.str().c_str());
}

/*!
* \brief
* Constructor class.
* 
* \param msg_id
* String resource ID
* 
* Uses a message ID for an error message from the string resource (localization-friendly)
*/
except::CException::CException( UINT msg_id )
{
	m_error_message = R2TS(msg_id) ;
}

/*!
* \brief
* Constructor class.
* 
* \param message
* Error message
*/
except::CException::CException( const CString message ) : 
m_error_message( message )
{
}
/*!
* \brief
* Constructor class.
* 
* \param e
* Exception class
*
* Used to process and re-throw an exception.
*/
except::CException::CException( const CException &e ) : 
m_error_message( e.m_error_message ), 
m_bottom_message( e.m_bottom_message ), 
m_description( e.m_description )
{ 
}

/*!
* \brief
* Sets the error message.
*
* Overwrites any error messages accumulated to far.
*
* \see
* CException::add_to_message | CException::what
*/
void except::CException::set_message( const CString message ) 
{ 
	m_error_message = message ; 
}

/*!
* \brief
* Sets the error description.
*
* Overwrites any description accumulated to far.
*
* \see
* CException::add_to_description | CException::description
*/
void except::CException::set_description( const CString description ) 
{ 
	m_description = description ; 
}

/*!
* \brief
* Sets a message to be shown at the bottom of the final error text.
*/
void except::CException::set_bottom_message( const CString bottom_message ) 
{ 
	m_bottom_message = bottom_message ; 
}
/*!
* \brief
* Sets a message to be shown at the bottom of the final error text.
* 
* \param bottom_message_id
* Resource ID of error string.
* 
* Using string resources makes this more localization friendly.
*/
void except::CException::set_bottom_message( UINT bottom_message_id )
{ 
	set_bottom_message( R2TS(bottom_message_id) ) ; 
}

/*!
* \brief
* Adds text to the error message, separated by two MessageBox-friendly newlines (CRLF).
*/
void except::CException::add_to_message( const CString message ) 
{ 
	m_error_message = message + TEXT("\r\n\r\n") + m_error_message ;
}

/*!
* \brief
* Adds text to the description, separated by two MessageBox-friendly newlines (CRLF).
*/
void except::CException::add_to_description( const CString added_description )
{
	set_description( added_description + TEXT("\r\n\r\n") + description() ) ;
}
// feedback

/*!
* \brief
* Notifies the user of an exception, with a top message ID from the string table.
*/
int except::CException::notify_user( UINT top_message_id, UINT flags,const CString title, HWND parent_hwnd )
{
	CString message ;

	if (top_message_id)
	{
		message = R2TS(top_message_id) + TEXT("\r\r");
	}

	return ::MessageBox( parent_hwnd, 
		message + format_message_for_message_box(), 
		title, 
		flags | MB_ICONSTOP | MB_SETFOREGROUND ) ;
}

/*!
* \brief
* Notifies the user of an exception, with a top message string.
*/
int except::CException::notify_user( const CString top_message, UINT flags, const CString title, HWND parent_hwnd )
{
	CString message = top_message + TEXT("\r\r") + format_message_for_message_box() ;
#ifndef UNIT_TEST
	return ::MessageBox( parent_hwnd, message, title, flags | MB_ICONSTOP | MB_SETFOREGROUND ) ;
#else
	parent_hwnd ;
	title ;
	flags ;
	return IDOK ;
#endif
}

/*!
* \brief
* Formats the message for presentation in a mesage box.
*/
CString except::CException::format_message_for_message_box()
{
	return what() +
		_T("\r\n\r\n") +
		description() +
		_T("\r\n\r\n") +
		bottom_message() ;
}


/*!
* \brief
* Gets the error message.
*/
CString except::CException::what() const
{ 
	return m_error_message ; 
}

/*!
* \brief
* Gets the error description.
*/
CString except::CException::description() const 
{ 
	return m_description ; 
}

/*!
* \brief
* A message to add to the bottom of the full error message.
*
* This prevents nesting problems, with different layers adding their own bits
* to the error message, and the decision string getting buried.
*/
CString except::CException::bottom_message() const
{ 
	return m_bottom_message  ; 
}


// protected

/*!
* \brief
* Format a system message (error code).
*/
CString except::CException::format_message( DWORD err )
{
	LPVOID buff(0)  ;
	if ( 0 == 
		::FormatMessage
		(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM ,
		NULL,
		err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &buff,
		0,
		NULL 
		)
		)
	{
		if ( buff != NULL )	::LocalFree( buff ) ;
		return CString( _T("Unknown Error Type") ) ;
	}
	ATLASSERT( buff != NULL ) ;
	CString msg( static_cast< LPCTSTR >( buff ) ) ;
	::LocalFree( buff ) ;

	return msg ;
}

//////////////////////////////////////////////////////////////////////////

/*!
* \brief
* Constructor
* 
* \param msg
* String resource ID
* 
* \param file_name
* The name of the file
* 
* \param line_number
* The line number where the exception was thrown
*/
except::CProgramException::CProgramException( UINT msgid, LPCTSTR file_name, int line_num ) : 
CException( msgid ), 
m_file_name( file_name ), 
m_line_number( line_num )
{
	ATLTRACE( _T("%s(%d): %s\n"), file_name ? file_name : _T("None"), line_num, R2TS(msgid) ) ;
}


except::CProgramException::CProgramException( const CString message, LPCTSTR file_name, int line_num ) : 
CException( message ), 
m_file_name( file_name ), 
m_line_number(line_num)
{

	ATLTRACE( _T("%s(%d): %s\n"), file_name ? file_name : _T("None"), line_num, message ) ;
}

//////////////////////////////////////////////////////////////////////////
/**
*	CComException
*/

HRESULT except::CComException::code( ) 
{ 
	return m_hr ; 
}

except::CComException::CComException( const CComException &e ) : 
m_hr( e.m_hr ), 
CException( e.what() ) 
{
	set_description( e.description() ) ;
	set_bottom_message( e.bottom_message() ) ;
}


except::CComException::CComException(const CString message, const HRESULT hresult, IUnknown *punk, LPCTSTR file_name, int line_num ) : 
CException(message), 
m_hr( hresult ) 
{ 

	set_description( get_reason() ) ;

	// get the error info from punk
	if( punk != NULL )
	{
		info_from_unknown( punk ) ;
	}

	file_name ;
	line_num ;
	ATLTRACE( _T("%s(%d): %s\n"), file_name ? file_name : _T("None"), line_num, message ) ;
}

void except::CComException::info_from_unknown( IUnknown *punk )
{
	CComPtr<ISupportErrorInfo> pSEI;
	HRESULT hr = punk->QueryInterface(IID_ISupportErrorInfo, out_ptr( &pSEI ) );
	punk->Release() ;

	if ( FAILED( hr ) )
	{
		return ;
	}

	IErrorInfoPtr pEO ;
	hr = GetErrorInfo(NULL, &pEO) ;
	ASSERT_HRESULT( hr ) ;

	if ( FAILED( hr ) )
	{
		return ;
	}

	_bstr_t bstrSource ;
	hr = pEO->GetSource(bstrSource.GetAddress());
	ASSERT_HRESULT( hr ) ;

	if ( FAILED( hr ) )
	{
		return ;
	}

	if ( bstrSource.length() > 0 ) 
	{
		add_to_description( _T("Source: " ) + CString( (LPCWSTR)bstrSource ) ) ;
	}

	_bstr_t bstrDesc;
	hr = pEO->GetDescription(bstrDesc.GetAddress());
	ASSERT_HRESULT( hr ) ;

	if ( FAILED( hr ) )
	{
		return ;
	}

	if ( bstrDesc.length() > 0 ) 
	{
		add_to_description( _T("Description: " ) + CString( (LPCWSTR)bstrDesc ) ) ;
	}
}

except::CComException::CComException( const _com_error &e ) : 
CException( _T("COM Exception") ), 
m_hr( e.Error() )
{
	translate_com_error( e ) ;
}

except::CComException::CComException(const CString message, const _com_error &e, LPCTSTR file_name, int line_num ) : 
CException(message), 
m_hr( e.Error() ) 
{ 
	translate_com_error( e ) ;

	file_name ;
	line_num ;
	ATLTRACE( _T("%s(%d): %s\n"), file_name ? file_name : _T("None"), line_num, message ) ;
}

void except::CComException::translate_com_error( const _com_error &e )
{
	// errnum
	CString errnum_str = TEXT("COM error code: ") + errnum_as_string() + TEXT("\r\n") ;

	// error
	CString error = TEXT("Error: ") ;
	error += e.ErrorMessage() ;
	error += _T("\r\n") ;

	// description
	CString desc_text =  CW2CT( e.Description() ) ;
	if (! desc_text.IsEmpty())
	{
		error += CString( TEXT("\r\nDescription: " ) ) + desc_text ;
	}

	// source
	CString source_text =  CW2CT( e.Source() ) ;
	if (! source_text.IsEmpty())
	{
		error += CString( TEXT("\r\nSource: " ) ) + source_text ;
	}

	// set description
	set_description( errnum_str + error) ;
}

CString except::CComException::get_type() 
{ 
	return CString( _T("COM Exception") ) ; 
}


CString except::CComException::get_reason()
{
	if ( SUCCEEDED( m_hr ) )
		return CString( TEXT("This is an informational message. There are no errors logged on the system.") ) ;

	_com_error e(m_hr) ;
	return TEXT("COM error code: ") + errnum_as_string() 
		+ TEXT("\r\nCOM error message: ") + e.ErrorMessage() ;
}

CString except::CComException::errnum_as_string()
{
	return CString((format("0x%08X") % m_hr).str().c_str()) ;
}

CString except::CComException::line_num_as_string( int line_num )
{
	return CString((format("%d") % line_num).str().c_str()) ;
}

except::CSWException::CSWException( const CString message, UINT code, PEXCEPTION_POINTERS pep ) : 
	CException( message ), 
	m_code( code ), 
	m_rec( NULL )
{
	if (pep)
	{
		m_rec = pep->ExceptionRecord ;
		m_filename = GenerateDump(pep) ;
		ATLASSERT( m_code == m_rec->ExceptionCode ) ;
	}
	set_description( get_reason( ) ) ;
}

CString except::CSWException::get_reason()
{
	CString reason( TEXT("Structured Windows Exception.\r\n") ) ;

	// do we have info in the ExceptionInformation array?
	if (m_rec)
	{
		if ( m_rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION )
		{
			reason += add_exception_information() ;
		}
		reason += get_faulting_module() ;
	}

	return reason ;
}

CString except::CSWException::get_faulting_module()
{
	// get the faulting module
	MEMORY_BASIC_INFORMATION mbi;
	if ( !VirtualQuery( m_rec->ExceptionAddress, &mbi, sizeof(mbi) ) )
		return CString() ;
	PVOID hMod = mbi.AllocationBase ;
	TCHAR buff[_MAX_PATH+1] ;
	if ( !::GetModuleFileName( static_cast< HMODULE >( hMod ), buff, _MAX_PATH ) )
		return CString() ;
	return TEXT("\nFaulting module = ") + CString( buff ) ;
}

CString except::CSWException::add_exception_information()
{
	return TEXT("Attempted to ") + 
		// pos 0 = read/write flag
		CString( ( m_rec->ExceptionInformation[0] ? _T("read inaccessible data in address: ") : _T("write to an inaccessible address: ") ) )
		// pos 1 = address
		+ address_as_string( m_rec->ExceptionInformation[1] ) + TEXT("\r\n") ;
}

CString except::CSWException::address_as_string( ULONG_PTR num )
{
	return CString((format("0x%08X") % num).str().c_str()) ;
}

except::CFloatException::CFloatException( LPCTSTR message, UINT code, PEXCEPTION_POINTERS pep ) : 	
	CSWException( message, code, pep )
{
	BANNER( "CFloatException" ) ;
	CString desc = _T("Floating point exception\r") + description() ;
	set_description( desc ) ;
}
