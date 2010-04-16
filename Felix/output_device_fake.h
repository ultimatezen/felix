#pragma once

#include "output_device.h"

class OutputDeviceFake : public OutputDevice
{
public:
	std::vector<string> m_calls ;
	wstring m_value ;
	
	void close()
	{
		m_calls.push_back("close") ;
	}
	void open(const CString &filename)
	{
		m_calls.push_back("open") ;
		m_calls.push_back((LPCSTR)CStringA(filename)) ;
	}

	void write(const char* text, size_t)
	{
		m_calls.push_back("write_charp") ;
		m_value += string2wstring(string(text)) ;
	}
	void write(const string &text)
	{
		m_calls.push_back("write_string") ;
		m_value += string2wstring(text) ;
	}
	void write(const wstring text)
	{
		m_calls.push_back("write_wstring") ;
		m_value += text ;
	}
};