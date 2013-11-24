#pragma once

#include "logging.h"

#ifdef UNIT_TEST

#include <sstream>

class string_logger : public logger
{

public:
	std::stringstream out ;

	string_logger(){}
	~string_logger(){}

	void write_entry(const string level, const string msg)
	{
		out << level << "\t" << msg << "\n" ;
	}
	void send_report(const CString language, const CString filename)
	{
		out << "send_report: " << static_cast<LPCSTR>(CStringA(language)) << " " << static_cast<LPCSTR>(CStringA(language)) << "\n" ;
	}
};


struct StringLoggerTester 
{
	StringLoggerTester() : out(new string_logger) { logging::set_logger(logger_ptr(out)); }
	string get_log() { return out->out.str() ;}
	bool has_text(string text) { return get_log().find(text) != string::npos ; }
	string_logger *out;
};

#endif // UNIT_TEST