#pragma once

#include "DispatchWrapper.h"
#include "file.h"
#include "miscwrappers.h"
#include "textstream_reader.h"
#include <fstream> 

typedef enum
{
	LOGGING_VERBOSE,
	LOGGING_DEBUG,
	LOGGING_WARN,
	LOGGING_ERROR
} LOGGING_LEVEL ;

#define LOG_LEVEL LOGGING_VERBOSE

class _com_error ;

class logger
{
public:
	virtual void write_entry(const string level, const string msg) = 0 ;
	virtual void send_report(const CString language, const CString filename) = 0 ;
};

typedef boost::shared_ptr<logger> logger_ptr ;

wstring get_logfile_name() ;

class file_logger : public logger
{
	const wstring m_logfile_name ;
	std::ofstream out ;
public:

	file_logger();

	~file_logger()
	{
	}
	void write_entry(const string level, const string msg);
	void send_report(const CString language, const CString filename);
};

class logging
{
public:

	static string get_datestring();
	static void write_entry(LOGGING_LEVEL level, string level_str, string msg);

	static void log_verbose(const string msg);
	static void log_debug(const string msg);
	static void log_warn(const string msg);
	static void log_error(const string msg);
	static void log_exception(except::CException &e);
	static void log_exception(_com_error &e);
	static void log_exception(std::exception &e);

	static wstring format_exception_msg( const wstring &msg );

	static void set_logger(logger_ptr logger);
	static logger_ptr get_logger();
	static void ensure_logger();
	static void logging::set_logging_level(LOGGING_LEVEL level);
	static void send_report(const CString language, const CString filename);
};

#if LOG_LEVEL <= LOGGING_VERBOSE
#define LOG_VERBOSE(x) logging::log_debug(x)
#else
#define LOG_VERBOSE(x) (void)(x)
#endif
#if LOG_LEVEL <= LOGGING_DEBUG
#define LOG_DEBUG(x) logging::log_debug(x)
#else
#define LOG_DEBUG(x) (void)(x)
#endif
