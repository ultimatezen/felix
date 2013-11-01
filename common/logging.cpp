#include "stdafx.h"
#include "logging.h"
#include "comdef.h"
#include "FileOpHandler.h"
#include "dispatchwrapper.h"
#include "Path.h"
#include "stringconversions.h"
#include <fstream>

#pragma warning( disable : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#pragma warning( default : 4245 ) // 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', signed/unsigned mismatch

namespace fs = boost::filesystem;
using namespace except ;
using std::string;
using std::wstring;

logger_ptr m_logger;
LOGGING_LEVEL logging_level = LOGGING_DEBUG ;

wstring get_logfile_name() 
{
	CString basepath = fileops::get_local_appdata_folder() ;

	file::CPath pathname(basepath) ;
	pathname.Append(CString(_T("Felix"))) ;
	pathname.Append(CString(_T("logs"))) ;
	pathname.Append(CString(LOGFILE_NAME)) ;
	return static_cast<LPCWSTR>(pathname.Path());
}

void logging::write_entry( LOGGING_LEVEL level, string level_str, string msg )
{
	if (level < logging_level)
	{
		return ;
	}
	try
	{
		ATLTRACE("%s - %s\n", level_str.c_str(), msg.c_str()) ;
		if (! m_logger)
		{
			ATLASSERT(FALSE && "Logger not initialized!") ;
			return ;
		}
		m_logger->write_entry(level_str, msg) ;
	}
	catch (...)
	{
		ATLASSERT(FALSE && "Logging failed") ;
	}
}


void logging::set_logging_level(LOGGING_LEVEL level)
{
	logging_level = level ;
}

void logging::log_verbose( const string msg )
{
	logging::write_entry(LOGGING_VERBOSE, "INFO", msg); 
}

void logging::log_debug( const string msg )
{
	logging::write_entry(LOGGING_DEBUG, "INFO", msg); 
}

void logging::log_warn( const string msg )
{
	logging::write_entry(LOGGING_WARN, "WARN", msg); 
}

void logging::log_error( const string msg )
{
	logging::write_entry(LOGGING_ERROR, "ERROR", msg); 
}

void logging::log_exception( CException &e )
{
	const wstring msg(e.format_message_for_message_box()) ;
	const wstring err_msg = logging::format_exception_msg(msg);
	logging::log_error(string2string(err_msg, CP_UTF8)) ;
}
void logging::log_exception( std::exception &e )
{
	const wstring msg(string2wstring(e.what())) ;
	const wstring err_msg = logging::format_exception_msg(msg);

	logging::log_error(string2string(err_msg, CP_UTF8)) ;
}

wstring logging::format_exception_msg( const wstring &msg )
{
	textstream_reader<wchar_t> reader(msg.c_str()) ;

	wstring err_msg ;
	while (! reader.empty())
	{
		if (! err_msg.empty())
		{
			err_msg += L"\t\t" ;
		}
		err_msg += boost::trim_copy(reader.getline()) ;
		reader.eat_whitespace() ;
		if (! reader.empty())
		{
			err_msg += L"\n" ;
		}
	}
	return err_msg;
}

void logging::log_exception( _com_error &e )
{
	const CString description = (LPCWSTR)e.Description() ;
	const CString message = e.ErrorMessage() ;
	CString err_msg ;
	err_msg.Format(_T("COM error 0x%08X: %s (%s)"), 
		e.Error(), 
		static_cast<LPCTSTR>(message), 
		static_cast<LPCTSTR>(description)) ;

	logging::log_error(string2string(static_cast< LPCWSTR >( err_msg ), CP_UTF8)) ;
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

string logging::get_datestring()
{
	misc_wrappers::date log_date ;
	log_date.set_to_local_time() ;
	return string2string(log_date.get_date_time_string(TEXT("yyyy'-'MM'-'dd"))) ;
}

logger_ptr logging::get_logger()
{
	return m_logger;
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

void logging::ensure_logger()
{
	if (! m_logger)
	{
		m_logger = logger_ptr(new file_logger) ;
	}
}

// file_logger

file_logger::file_logger() : m_logfile_name(get_logfile_name()), 
	out(get_logfile_name())
{
}

void file_logger::write_entry(const string level, const string msg)
{
	if (! out.is_open())
	{
		ATLTRACE("Failed to open logging file!\n") ;
		return ;
	}

	out << level << "\t" << logging::get_datestring() << "\t" << msg << "\n" ;
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
		logging::log_error("Failed to send error report") ;
		logging::log_exception(ce) ;
	}
#endif
}