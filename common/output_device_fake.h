#pragma once

#include "output_device.h"

class OutputDeviceFake : public OutputDevice
{
public:
	DECLARE_SENSING_VAR ;
	wstring m_value ;
	
	void close()
	{
		SENSE("close") ;
	}
	void open(const CString filename)
	{
		SENSE("open") ;
		SENSE((LPCSTR)CStringA(filename)) ;
	}

	void write(const char* text, size_t)
	{
		SENSE("write_charp") ;
		m_value += string2wstring(string(text)) ;
	}
	void write(const string &text)
	{
		SENSE("write_string") ;
		m_value += string2wstring(text) ;
	}
	void write(const wstring text)
	{
		SENSE("write_wstring") ;
		m_value += text ;
	}
	void ensure_dirs(const CString dirname) 
	{
		SENSE("ensure_dirs") ;
		SENSE((LPCSTR)CStringA(dirname)) ;
	}
};