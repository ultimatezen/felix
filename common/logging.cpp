#include "stdafx.h"
#include "logging.h"

// shell API support
#include <shlobj.h>

#pragma warning( disable : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#pragma warning( default : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch

namespace fs = boost::filesystem;
using namespace except ;

logger_ptr m_logger ;
bool initialized = false ;

LOGGING_LEVEL logging_level = LOGGING_ALL ;

void set_logging_level(LOGGING_LEVEL level)
{
	logging_level = level ;
}

void ensure_logger()
{
	if (! m_logger)
	{
		m_logger = logger_ptr(new file_logger) ;
		if (! initialized)
		{
			m_logger->init() ;
			initialized = true ;
		}
	}
}
void dispose_logger()
{
	if (m_logger)
	{
		m_logger = logger_ptr() ;
	}
}

void logging::log_debug( const string msg )
{
	try
	{
		ATLTRACE("INFO - %s\n", msg.c_str()) ;
		if (logging_level > LOGGING_ALL)
		{
			return ;
		}
		ensure_logger() ;
		m_logger->log_debug(msg) ;
		dispose_logger() ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}

void logging::log_warn( const string msg )
{
	try
	{
		ATLTRACE("WARN - %s\n", msg.c_str()) ;
		if (logging_level > LOGGING_WARN)
		{
			return ;
		}
		ensure_logger() ;
		m_logger->log_warn(msg) ;
		dispose_logger() ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}

void logging::log_error( const string msg )
{
	try
	{
		ATLTRACE("ERROR - %s\n", msg.c_str()) ;
		ensure_logger() ;
		m_logger->log_error(msg) ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}

void logging::log_exception( CException &e )
{
	try
	{
		ATLTRACE(_T("EXCEPTION - %ls\n"), e.what()) ;
		ensure_logger() ;
		m_logger->log_exception(e) ;
		dispose_logger() ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}
void logging::log_exception( _com_error &e )
{
	try
	{
		ATLTRACE(_T("EXCEPTION - %ls\n"), e.Description()) ;
		ensure_logger() ;
		m_logger->log_exception(e) ;
		dispose_logger() ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}

void logging::set_logger( logger_ptr logger )
{
	try
	{
		m_logger = logger ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}

void logging::send_report(const CString language, const CString filename)
{
	try
	{
		ensure_logger() ;
		m_logger->send_report(language, filename) ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}


file_logger::file_logger()
{
#ifdef UNIT_TEST
	return ;
#else

	TCHAR szPath[MAX_PATH];
	COM_ENFORCE(SHGetFolderPath(NULL, // hwndOwner
		CSIDL_LOCAL_APPDATA,		  // nFolder
		(HANDLE)NULL,				  // hToken (-1 means "default user")
		SHGFP_TYPE_CURRENT,			  // dwFlags 
		szPath
		), _T("Failed to retrieve local app data folder") ) ; 

	fs::wpath pathname = fs::wpath(szPath) 
		/ _T("Felix") 
		/ _T("logs") 
		/ LOGFILE_NAME ;
	m_logfile_name = CString(pathname.string().c_str()) ;
#endif
}

void file_logger::init()
{
	SENSE("init") ;
#ifdef UNIT_TEST
	return ;
#else

	::DeleteFile(this->m_logfile_name) ;
#endif
}

string file_logger::get_datestring()
{
	misc_wrappers::date log_date ;
	log_date.set_to_local_time() ;
	return string2string(log_date.get_date_time_string(TEXT("yyyy'-'MM'-'dd"))) ;
}

void file_logger::write_entry( const string level, const string msg )
{
	SENSE("write_entry") ;
	SENSE(level) ;
	SENSE(msg) ;
#ifdef UNIT_TEST
	return ;
#else

	file::CFile outfile ;
	outfile.Create(this->m_logfile_name, 
		GENERIC_WRITE, 
		0, 
		OPEN_ALWAYS) ;

	const string outstring = level + "\t" 
		+ this->get_datestring() + "\t" 
		+ msg + "\n" ;

	outfile.Seek(0, FILE_END) ;
	outfile.Write(outstring.c_str(), static_cast< DWORD >( outstring.size() )) ;
#endif
}

void file_logger::log_exception( CException &e )
{
	const wstring msg(e.format_message_for_message_box()) ;
	textstream_reader<wchar_t> reader(msg.c_str()) ;

	wstring err_msg ;
	while (! reader.empty())
	{
		wstring line = boost::trim_copy(reader.getline()) ;
		if (! err_msg.empty())
		{
			err_msg += L"\t\t" ;
		}
		err_msg += line ;
		reader.eat_whitespace() ;
		if (! reader.empty())
		{
			err_msg += L"\n" ;
		}
	}


	this->log_error(string2string(err_msg, CP_UTF8)) ;
}

void file_logger::log_exception( _com_error &e )
{
	const CString description = (LPCWSTR)e.Description() ;
	const CString message = e.ErrorMessage() ;
	CString err_msg ;
	err_msg.Format(_T("COM error 0x%08X: %s (%s)"), 
					e.Error(), 
					static_cast<LPCTSTR>(message), 
					static_cast<LPCTSTR>(description)) ;
	const CStringA msg(err_msg) ;
#ifdef UNIT_TEST
	return ;
#else
	this->log_error(string(static_cast< LPCSTR >( msg ))) ;
#endif
}

void file_logger::send_report( const CString language, const CString filename )
{
#ifdef UNIT_TEST
	return ;
#else
	try
	{
		CDispatchWrapper wrapper(LOGGING_SERVER); 
		CComVariant var1(language) ;
		CComVariant var2(filename) ;
		wrapper.method(L"Report", var1, var2) ;
		wrapper.m_app = NULL ;
	}
	catch (_com_error& err)
	{
		ATLASSERT(FALSE && "Raised exception in file_logger") ;
		CComException ce(err) ;
		ce ;
	}
#endif
}