#pragma once

#include "DispatchWrapper.h"
#include "file.h"
#include "miscwrappers.h"
#include "textstream_reader.h"

typedef enum
{
	LOGGING_VERBOSE,
	LOGGING_DEBUG,
	LOGGING_WARN,
	LOGGING_ERROR
} LOGGING_LEVEL ;

#define LOG_LEVEL LOGGING_VERBOSE

void set_logging_level(LOGGING_LEVEL level) ;

class logger
{
public:
	virtual void init() = 0 ;
	virtual void log_debug(const string msg) = 0 ;
	virtual void log_warn(const string msg) = 0;
	virtual void log_error(const string msg) = 0;
	virtual void log_exception(except::CException &e) = 0 ;
	virtual void log_exception(std::exception &e) = 0 ;
	virtual void log_exception(_com_error &e) = 0 ;
	virtual void send_report(const CString language, const CString filename) = 0 ;
};

typedef boost::shared_ptr<logger> logger_ptr ;

class file_logger : public logger
{
	CString	m_logfile_name ;
public:
	DECLARE_SENSING_VAR ;

	file_logger();
	~file_logger()
	{
	}
	void init();
	string get_datestring();
	void write_entry(const string level, const string msg);
	void log_debug(const string msg)
	{
		this->write_entry("INFO", msg) ;
	}
	void log_warn(const string msg)
	{
		this->write_entry("WARN", msg) ;
	}
	void log_error(const string msg)
	{
		this->write_entry("ERROR", msg) ;
	}
	void log_exception(except::CException &e);
	void log_exception(std::exception &e);
	void log_exception(_com_error &e);
	void send_report(const CString language, const CString filename);
};

class logging
{
public:

	static void log_debug(const string msg);
	static void log_warn(const string msg);
	static void log_error(const string msg);
	static void log_exception(except::CException &e);
	static void log_exception( _com_error &e );
	static void log_exception( std::exception &e );
	static void set_logger(logger_ptr logger);
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
