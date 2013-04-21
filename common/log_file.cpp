// log_file.cpp: implementation of the log_file class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "log_file.h"

#include "MiniVersion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

log_file::log_file( LPCTSTR file_name )
{
	file::CPath path ;
	path.GetModulePath( _Module.GetModuleInstance() ) ;
	path.Append( file_name ) ;

	m_file_name = path.Path() ;
}

log_file::~log_file()
{

}

void log_file::init()
{
	BANNER( "Initializing log file" ) ;
	try
	{
		
		file::file lfile ;
		
		if ( lfile.exists( m_file_name ) )
		{
			lfile.open_existing( m_file_name ) ;
			ATLASSERT( lfile.is_open() ) ;
			lfile.write_eof() ;
			lfile.close() ;
			ATLASSERT( file::file::size( m_file_name ) == 0 ) ;
		}
	}
	catch( CFileException &e )
	{
		e ;
		ATLASSERT( FALSE && "Failed to write to log file" ) ;

		return ;
	}
}

void log_file::write_line(LPCSTR line)
{
	if ( line != NULL )
	{
		write_text( line ) ;
	}
	write_text( "\r\n" ) ;
}

void log_file::write_line(LPCWSTR line)
{
	if ( line != NULL )
	{
		write_text( line ) ;
	}
	write_text( "\r\n" ) ;
}

void log_file::write_text(LPCSTR text)
{
	try
	{
		ATLASSERT( text != NULL ) ;
		file::file lfile ;
		
		lfile.open( m_file_name ) ;
		
		ATLASSERT( lfile.is_open() ) ;
		if ( ! lfile.is_open( ) )
			return ;
		
		lfile.end() ;
		
		DWORD len_written = 0 ;
		len_written = lfile.write( text ) ;
		ATLASSERT( len_written == strlen( text ) ) ;
		
		lfile.write_eof() ;
		lfile.close() ;
	}
	catch( CFileException &e )
	{
		e ;
		ATLTRACE( e.format_message_for_output() ) ;
		ATLASSERT( FALSE && "Failed to write to log file" ) ;
		return ;
	}
}

void log_file::write_text(LPCWSTR text)
{
	USES_CONVERSION ;
	_acp = CP_UTF8 ;
	
	ATLASSERT( text != NULL ) ;
	write_text( W2CA( text ) ) ;
}

void log_file::log_exception(_com_error &ce)
{
	char num_buff[MAX_NUM_STRLEN] = {0};
	LPSTR dest_end(NULL) ;
	DWORD flags = STRSAFE_IGNORE_NULLS | STRSAFE_FILL_ON_FAILURE ;
	HRESULT str_cpy_result(S_OK) ;
	str_cpy_result = StringCbPrintfEx(num_buff, MAX_NUM_STRLEN, &dest_end, NULL, flags, _T("%08X"), ce.Error() ) ;
	ASSERT_HRESULT( str_cpy_result ) ;

	write_text( "*******************\r\n*** COM Error Detected ***\r\n\r\n" ) ;
	write_text( "COM error code: " ) ;
		write_text( num_buff ) ;
		write_text( "\r\n" ) ;
	write_text( "Error: " )  ;
		write_text( ce.ErrorMessage() )  ;
		write_text( "\r\n" ) ;
	write_text( "Description: " )  ;
		write_text( (LPCWSTR)ce.Description() )  ;
		write_text( "\r\n" ) ;
	write_text( "Source: " ) ;
		write_text( (LPCWSTR)ce.Source() ) ;
		write_text( "\r\n" );
	write_timestamp( ) ;
	write_text( "*******************\r\n" ) ;
}

void log_file::log_exception(CException &e)
{
	USES_CONVERSION ;
	_acp = CP_UTF8 ;

	try
	{
		write_text( "*******************\r\n*** Exception Thrown ***\r\n\r\n" ) ;
		write_text( "System-generated exception\r\n" ) ;
		write_text( "Error: " ) ;
		write_text(  T2CA( e.what() ) ) ;
		write_text(  "\r\n" ) ;
		write_text( "Description: " ) ;
		write_text(  T2CA( e.description() )  ) ;
		write_text(  "\r\n" );
	}
	catch( CFileException &e )
	{
		e ;
		ATLTRACE( e.format_message_for_output() ) ;
		ATLASSERT( FALSE && "Failed to write to log file" ) ;

		return ;
	}
}

void log_file::write_timestamp( LPCSTR title )
{
	write_text( title ) ;
	misc_wrappers::date run_date ;
	run_date.set_to_local_time() ;
	write_text( run_date.get_date_time_string( _T("dd'-'MMM'-'yyyy"), _T("hh':'mm':'ss tt"), _T(" ") ).c_str() ) ;
	write_text( "\r\n" ) ;
}

void log_file::write_version_info()
{
	const size_t buffer_len = _MAX_PATH*2 ;
	TCHAR szModName[buffer_len] = {0} ;
	
	::GetModuleFileName( _Module.GetModuleInstance(), szModName, sizeof(szModName)-2) ;

	*this << "Module: " << szModName << "\r\n" ;
	write_line( "Version Information:" ) ;
		
	CMiniVersion ver(szModName);
	
	char szBuf[buffer_len] = {0} ;
		
	ver.GetCompanyName(szBuf, sizeof(szBuf)-1);
	*this << "   Company:    " << szBuf << "\r\n" ;
	
	ZeroMemory( szBuf, sizeof( szBuf ) ) ;
	ver.GetProductName(szBuf, sizeof(szBuf)-1);
	*this << "   Product:    " << szBuf << "\r\n" ;
	
	ZeroMemory( szBuf, sizeof( szBuf ) ) ;
	ver.GetFileDescription(szBuf, sizeof(szBuf)-1);
	*this << "   FileDesc:   " << szBuf << "\r\n" ;
	
	WORD dwBuf[4] = {0} ;
	ver.GetFileVersion(dwBuf);

	ZeroMemory( szBuf, sizeof( szBuf ) ) ;
	LPSTR dest_end(NULL) ;
	DWORD flags = STRSAFE_IGNORE_NULLS | STRSAFE_FILL_ON_FAILURE ;
	
	HRESULT str_cpy_result = StringCbPrintfExA(szBuf, buffer_len, &dest_end, NULL, flags, "   FileVer:    %d.%d.%d.%d\r\n", dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]  ) ;
	ASSERT_HRESULT( str_cpy_result ) ;
	if(SUCCEEDED(str_cpy_result))
	{
		*this << szBuf ;
	}
	
	ver.GetProductVersion(dwBuf);
	ZeroMemory( szBuf, sizeof( szBuf ) ) ;
	str_cpy_result = StringCbPrintfExA(szBuf, buffer_len, &dest_end, NULL, flags, "   ProdVer:    %d.%d.%d.%d\r\n\r\n", dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3] ) ;
	ASSERT_HRESULT( str_cpy_result ) ;
	if(SUCCEEDED(str_cpy_result))
	{
		*this << szBuf ;
	}

	ver.Release();
}
